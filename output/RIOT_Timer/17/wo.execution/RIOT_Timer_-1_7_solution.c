# Include necessary headers in descending order
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "net/emcute.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)

void *led_thread(void *arg) {
    while (1) {
        gpio_write(LED_PIN, 1);
        xtimer_sleep(1);
        gpio_write(LED_PIN, 0);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to handle LED toggling
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, led_thread, NULL, "led_thread");
    
    // Let the main thread sleep
    while (1) {
        xtimer_sleep(60);
    }
    return 0;
}
