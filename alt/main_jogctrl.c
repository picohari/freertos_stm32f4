#include "jog_control.h"
#include "udp_comm.h"
#include <stdio.h>

int main(void)
{
    HandwheelState handwheel;
    jog_init(&handwheel, AXIS_X);

    // Example loop: simulate encoder and communication
    uint16_t encoder_value = 0;

    while (1)
    {
        // Simulate encoder readout
        encoder_value += 3;  // assume user turns it 3 steps

        float delta = jog_process(&handwheel, encoder_value);

        if (delta != 0.0f) {
            JogPacket pkt = { handwheel.axis, delta };
            udp_send_jog(&pkt);
        }

        // Example: read position data back from LinuxCNC
        AxisPositionPacket pos;
        if (udp_recv_axis_position(&pos)) {
            printf("Axis Positions: X=%.3f  Y=%.3f  Z=%.3f\n",
                   pos.x, pos.y, pos.z);
        }

        // delay or tick
    }
    return 0;
}
