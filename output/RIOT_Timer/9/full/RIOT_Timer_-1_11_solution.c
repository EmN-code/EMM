#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "dht_params.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define TIMER_INTERVAL (1U * 1000000U)  // Timer interval in microseconds

void toggle_led(void) {
    // Dummy function to simulate LED toggling
    static bool led_state = false;
    led_state = !led_state;
    printf("LED is now %s\n", led_state ? "ON" : "OFF");
}

void timer_callback(void *arg) {
    (void)arg;
    toggle_led();
}

int main(void) {
    ztimer_t timer;
    int rc;
    memset(&timer, 0, sizeof(timer));

    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.interval = TIMER_INTERVAL;

    rc = ztimer_set(&timer, timer.interval);
    if (rc != 0) {
        fprintf(stderr, "Failed to set timer\n");
        return EXIT_FAILURE;
    }

    while (1) {
        // wait
        ztimer_sleep(TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}