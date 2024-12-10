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
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "periph/i2c.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"

#define INTERVAL_SEC 1
#define GPIO_PIN_LED GPIO_PIN(0,1) // Example pin

void *timer_thread(void *arg) {
    (void)arg;
    gpio_init(GPIO_PIN_LED, GPIO_OUT);
    while (1) {
        gpio_toggle(GPIO_PIN_LED);
        ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    puts("Starting timer to toggle LED every second\n");

    // Create thread for toggling the LED
    pthread_t t1;
    pthread_create(&t1, NULL, timer_thread, NULL);
    pthread_join(t1, NULL);

    return 0;
}
