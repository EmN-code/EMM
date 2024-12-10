
#include "periph/gpio.h"
#include "xtimer.h"
#include "msg.h"
#include "evtimer_mbox.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define MSG_QUEUE_SIZE (8)
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

typedef struct {
    gpio_t pin;
} dht_params_t;

typedef struct {
    dht_params_t params;
} dht_t;

static dht_t dht11;
static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;

static void _dht_read(void)
{
    int16_t temp, hum;
    gpio_init(dht11.params.pin, GPIO_OUT);
    gpio_set(dht11.params.pin);
    xtimer_usleep(2000);
    gpio_clear(dht11.params.pin);
    xtimer_usleep(20000);
    gpio_set(dht11.params.pin);
    xtimer_usleep(40);
    gpio_init(dht11.params.pin, GPIO_IN_PU);

    /* Dummy read to simulate DHT11 reading */
    temp = 25;
    hum = 60;

    printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
}

static void _evtimer_mbox_handler(event_t *event)
{
    (void)event;
    _dht_read();
}

int main(void)
{
    dht_params_t params = { .pin = DHT11_PIN };
    dht11.params = params;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer, _evtimer_mbox_handler);

    evtimer_event_t event;
    event.event.callback = _evtimer_mbox_handler;
    event.event.arg = NULL;

    while (1) {
        evtimer_add(&evtimer, &event.event, INTERVAL);
        msg_receive(&msg_queue[0]);
    }

    return 0;
}
