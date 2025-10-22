/*
 * Copyright (c) 2012 - 2021, Joel Bodenmann aka Tectu <joel@ugfx.io>
 * Copyright (c) 2012 - 2021, Andrew Hannam aka inmarket <inmarket@ugfx.io>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdlib.h"
#include "string.h"
#include "gfx.h"

#define RESULT_STR_LENGTH 32

void benchmark(void)
{
	gCoord height, width, rx, ry, rcx, rcy;
    gColor random_color;
	gCoord fontHeight;
	gFont font;

	// Prepare resources
	width = gdispGetWidth();
	height = gdispGetHeight();
    font = gdispOpenFont("*");
	fontHeight = gdispGetFontMetric(font, gFontHeight);

	// Show intro message
	gdispDrawStringBox(0, 0, width, 30, "uGFX - Benchmark", font, GFX_WHITE, gJustifyCenter);
	gdispDrawStringBox(0, height/2, width, 30, "5000 random rectangles", font, GFX_WHITE, gJustifyCenter);
	gfxSleepMilliseconds(3000);
	
	// Seed RNG
	srand(0);

	// Render rectangles and count ticks & pixels
	gU64 pixels = 0;
	const gTicks ticksStart = gfxSystemTicks();
	for (gU32 i = 0; i < 5000; i++) {
		random_color = (rand() % 65535);
		rx = (rand() % (width-10));
		ry = (rand() % (height-10));
		rcx = (rand() % ((width-rx)-10))+10;
		rcy = (rand() % ((height-ry)-10))+10;

		gdispFillArea(rx, ry, rcx, rcy, random_color);
		pixels += (rcx+1)*(rcy+1);
	}
	const gTicks ticksEnd = gfxSystemTicks();
	
	// Calculate result
	char str_ticks[RESULT_STR_LENGTH];
	char str_seconds[RESULT_STR_LENGTH];
	char str_pps[RESULT_STR_LENGTH];
	{		
		// Figure out how many ticks are 1 second
		const gTicks ticksPerSecond = gfxMillisecondsToTicks(1000);
		
		const gTicks ticksElapsed = ticksEnd - ticksStart;
		const float secondsElapsed = (float)ticksElapsed / (float)ticksPerSecond;
		gU32 pps = (float)pixels / secondsElapsed;

		// Produce strings
		memset(str_ticks, 0, RESULT_STR_LENGTH);
		memset(str_seconds, 0, RESULT_STR_LENGTH);
		memset(str_pps, 0, RESULT_STR_LENGTH);
		snprintg(str_ticks, RESULT_STR_LENGTH, "%d ticks", ticksElapsed);
		snprintg(str_pps, RESULT_STR_LENGTH, "%d pixels/s", pps);
	}

	// Show result
	gdispClear(GFX_BLACK);
	gdispDrawStringBox(0, 0, width, 30, "uGFX - Benchmark", font, GFX_WHITE, gJustifyCenter);
	gdispDrawStringBox(0, height/2+0*(fontHeight+10), width, 30, str_ticks, font, GFX_WHITE, gJustifyCenter);
	gdispDrawStringBox(0, height/2+1*(fontHeight+10), width, 30, str_pps, font, GFX_WHITE, gJustifyCenter);
}

int main(void) {
	gfxInit();
	
	benchmark();
    
	while (gTrue)
		gfxSleepMilliseconds(500);

	return 0;
}
