#include <stdint.h>

/* Central machine state structure */
typedef struct {
    
    float   G53_x;              // Axis positions (G53 machine absolute)
    float   G53_y;
    float   G53_z;

    uint8_t homed;              // Bitfield for homing status
    uint8_t num_axes;           // Number of configured axes

    float   feed_override;
    float   spindle_speed;

    uint32_t status_flags;      // bitfield for states (running, homed, etc.)
    uint32_t last_update_ms;    // timestamp

} MachineState_t;



