
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define TEST_DURATION_SEC 5

static volatile bool interrupt_flag = false;

void custom_interrupt_handler(void) {
    interrupt_flag = true;
}

int main(void) {
    xtimer_ticks32_t start_time = xtimer_now();

    // Simulate disabling interrupts
    unsigned state = irq_disable();

    // Set custom interrupt handler
    irq_set_handler(UART_IRQ_NUM, custom_interrupt_handler);

    // Simulate enabling interrupts
    irq_enable();

    // Simulate some work
    xtimer_sleep(TEST_DURATION_SEC);

    // Restore interrupt state
    irq_restore(state);

    // Check if the interrupt flag is set
    if (interrupt_flag) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
