#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mqtt.h"
#include "thread.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(0, 2)     // Example LED pin definition

static evtimer_t evtimer;
static evtimer_mbox_t evtimer_mbox;
static msg_t mbox_buffer[8];

void led_toggle(void)
{
    static bool led_state = false;
    if (led_state)
    {
        gpio_clear(LED_PIN);
    }
    else
    {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

void evtimer_callback(void *arg)
{
    led_toggle();
    puts("LED toggled.");
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    msg_init_queue(mbox_buffer, sizeof(mbox_buffer) / sizeof(msg_t));
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t evtimer_mbox_event;
    evtimer_mbox_event.mbox = mbox_buffer;

    evtimer_add_mbox(&evtimer, &evtimer_mbox_event, &evtimer_mbox);

    msg_t msg;
    while (1)
    {
        msg_receive(&msg);
        evtimer_callback(NULL);
    }

    return 0;
}