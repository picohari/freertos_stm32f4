//*****************************************************************************
//
// cgifuncs.c - Helper functions related to CGI script parameter parsing.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 5570 of the EK-LM3S6965 Firmware Package.
//
//*****************************************************************************

#ifndef __CGIFUNCS_H__
#define __CGIFUNCS_H__

#include <stdio.h>

//*****************************************************************************
//
// Prototypes of functions exported by this module.
//
//*****************************************************************************
int FindCGIParameter(const char *pcToFind, char *pcParam[], int iNumParams);
uint8_t IsValidHexDigit(const char cDigit);
unsigned char HexDigit(const char cDigit);
uint8_t DecodeHexEscape(const char *pcEncoded, char *pcDecoded);
unsigned long EncodeFormString(const char *pcDecoded, char *pcEncoded, unsigned long ulLen);
unsigned long DecodeFormString(const  char *pcEncoded, char *pcDecoded, unsigned long ulLen);
uint8_t CheckDecimalParam(const char *pcValue, long *plValue);
long GetCGIParam(const char *pcName, char *pcParams[], char *pcValue[], int iNumParams, uint8_t *pbError);

#endif // __CGIFUNCS_H__