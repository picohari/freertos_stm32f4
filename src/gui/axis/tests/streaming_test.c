/* ============================================================================
 * File: streaming_test.c
 * Description: Implementation of streaming tests
 * ============================================================================ */

#include "gfx.h"
#include "streaming_test.h"

/**
 * @brief Test 1: Horizontal gradient (Red to Green)
 */
void Test_StreamGradient(coord_t x, coord_t y, coord_t width, coord_t height) {
    /* Start streaming region */
    gdispStreamStart(x, y, width, height);
    
    /* Stream pixels row by row */
    for (coord_t row = 0; row < height; row++) {
        for (coord_t col = 0; col < width; col++) {
            /* Calculate color: gradient from red to green */
            uint8_t red = 255 - (col * 255 / width);
            uint8_t green = (col * 255 / width);
            color_t color = RGB2COLOR(red, green, 0);
            
            gdispStreamColor(color);
        }
    }
    
    /* Stop streaming */
    gdispStreamStop();
}

/**
 * @brief Test 2: Checkerboard pattern (8x8 squares)
 */
void Test_StreamCheckerboard(coord_t x, coord_t y, coord_t width, coord_t height) {
    const coord_t square_size = 8;
    
    gdispStreamStart(x, y, width, height);
    
    for (coord_t row = 0; row < height; row++) {
        for (coord_t col = 0; col < width; col++) {
            /* Determine if this pixel is black or white */
            coord_t sq_row = row / square_size;
            coord_t sq_col = col / square_size;
            
            color_t color = ((sq_row + sq_col) % 2) ? White : Black;
            
            gdispStreamColor(color);
        }
    }
    
    gdispStreamStop();
}

/**
 * @brief Test 3: Simple 5x7 character pattern (letter 'A')
 * Demonstrates streaming a manually-defined glyph
 */
void Test_StreamTextPattern(coord_t x, coord_t y) {
    /* 5x7 bitmap for letter 'A' (1 = text, 0 = background) */
    const uint8_t pattern_A[7][5] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1}
    };
    
    const coord_t width = 5;
    const coord_t height = 7;
    const color_t text_color = Green;
    const color_t bg_color = Black;
    
    gdispStreamStart(x, y, width, height);
    
    for (coord_t row = 0; row < height; row++) {
        for (coord_t col = 0; col < width; col++) {
            color_t color = pattern_A[row][col] ? text_color : bg_color;
            gdispStreamColor(color);
        }
    }
    
    gdispStreamStop();
}

/**
 * @brief Run all tests with visual spacing
 */
void Test_RunAllStreamingTests(void) {
    /* Clear screen */
    gdispClear(Black);
    
    /* Test 1: Gradient at top */
    Test_StreamGradient(10, 10, 200, 30);
    
    /* Test 2: Checkerboard in middle */
    Test_StreamCheckerboard(10, 50, 128, 64);
    
    /* Test 3: Text pattern at bottom */
    Test_StreamTextPattern(10, 130);
    Test_StreamTextPattern(20, 130);  // Draw two 'A's
    Test_StreamTextPattern(30, 130);
}
