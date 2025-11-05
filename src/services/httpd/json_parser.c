#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "log.h"


// Skip whitespace
static const char* skip_ws(const char* s)
{
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

// Parse a string value (between quotes)
static const char* parse_string(const char* s, char* out, int max_len)
{
    s = skip_ws(s);
    if (*s != '"') return NULL;
    s++;
    
    int i = 0;
    while (*s && *s != '"' && i < max_len - 1) {
        out[i++] = *s++;
    }
    out[i] = '\0';
    
    if (*s != '"') return NULL;
    return s + 1;
}

// Parse a number or unquoted value
static const char* parse_value(const char* s, char* out, int max_len)
{
    s = skip_ws(s);
    int i = 0;
    
    while (*s && *s != ',' && *s != '}' && *s != ']' && !isspace((unsigned char)*s) && i < max_len - 1) {
        out[i++] = *s++;
    }
    out[i] = '\0';
    return s;
}

// Parse an array [1, 2, 3]
static const char* parse_array(const char* s, JsonParser* p, int parent_idx)
{
    s = skip_ws(s);
    if (*s != '[') return NULL;
    s++;
    
    int start_idx = p->count;
    p->tokens[parent_idx].array_start = start_idx;
    
    while (1) {
        s = skip_ws(s);
        if (*s == ']') break;
        
        if (p->count >= MAX_TOKENS) return NULL;
        
        JsonToken* t = &p->tokens[p->count++];
        memset(t, 0, sizeof(JsonToken));
        
        if (*s == '"') {
            t->type = JSON_STRING;
            s = parse_string(s, t->val, MAX_VAL_LEN);
        } else {
            t->type = JSON_NUMBER;
            s = parse_value(s, t->val, MAX_VAL_LEN);
        }
        
        if (!s) return NULL;
        
        s = skip_ws(s);
        if (*s == ',') s++;
    }
    
    p->tokens[parent_idx].array_count = p->count - start_idx;
    return s + 1;
}


/* JSON exported functions (see httpd_server.h)*/

// Main parser function
int json_parse(const char* json, JsonParser* p)
{
    memset(p, 0, sizeof(JsonParser));
    
    const char* s = skip_ws(json);
    if (*s != '{') return -1;
    s++;
    
    while (1) {
        s = skip_ws(s);
        if (*s == '}') break;
        if (p->count >= MAX_TOKENS) return -1;
        
        JsonToken* t = &p->tokens[p->count++];
        memset(t, 0, sizeof(JsonToken));
        
        // Parse key
        s = parse_string(s, t->key, MAX_KEY_LEN);
        if (!s) return -1;
        
        s = skip_ws(s);
        if (*s != ':') return -1;
        s++;
        
        s = skip_ws(s);
        
        // Parse value
        if (*s == '"') {
            t->type = JSON_STRING;
            s = parse_string(s, t->val, MAX_VAL_LEN);
        } else if (*s == '[') {
            t->type = JSON_ARRAY;
            s = parse_array(s, p, p->count - 1);
        } else {
            t->type = JSON_NUMBER;
            s = parse_value(s, t->val, MAX_VAL_LEN);
        }
        
        if (!s) return -1;
        
        s = skip_ws(s);
        if (*s == ',') s++;
    }
    
    return 0;
}

// Helper to get a value by key
const char* json_get(JsonParser* p, const char* key)
{
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->tokens[i].key, key) == 0) {
            return p->tokens[i].val;
        }
    }
    return NULL;
}

// Helper to get array items
int json_get_array(JsonParser* p, const char* key, char values[][MAX_VAL_LEN])
{
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->tokens[i].key, key) == 0 && p->tokens[i].type == JSON_ARRAY) {
            int start = p->tokens[i].array_start;
            int count = p->tokens[i].array_count;
            
            for (int j = 0; j < count; j++) {
                strcpy(values[j], p->tokens[start + j].val);
            }
            return count;
        }
    }
    return 0;
}

// Example usage
int json_test(void) {
    JsonParser parser;
    
    // Test 1: GPIO command
    printf("=== Test 1: GPIO Command ===\n");
    const char* json1 = "{ \"cmd\": \"gpio\", \"action\": \"mode\", \"pin\": \"13\" }";
    if (json_parse(json1, &parser) == 0) {
        printf("cmd: %s\n", json_get(&parser, "cmd"));
        printf("action: %s\n", json_get(&parser, "action"));
        printf("pin: %s\n", json_get(&parser, "pin"));
    }
    
    // Test 2: LED command
    printf("\n=== Test 2: LED Command ===\n");
    const char* json2 = "{ \"cmd\": \"led\", \"action\": \"ON\" }";
    if (json_parse(json2, &parser) == 0) {
        printf("cmd: %s\n", json_get(&parser, "cmd"));
        printf("action: %s\n", json_get(&parser, "action"));
    }
    
    // Test 3: Array values
    printf("\n=== Test 3: Array Values ===\n");
    const char* json3 = "{ \"values\": [1, 2, 3] }";
    if (json_parse(json3, &parser) == 0) {
        char values[10][MAX_VAL_LEN];
        int count = json_get_array(&parser, "values", values);
        printf("values: [");
        for (int i = 0; i < count; i++) {
            printf("%s%s", values[i], i < count - 1 ? ", " : "");
        }
        printf("]\n");
    }
    
    return 0;
}