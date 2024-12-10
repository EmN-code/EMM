
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <thread.h>
#include <irq.h>

// Define atomic variables
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle interrupts
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1); // Increment atomic variable
}

int main(void)
{
    // Initialize the atomic variable
    atomic_store(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation
    atomic_fetch_add(&atomic_var, 1);

    // Restore interrupts
    irq_restore(state);

    // Busy wait to simulate some work
    for (int i = 0; i < 1000000; i++) {}

    // Verify the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
