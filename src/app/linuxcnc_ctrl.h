
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

#pragma pack(1)

/* Central Jogwheel state structure (local JogWheel device)*/
typedef struct {
    /* OUT: Axis positions & speeds */
    struct {
        float     pos[3];         // Actual machine position
        float     vel[3];         // Calculated velocity from LinuxCNC
        float     scale[3];
        uint8_t   enable[3];
        uint8_t   control;        // Control register feedback
        uint16_t  io;             // IO register multipurpose
    } fb;
} __attribute__((packed)) HalState_t;   // Sends:

typedef struct {
    /* OUT: Jogwheel state*/
    struct {
        float      encoder_value; // 4
        int16_t    encoder_count; // 2
        uint16_t   io;            // 2
        uint8_t    active_mask;   // 1 bitmask: bit0=X, bit1=Y, bit2=Z
        uint8_t    control;       // 1
        AxisSelect axis_select;   // 1
        RangeMode  range_mode;    // 1
    } jogstate;
} __attribute__((packed)) JogState_t;   // Sends: 0100002041

#pragma pack()


/* Axis mask bits for homing status */
#define AXIS_MASK_X   (1 << 0)
#define AXIS_MASK_Y   (1 << 1)
#define AXIS_MASK_Z   (1 << 2)


void LinuxCNC_Init(void);


/* Partial updates (faster) */
void LinuxCNC_UpdateHomed(uint8_t homed, uint8_t num_axes);
void LinuxCNC_SetStatusFlags(uint32_t flags);

/* Helper functions */
bool LinuxCNC_ParseUDP(const uint8_t *data, size_t len, MachineState_t *out);
bool LinuxCNC_ParseHAL(const uint8_t *data, size_t len, HalState_t *out);

/* Thread-safe accessors */
void LinuxCNC_SetMachineState(const MachineState_t *new_state);
void LinuxCNC_GetMachineState(MachineState_t *out_state);

void LinuxCNC_SetJogState(const JogState_t * new_state);
void LinuxCNC_GetJogState(JogState_t *out_state);

void LinuxCNC_SetHalState(const HalState_t *new_state);
void LinuxCNC_GetHalState(HalState_t *out_state);


#endif /* LINUXCNC_CTRL */
