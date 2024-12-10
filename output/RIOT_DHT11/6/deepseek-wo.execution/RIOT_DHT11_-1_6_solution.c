
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <periph/rtc.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <periph/i2c.h>
#include <event/thread.h>
#include <periph/gpio.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <kernel_defines.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

static void _dht_read(void)
{
    dht_t dev;
    int16_t temp, hum;

    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dev, &params) != 0) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != 0) {
        puts("DHT11 read failed");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);
}

static void _evtimer_handler(void *arg)
{
    (void)arg;
    _dht_read();
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.callback = _evtimer_handler;
    event.event.arg = NULL;

    xtimer_t timer;
    timer.target = xtimer_now() + (5 * US_PER_SEC);
    timer.long_target = timer.target;

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        xtimer_set(&timer, 5 * US_PER_SEC);
        msg_receive(&msg_queue[0]);
    }

    return 0;
}
