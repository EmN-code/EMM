#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "mutex.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "stdatomic.h"

#define INTERVAL (1U * US_PER_SEC)
static bool led_state = false;

void toggle_led(void) {
    // Simulated function - in actual code replace with proper GPIO toggle functionality
    led_state = !led_state;
    printf("LED is now %s\n", led_state ? "ON" : "OFF");
}

void *timer_thread(void *arg) {
    (void)arg;
    while (1) {
        toggle_led();
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    int rc;

    rc = pthread_create(&tid, NULL, timer_thread, NULL);
    if (rc) {
        printf("Error creating thread\n");
        return 1;
    }

    pthread_join(tid, NULL);
    return 0;
}

