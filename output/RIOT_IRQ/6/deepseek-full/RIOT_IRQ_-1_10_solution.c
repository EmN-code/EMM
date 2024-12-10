
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <stddef.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Initialize the atomic variable
    atomic_store(&atomic_var, 0);

    // Simulate an interrupt
    interrupt_handler();

    // Verify the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
