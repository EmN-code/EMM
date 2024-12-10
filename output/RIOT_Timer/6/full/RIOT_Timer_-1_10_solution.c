#include "net/sock/util.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "dht_params.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"

#define GPIO_PIN 2
#define TIMER_INTERVAL_USEC (1U * 1000U * 1000U)

typedef struct {
    void *handler;
} timer_t;

timer_t timer;

void event_handler(void) {
    printf("Toggle LED\n");
    // pseudo function to toggle LED
}

void init_timer(timer_t *timer, uint32_t interval_usec, void (*handler)(void)) {
    // Correct API call and structure
    ztimer_set_handler(&timer->handler, handler, NULL);
    ztimer_start_periodic(ztimer_nsec, &timer->handler, interval_usec);
}

int main(void) {
    // Initialize hardware specifics
    printf("System start\n");

    // Setup timer to trigger every second
    init_timer(&timer, TIMER_INTERVAL_USEC, event_handler);

    while (1) {
        // Simulate waiting and running the main program loop
        usleep(TIMER_INTERVAL_USEC);
    }

    return 0;
}