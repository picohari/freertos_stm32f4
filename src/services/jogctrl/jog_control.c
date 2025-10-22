#include "handwheel.h"
#include <math.h>
#include <stddef.h>

// Range increments per step
static const double range_steps[] = { 0.001, 0.01, 0.1, 1.0, 10.0 };

// ----------------------------------------------------------
// Internal helper: handle 16-bit rollover
// ----------------------------------------------------------
static inline int16_t encoder_delta(int16_t current, int16_t prev)
{
    int16_t delta = current - prev;
    if (delta > 32767) delta -= ENCODER_CPR;
    else if (delta < -32768) delta += ENCODER_CPR;
    return delta;
}

// ----------------------------------------------------------
// Public API implementations
// ----------------------------------------------------------

void handwheel_init(HandwheelSystem *sys,
                    double start_x, double start_y, double start_z,
                    int16_t encoder_start)
{
    if (!sys) return;

    for (int i = 0; i < AXIS_COUNT; ++i) {
        sys->axes[i].prev_encoder = encoder_start;
        sys->axes[i].vel_mm_s = 0.0;

        switch (i) {
            case AXIS_X: sys->axes[i].pos_mm = start_x; break;
            case AXIS_Y: sys->axes[i].pos_mm = start_y; break;
            case AXIS_Z: sys->axes[i].pos_mm = start_z; break;
            default: break;
        }
    }

    sys->active_axis = AXIS_X;
    sys->range_mode = RANGE_MEDIUM;
}

void handwheel_update(HandwheelSystem *sys, int16_t encoder_value)
{
    if (!sys) return;

    HandwheelAxisState *axis = &sys->axes[sys->active_axis];
    int16_t delta = encoder_delta(encoder_value, axis->prev_encoder);
    axis->prev_encoder = encoder_value;

    if (delta == 0) {
        // small velocity decay for natural stop
        axis->vel_mm_s *= 0.95;
        return;
    }

    // 1) Determine spin rate (pulses per second)
    double pulses_per_sec = fabs((double)delta) / CONTROL_PERIOD_SEC;

    // 2) Feed scaling based on speed
    double scale = fmin(1.0, pulses_per_sec / 200.0);
    scale = pow(scale, 0.7);

    // 3) Compute desired velocity
    double base_step = range_steps[sys->range_mode];
    double desired_vel = (delta > 0 ? 1 : -1) *
                         (base_step * 1000.0 * scale);

    if (desired_vel >  MAX_FEEDRATE_MM_S) desired_vel =  MAX_FEEDRATE_MM_S;
    if (desired_vel < -MAX_FEEDRATE_MM_S) desired_vel = -MAX_FEEDRATE_MM_S;

    // 4) Apply acceleration ramp
    double max_dv = MAX_ACCEL_MM_S2 * CONTROL_PERIOD_SEC;
    double dv = desired_vel - axis->vel_mm_s;
    if (dv >  max_dv) dv =  max_dv;
    if (dv < -max_dv) dv = -max_dv;
    axis->vel_mm_s += dv;

    // 5) Integrate velocity to position
    axis->pos_mm += axis->vel_mm_s * CONTROL_PERIOD_SEC;
}

void handwheel_set_axis(HandwheelSystem *sys, AxisSelect axis)
{
    if (!sys) return;
    if (axis < AXIS_COUNT)
        sys->active_axis = axis;
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






/* Encoder ONLY version */

#include "jog_control.h"

static const float jog_steps_mm[] = {
    0.001f, 0.01f, 0.1f, 1.0f, 10.0f
};

void jog_init(HandwheelState *h, AxisID axis) {
    h->last_encoder = 0;
    h->delta_accum = 0;
    h->range = JOG_RANGE_FINE;
    h->axis = axis;
}

// --- Core encoder delta and jog calculation ---
float jog_process(HandwheelState *h, uint16_t new_encoder)
{
    int16_t delta = (int16_t)(new_encoder - h->last_encoder);
    h->last_encoder = new_encoder;

    // Optional: deadband or debounce can go here

    float increment = (float)delta * jog_steps_mm[h->range];
    return increment;  // in mm, positive or negative
}

void jog_set_range(HandwheelState *h, JogRange new_range) {
    h->range = new_range;
}

void jog_set_axis(HandwheelState *h, AxisID new_axis) {
    h->axis = new_axis;
}
