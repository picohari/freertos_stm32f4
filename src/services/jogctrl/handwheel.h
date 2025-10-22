#ifndef HANDWHEEL_H
#define HANDWHEEL_H

#include <stdint.h>

#define CONTROL_PERIOD_SEC   0.01
#define MAX_FEEDRATE_MM_S    100.0
#define MAX_ACCEL_MM_S2      800.0
#define ENCODER_CPR          65536

typedef enum {
    RANGE_FINE = 0,
    RANGE_MEDIUM_FINE,
    RANGE_MEDIUM,
    RANGE_COARSE,
    RANGE_VERY_COARSE
} RangeMode;

typedef enum {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z,
    AXIS_COUNT
} AxisSelect;

typedef struct {
    int16_t prev_encoder;
    double  pos_mm;
    double  vel_mm_s;
} HandwheelAxisState;

typedef struct {
    HandwheelAxisState axes[AXIS_COUNT];
    uint8_t active_mask;   // bitmask: bit0=X, bit1=Y, bit2=Z
    RangeMode range_mode;
} HandwheelSystem;

void handwheel_init(HandwheelSystem *sys,
                    double start_x, double start_y, double start_z,
                    int16_t encoder_start);

void handwheel_update(HandwheelSystem *sys, int16_t encoder_value);

void handwheel_set_axis_mask(HandwheelSystem *sys, uint8_t mask);
void handwheel_set_range(HandwheelSystem *sys, RangeMode mode);

double handwheel_get_position(const HandwheelSystem *sys, AxisSelect axis);

#endif // HANDWHEEL_H
