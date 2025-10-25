
#ifndef LINUXCNC_CTRL_H
#define LINUXCNC_CTRL_H

#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"


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

} __attribute__((packed)) MachineState_t;


/* Axis mask bits for homing status */
#define AXIS_MASK_X   (1 << 0)
#define AXIS_MASK_Y   (1 << 1)
#define AXIS_MASK_Z   (1 << 2)


void LinuxCNC_Init(void);

/* Thread-safe accessors */
void LinuxCNC_SetState(const MachineState_t *new_state);
void LinuxCNC_GetState(MachineState_t *out_state);

/* Partial updates (faster) */
void LinuxCNC_UpdateHomed(uint8_t homed, uint8_t num_axes);
void LinuxCNC_SetStatusFlags(uint32_t flags);

/* Helper functions */
bool LinuxCNC_ParseUDP(const uint8_t *data, size_t len, MachineState_t *out);


#endif /* LINUXCNC_CTRL */
