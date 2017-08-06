


#include "iniparser.h"
#include "dictionary.h"

#include "ff_gen_drv.h"
#include "usbh_diskio.h"

#include "uart.h"







void ini_write_file(dictionary * iniDict) {

	FIL iniFile;

	if((f_open(&iniFile, "SETTINGS.water", FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
		writef("Could not create SETTINGS.water\r\n");
		return;
	}
	else {
		iniparser_dump_ini(iniDict, &iniFile);
		f_close(&iniFile);
		writef("Wrote SETTINGS.water\r\n");
	}
}




//Overwrite the settings file with the defaults, or create a new file from scratch (NULL value = section header)
void ini_create_file(void) {

	dictionary* iniDict = dictionary_new(0);
	
	iniparser_set(iniDict, "general", NULL);
	iniparser_set(iniDict, "general:level_sensor", "0000/0000");
	iniparser_set(iniDict, "general:date", "01.01.2017");
	iniparser_set(iniDict, "general:time", "00:00");
	
	iniparser_set(iniDict, "solar", NULL);
	iniparser_set(iniDict, "solar:power", "1");
	iniparser_set(iniDict, "solar:voltage", "20.0");
	
	iniparser_set(iniDict, "network", NULL);
	iniparser_set(iniDict, "network:ipv4", "127.000.000.001");
	iniparser_set(iniDict, "network:gateway", "127.000.000.254");
	iniparser_set(iniDict, "network:netmask", "255.255.255.000");
	iniparser_set(iniDict, "network:dhcp", "0");
	
	iniparser_set(iniDict, "rainwater", NULL);
	iniparser_set(iniDict, "rainwater:membranes", "1");
	iniparser_set(iniDict, "rainwater:cycles", "00/00");
	
	iniparser_set(iniDict, "infiltration", NULL);
	
	iniparser_set(iniDict, "greywater", NULL);
	iniparser_set(iniDict, "greywater:suction_overflow", "0");
	iniparser_set(iniDict, "greywater:sludge_pump", "0/00");
	iniparser_set(iniDict, "greywater:t1_time", "09:15");
	iniparser_set(iniDict, "greywater:t2_time", "21:15");
	iniparser_set(iniDict, "greywater:air", "00/15");
	
	iniparser_set(iniDict, "fireprotection", NULL);
	
	ini_write_file(iniDict);
	
	iniparser_freedict(iniDict);

}


int ini_parse_file(const char * ini_name)
{
    dictionary  *   ini ;

    /* Some temporary variables to hold query results */
    //int             b ;
    //int             i ;
    //double          d ;
    const char  *   s ;

    ini = iniparser_load(ini_name);

    if (ini == NULL) {
        writef("Cannot parse file: %s\r\n", ini_name);
        return -1 ;
    }

    iniparser_dump_uart(ini);

    /* Get attribute */
    s = iniparser_getstring(ini, "general:date", NULL);
    writef("Date:   [%s]\r\n", s ? s : "UNDEF");

    iniparser_freedict(ini);

    return 0 ;
}
