
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <irq.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <timex.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <stddef.h>
#include <ztimer.h>

atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the interrupt flag
    atomic_store(&interrupt_flag, 0);

    // Register the interrupt handler
    irq_set_handler(ESP_INTR_SOURCE_TIMER, interrupt_handler, NULL);

    // Disable interrupts
    irq_disable();

    // Perform some atomic operation to simulate critical section
    atomic_fetch_add(&interrupt_flag, 1);

    // Restore interrupts
    irq_restore(irq_disable());

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) != 1) {
        // Busy wait
    }

    // Verify the interrupt was handled correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
