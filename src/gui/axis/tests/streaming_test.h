/* ============================================================================
 * File: streaming_test.h
 * Description: Minimal test for GDISP streaming functionality
 * ============================================================================ */

#ifndef STREAMING_TEST_H
#define STREAMING_TEST_H

#include "gfx.h"

/**
 * @brief Test 1: Draw a simple horizontal gradient using streaming
 * @param x X position
 * @param y Y position
 * @param width Width of gradient bar
 * @param height Height of gradient bar
 */
void Test_StreamGradient(coord_t x, coord_t y, coord_t width, coord_t height);

/**
 * @brief Test 2: Draw a checkerboard pattern using streaming
 * @param x X position
 * @param y Y position
 * @param width Width of checkerboard
 * @param height Height of checkerboard
 */
void Test_StreamCheckerboard(coord_t x, coord_t y, coord_t width, coord_t height);

/**
 * @brief Test 3: Stream a simple text pattern (manual pixel data)
 * @param x X position
 * @param y Y position
 */
void Test_StreamTextPattern(coord_t x, coord_t y);

/**
 * @brief Run all streaming tests
 */
void Test_RunAllStreamingTests(void);

#endif /* STREAMING_TEST_H */
