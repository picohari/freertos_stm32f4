#include <string.h>
#include "linuxcnc_ctrl.h"

static MachineState_t   machine_state;
static JogState_t       jog_state;
static HalState_t       hal_state;


static SemaphoreHandle_t m_state_mutex, j_state_mutex, h_state_mutex;

void LinuxCNC_Init(void)
{
    m_state_mutex = xSemaphoreCreateMutex();
    memset(&machine_state, 0, sizeof(machine_state));

    j_state_mutex = xSemaphoreCreateMutex();
    memset(&jog_state, 0, sizeof(jog_state));

    h_state_mutex = xSemaphoreCreateMutex();
    memset(&hal_state, 0, sizeof(hal_state));
}

void LinuxCNC_SetMachineState(const MachineState_t *new_state)
{
    if (xSemaphoreTake(m_state_mutex, portMAX_DELAY) == pdTRUE) {
        machine_state = *new_state;
        xSemaphoreGive(m_state_mutex);
    }
}

void LinuxCNC_GetMachineState(MachineState_t *out_state)
{
    if (xSemaphoreTake(m_state_mutex, portMAX_DELAY) == pdTRUE) {
        *out_state = machine_state;
        xSemaphoreGive(m_state_mutex);
    }
}

void LinuxCNC_SetJogState(const JogState_t *new_state)
{
    if (xSemaphoreTake(j_state_mutex, portMAX_DELAY) == pdTRUE) {
        jog_state = *new_state;
        xSemaphoreGive(j_state_mutex);
    }
}

void LinuxCNC_GetJogState(JogState_t *out_state)
{
    if (xSemaphoreTake(j_state_mutex, portMAX_DELAY) == pdTRUE) {
        *out_state = jog_state;
        xSemaphoreGive(j_state_mutex);
    }
}

void LinuxCNC_SetHalState(const HalState_t *new_state)
{
    if (xSemaphoreTake(h_state_mutex, portMAX_DELAY) == pdTRUE) {
        hal_state = *new_state;
        xSemaphoreGive(h_state_mutex);
    }
}

void LinuxCNC_GetHalState(HalState_t *out_state)
{
    if (xSemaphoreTake(h_state_mutex, portMAX_DELAY) == pdTRUE) {
        *out_state = hal_state;
        xSemaphoreGive(h_state_mutex);
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

bool LinuxCNC_ParseHAL(const uint8_t *data, size_t len, HalState_t *out)
{
    if (len < sizeof(HalState_t))
        return false;

    const uint8_t *ptr = data;
    
    memcpy(&out->fb.pos[0],     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.pos[1],     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.pos[2],     ptr, sizeof(float));    ptr += sizeof(float);

    memcpy(&out->fb.vel[0],     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.vel[1],     ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.vel[2],     ptr, sizeof(float));    ptr += sizeof(float);

    memcpy(&out->fb.scale[0],   ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.scale[1],   ptr, sizeof(float));    ptr += sizeof(float);
    memcpy(&out->fb.scale[2],   ptr, sizeof(float));    ptr += sizeof(float);

    memcpy(&out->fb.enable[0],  ptr, sizeof(uint8_t));  ptr += sizeof(uint8_t);
    memcpy(&out->fb.enable[1],  ptr, sizeof(uint8_t));  ptr += sizeof(uint8_t);
    memcpy(&out->fb.enable[2],  ptr, sizeof(uint8_t));  ptr += sizeof(uint8_t);

    memcpy(&out->fb.control,    ptr, sizeof(uint8_t));  ptr += sizeof(uint8_t);
    memcpy(&out->fb.io,         ptr, sizeof(uint16_t)); ptr += sizeof(uint16_t);

    return true;
}
