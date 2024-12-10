
#include "periph/gpio.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * US_PER_SEC)

static void _dht_read_handler(void *arg)
{
    (void)arg;
    int16_t temp, hum;
    dht_t dev;
    dht_params_t params = { .pin = DHT_PIN };

    if (dht_init(&dev, &params) != 0) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != 0) {
        puts("Failed to read from DHT11");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
}

int main(void)
{
    msg_t msg_queue[8];
    mbox_t mbox = { .msg_array = msg_queue };
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_mbox(&evtimer, &mbox);

    event.event.offset = SLEEP_INTERVAL;
    event.event.callback = _dht_read_handler;
    event.event.arg = NULL;

    evtimer_add_msg(&evtimer, &event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        evtimer_add_msg(&evtimer, &event);
    }

    return 0;
}
