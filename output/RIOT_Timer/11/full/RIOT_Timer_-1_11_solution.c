#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mbox.h>
#include <time_units.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval 1 second

static ztimer_t timer;

void turn_on_led(void)
{
    // Implementation to turn on LED
    printf("LED is turned ON\n");
}

static void timer_callback(void *arg)
{
    (void)arg;
    turn_on_led();
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
}

int main(void)
{
    // Initialize message queue (assuming an array of 8 msg_t structures)
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    
    // Initialize ztimer
    ztimer_init();

    // Set up the timer
    timer.callback = timer_callback;
    timer.callback_arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    while (1) {
        // Main loop
    }

    return 0;
}