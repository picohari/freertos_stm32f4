#ifndef _INISETTINGS_H_
#define _INISETTINGS_H_


#include "dictionary.h"

void ini_create_file(void);

void ini_write_file(dictionary * iniDict);

int ini_parse_file(const char * ini_name);

#endif /*_INISETTINGS_H_*/