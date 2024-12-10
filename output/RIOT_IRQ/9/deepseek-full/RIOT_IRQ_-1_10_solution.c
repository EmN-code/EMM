
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <irq.h>
#include <msg.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

atomic_int interrupt_state = ATOMIC_VAR_INIT(0);

void simulate_interrupt(void) {
    atomic_store(&interrupt_state, 1);
    printf("Interrupts disabled\n");
    xtimer_sleep(1); // Simulate some work
    atomic_store(&interrupt_state, 0);
    printf("Interrupts restored\n");
}

int main(void) {
    printf("Starting interrupt handling test\n");

    while (1) {
        simulate_interrupt();
        xtimer_sleep(2); // Wait before next interrupt simulation
    }

    return 0;
}
