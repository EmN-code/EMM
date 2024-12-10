#include "xtimer.h"
#include "ztimer.h"
#include <esp_system.h>
#include <esp_wifi.h>
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"

void setup_timer(void);
void timer_callback(void *arg);

void setup_timer(void) {
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_USEC, 1000000, &msg_received, &response_msg);
    msg_post(&msg_received, &response_msg);
}

void timer_callback(void *arg) {
    // Code to turn on LED
    printf("Timer triggered: LED turned ON\n");
}

int main(void) {
    printf("Setting up the timer to trigger every second\n");
    setup_timer();

    // Infinite loop to keep the program running
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return 0;
}