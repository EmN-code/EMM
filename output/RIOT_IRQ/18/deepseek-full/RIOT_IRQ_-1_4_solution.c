
#include <evtimer_msg.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <irq.h>
#include <msg.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void) {
    atomic_store(&interrupt_flag, 1);
}

int main(void) {
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable interrupts
    irq_enable(state);

    // Simulate an interrupt
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_sleep(1);
    }

    // Check if the interrupt was handled correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
