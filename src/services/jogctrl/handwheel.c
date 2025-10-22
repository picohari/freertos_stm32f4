#include "handwheel.h"
#include <math.h>
#include <stddef.h>

static const double range_steps[] = { 0.001, 0.01, 0.1, 1.0, 10.0 };

static inline int16_t encoder_delta(int16_t current, int16_t prev)
{
    int16_t delta = current - prev;
    if (delta > 32767) delta -= ENCODER_CPR;
    else if (delta < -32768) delta += ENCODER_CPR;
    return delta;
}

void handwheel_init(HandwheelSystem *sys,
                    double start_x, double start_y, double start_z,
                    int16_t encoder_start)
{
    if (!sys) return;

    for (int i = 0; i < AXIS_COUNT; ++i) {
        sys->axes[i].prev_encoder = encoder_start;
        sys->axes[i].vel_mm_s = 0.0;
    }
    sys->axes[AXIS_X].pos_mm = start_x;
    sys->axes[AXIS_Y].pos_mm = start_y;
    sys->axes[AXIS_Z].pos_mm = start_z;

    sys->active_mask = 0b001; // default: X axis
    sys->range_mode = RANGE_MEDIUM;
}

void handwheel_update(HandwheelSystem *sys, int16_t encoder_value)
{
    if (!sys || sys->active_mask == 0) return;

    // Use the first axis’s previous encoder as reference
    HandwheelAxisState *ref_axis = &sys->axes[AXIS_X];
    int16_t delta = encoder_delta(encoder_value, ref_axis->prev_encoder);
    ref_axis->prev_encoder = encoder_value;

    if (delta == 0) {
        // decay velocities when idle
        for (int i = 0; i < AXIS_COUNT; ++i)
            sys->axes[i].vel_mm_s *= 0.95;
        return;
    }

    double pulses_per_sec = fabs((double)delta) / CONTROL_PERIOD_SEC;
    double scale = fmin(1.0, pulses_per_sec / 200.0);
    scale = pow(scale, 0.7);

    double base_step = range_steps[sys->range_mode];
    double desired_vel = (delta > 0 ? 1 : -1) *
                         (base_step * 1000.0 * scale);

    if (desired_vel >  MAX_FEEDRATE_MM_S) desired_vel =  MAX_FEEDRATE_MM_S;
    if (desired_vel < -MAX_FEEDRATE_MM_S) desired_vel = -MAX_FEEDRATE_MM_S;

    double max_dv = MAX_ACCEL_MM_S2 * CONTROL_PERIOD_SEC;

    // Apply same delta to all active axes
    for (int i = 0; i < AXIS_COUNT; ++i) {
        if ((sys->active_mask >> i) & 0x01) {
            HandwheelAxisState *ax = &sys->axes[i];
            double dv = desired_vel - ax->vel_mm_s;
            if (dv >  max_dv) dv =  max_dv;
            if (dv < -max_dv) dv = -max_dv;
            ax->vel_mm_s += dv;
            ax->pos_mm += ax->vel_mm_s * CONTROL_PERIOD_SEC;
        }
    }
}

void handwheel_set_axis_mask(HandwheelSystem *sys, uint8_t mask)
{
    if (!sys) return;
    sys->active_mask = mask & ((1 << AXIS_COUNT) - 1); // only lower 3 bits
}

void handwheel_set_range(HandwheelSystem *sys, RangeMode mode)
{
    if (!sys) return;
    if (mode <= RANGE_VERY_COARSE)
        sys->range_mode = mode;
}

double handwheel_get_position(const HandwheelSystem *sys, AxisSelect axis)
{
    if (!sys || axis >= AXIS_COUNT)
        return 0.0;
    return sys->axes[axis].pos_mm;
}
