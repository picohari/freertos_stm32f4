#include "main.h"
#include "cmsis_os.h"
#include "handwheel.h"
#include "stm32f4xx_hal.h"

// ============================================================
// External peripherals from CubeMX
// ============================================================
extern TIM_HandleTypeDef htim2; // encoder timer

// ============================================================
// FreeRTOS task handles
// ============================================================
osThreadId handwheelTaskHandle;

// ============================================================
// Handwheel system object
// ============================================================
static HandwheelSystem hw;

// ============================================================
// Helper functions for GPIO inputs
// ============================================================

// --- Read range switch (3-bit rotary switch, GPIOA PIN0..2) ---
static RangeMode read_range_mode(void)
{
    uint8_t val = 0;
    val |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ? 1 : 0);
    val |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) ? 2 : 0);
    val |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) ? 4 : 0);

    switch (val) {
        case 0: return RANGE_FINE;
        case 1: return RANGE_MEDIUM_FINE;
        case 2: return RANGE_MEDIUM;
        case 3: return RANGE_COARSE;
        case 4: return RANGE_VERY_COARSE;
        default: return RANGE_MEDIUM;
    }
}

// --- Read axis mask (3 switches, GPIOB PIN0..2) ---
static uint8_t read_axis_mask(void)
{
    uint8_t mask = 0;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) mask |= (1 << AXIS_X);
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) mask |= (1 << AXIS_Y);
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)) mask |= (1 << AXIS_Z);
    return mask;
}

// ============================================================
// Encoder read helper
// ============================================================
static inline int16_t read_encoder_count(void)
{
    return (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
}

// ============================================================
// Handwheel Task
// ============================================================
void StartHandwheelTask(void const * argument)
{
    // Start encoder interface
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    // Initialize handwheel system
    handwheel_init(&hw, 0.0, 0.0, 0.0, read_encoder_count());

    TickType_t lastWakeTime = osKernelSysTick();

    for (;;) {
        // Read hardware inputs
        RangeMode range = read_range_mode();
        uint8_t mask = read_axis_mask();
        int16_t encoder_val = read_encoder_count();

        // Update system parameters
        handwheel_set_range(&hw, range);
        handwheel_set_axis_mask(&hw, mask);

        // Run the control update
        handwheel_update(&hw, encoder_val);

        // Here you could send new coordinates to CNC controller
        // e.g., via UART, CAN, or shared variable
        // Example:
        // send_axis_target(AXIS_X, handwheel_get_position(&hw, AXIS_X));

        // Wait until next 10 ms tick
        osDelayUntil(&lastWakeTime, (uint32_t)(CONTROL_PERIOD_SEC * 1000.0));
    }
}

// ============================================================
// main()
// ============================================================
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_FREERTOS_Init(); // if CubeMX autogen

    // Manually create handwheel task (if not via CubeMX)
    osThreadDef(handwheelTask, StartHandwheelTask, osPriorityNormal, 0, 512);
    handwheelTaskHandle = osThreadCreate(osThread(handwheelTask), NULL);

    osKernelStart(); // start scheduler

    while (1) {
        // Should never reach here
    }
}
