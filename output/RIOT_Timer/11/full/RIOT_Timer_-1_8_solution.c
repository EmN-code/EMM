#include "net/sock/util.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN 2  // Define the pin where LED is connected

void led_on(void) {
    // Implementation of turning on the LED
    printf("LED is turned on\n");
}

int main(void) {
    xtimer_usleep(TIMER_INTERVAL); // Let the system stabilize

    while (1) {
        xtimer_usleep(TIMER_INTERVAL); // Wait for the timer interval
        led_on(); // Turn on the LED
    }

    return 0;
}