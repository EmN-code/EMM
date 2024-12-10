
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <time_units.h>
#include <timex.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stddef.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <ztimer.h>

// Define a simple atomic variable
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Define a simple interrupt handler
void my_irq_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Enable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(ESP32_IRQ_NUM, my_irq_handler);

    // Enable the specific interrupt
    irq_enable_interrupt(ESP32_IRQ_NUM);

    // Restore interrupts to the previous state
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(ESP32_IRQ_NUM);

    // Disable the specific interrupt
    irq_disable_interrupt(ESP32_IRQ_NUM);

    // Print the value of the atomic variable to verify the interrupt handling
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
