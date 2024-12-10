#include "ztimer.h"
#include "timex.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "event/thread.h"

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LED_PIN GPIO_PIN(PORT_A, 5)  // Adjust according to your board specifics

static char led_blinker_stack[THREAD_STACKSIZE_MAIN];

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        gpio_toggle(LED_PIN);
        ztimer_sleep(ZTIMER_MSEC, 1000);  // sleep for 1 second
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(led_blinker_stack, sizeof(led_blinker_stack),
                  THREAD_PRIORITY, 0, led_blinker, NULL, "led_blinker");
    
    while (1) {
        thread_yield();  // Yield control to other threads
    }
    return 0;
}