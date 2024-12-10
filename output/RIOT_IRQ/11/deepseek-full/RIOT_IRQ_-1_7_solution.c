
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mutex.h>
#include <timex.h>
#include <dht_params.h>
#include <ztimer.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)

atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

void timer_callback(void *arg)
{
    (void)arg;
    interrupt_handler();
}

int main(void)
{
    xtimer_t timer;
    xtimer_init(&timer, timer_callback, NULL);

    printf("Starting interrupt test...\n");

    // Set the timer to trigger the interrupt handler
    xtimer_set(&timer, TIMER_INTERVAL);

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_sleep(1);
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
