#include <string.h>
#include "linuxcnc_ctrl.h"

static MachineState_t machine_state;
static SemaphoreHandle_t state_mutex;


void LinuxCNC_Init(void)
{
    state_mutex = xSemaphoreCreateMutex();
    memset(&machine_state, 0, sizeof(machine_state));
}

void LinuxCNC_SetState(const MachineState_t *new_state)
{
    if (xSemaphoreTake(state_mutex, portMAX_DELAY) == pdTRUE) {
        machine_state = *new_state;
        xSemaphoreGive(state_mutex);
    }
}

void LinuxCNC_GetState(MachineState_t *out_state)
{
    if (xSemaphoreTake(state_mutex, portMAX_DELAY) == pdTRUE) {
        *out_state = machine_state;
        xSemaphoreGive(state_mutex);
    }
}

bool LinuxCNC_ParseUDP(const uint8_t *data, size_t len, MachineState_t *out)
{
    if (len < sizeof(MachineState_t))
        return false;

    const uint8_t *ptr = data;

    memcpy(&out->G53_x,     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->G53_y,     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->G53_z,     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->homed,     ptr, sizeof(uint8_t));  ptr += sizeof(uint8_t);

    return true;
}

