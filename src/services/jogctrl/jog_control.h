#ifndef JOG_CONTROL_H
#define JOG_CONTROL_H

#include <stdint.h>

// ==========================================================
// CONFIGURATION CONSTANTS
// ==========================================================

#define CONTROL_PERIOD_SEC   0.01     // Control loop period [s]
#define MAX_FEEDRATE_MM_S    100.0    // Maximum jogging velocity [mm/s]
#define MAX_ACCEL_MM_S2      800.0    // Acceleration limit [mm/s²]
#define ENCODER_CPR          65536    // Encoder count range (16-bit rollover)

// ==========================================================
// ENUMS
// ==========================================================

typedef enum {
    RANGE_FINE = 0,        // 0.001 mm per step
    RANGE_MEDIUM_FINE,     // 0.01 mm per step
    RANGE_MEDIUM,          // 0.1 mm per step
    RANGE_COARSE,          // 1.0 mm per step
    RANGE_VERY_COARSE      // 10.0 mm per step
} RangeMode;

typedef enum {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z,
    AXIS_COUNT
} AxisSelect;

// ==========================================================
// STRUCTURES
// ==========================================================

typedef struct {
    int16_t prev_encoder;   // Previous encoder count
    double  pos_mm;         // Current target position [mm]
    double  vel_mm_s;       // Current velocity [mm/s]
} HandwheelAxisState;

typedef struct {
    HandwheelAxisState axes[AXIS_COUNT];  // One per axis
    AxisSelect active_axis;               // Currently selected axis
    RangeMode range_mode;                 // Active range increment
} HandwheelSystem;

// ==========================================================
// PUBLIC API FUNCTIONS
// ==========================================================

/**
 * Initialize the handwheel system.
 * @param sys            Pointer to system object.
 * @param start_x/y/z    Initial absolute axis positions [mm].
 * @param encoder_start  Initial encoder value.
 */
void handwheel_init(HandwheelSystem *sys,
                    double start_x, double start_y, double start_z,
                    int16_t encoder_start);

/**
 * Update the handwheel logic (to be called periodically).
 * @param sys            Pointer to system object.
 * @param encoder_value  Latest encoder reading (16-bit signed).
 */
void handwheel_update(HandwheelSystem *sys, int16_t encoder_value);

/**
 * Set the currently controlled axis.
 * @param sys   Pointer to system object.
 * @param axis  Selected axis (X/Y/Z).
 */
void handwheel_set_axis(HandwheelSystem *sys, AxisSelect axis);

/**
 * Set the range (step size) mode.
 * @param sys   Pointer to system object.
 * @param mode  Range mode.
 */
void handwheel_set_range(HandwheelSystem *sys, RangeMode mode);

/**
 * Optional helper: get current position of an axis.
 * @param sys   Pointer to system object.
 * @param axis  Axis to read.
 * @return      Position in mm.
 */
double handwheel_get_position(const HandwheelSystem *sys, AxisSelect axis);






/* Encoder ONLY version */

#if 0

// Jog range settings (mm per encoder step)
typedef enum {
    JOG_RANGE_FINE = 0,   // 0.001 mm
    JOG_RANGE_0_01,       // 0.01 mm
    JOG_RANGE_0_1,        // 0.1 mm
    JOG_RANGE_1,          // 1 mm
    JOG_RANGE_10          // 10 mm
} JogRange;

// Axis selector
typedef enum {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z
} AxisID;

// Handwheel state structure
typedef struct {
    uint16_t last_encoder;
    int32_t  delta_accum;
    JogRange range;
    AxisID   axis;
} HandwheelState;

// Function declarations
void jog_init(HandwheelState *h, AxisID axis);
float jog_process(HandwheelState *h, uint16_t new_encoder);
void jog_set_range(HandwheelState *h, JogRange new_range);
void jog_set_axis(HandwheelState *h, AxisID new_axis);

#endif

#endif
