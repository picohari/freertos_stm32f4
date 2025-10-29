
#ifndef LINUXCNC_CTRL_H
#define LINUXCNC_CTRL_H

#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"


/* Central machine state structure (remote LinuxCNC machine)*/
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


/* Central Jogwheel state structure (local JogWheel device)*/
typedef struct {

    uint8_t axis_select;
    float encoder_value;        // Absolute rotary encoder value

} __attribute__((packed)) JogState_t;   // Sends: 0100002041


/* Axis mask bits for homing status */
#define AXIS_MASK_X   (1 << 0)
#define AXIS_MASK_Y   (1 << 1)
#define AXIS_MASK_Z   (1 << 2)


void LinuxCNC_Init(void);

/* Thread-safe accessors */
void LinuxCNC_SetMachineState(const MachineState_t *new_state);
void LinuxCNC_GetMachineState(MachineState_t *out_state);

/* Partial updates (faster) */
void LinuxCNC_UpdateHomed(uint8_t homed, uint8_t num_axes);
void LinuxCNC_SetStatusFlags(uint32_t flags);

/* Helper functions */
bool LinuxCNC_ParseUDP(const uint8_t *data, size_t len, MachineState_t *out);

void LinuxCNC_SetJogState(const JogState_t * new_state);
void LinuxCNC_GetJogState(JogState_t *out_state);

#endif /* LINUXCNC_CTRL */
