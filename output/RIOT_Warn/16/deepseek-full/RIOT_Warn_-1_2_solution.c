
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "event/thread.h"

#define LED_PIN            GPIO_PIN(0, 13)  // Example GPIO pin for LED
#define DHT_PIN            GPIO_PIN(0, 14)  // Example GPIO pin for DHT11
#define TEMP_THRESHOLD     25               // Temperature threshold in Celsius
#define SLEEP_DURATION     5                // Sleep duration in seconds

static event_queue_t event_queue;

void read_dht_and_check_temp(void)
{
    int16_t temp, hum;
    if (dht_read(DHT_PIN, &temp, &hum) == 0) {
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
}

void timer_callback(void *arg)
{
    (void)arg;
    event_post(&event_queue, (event_t *)read_dht_and_check_temp);
}

int main(void)
{
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT sensor
    dht_init(DHT_PIN);

    // Initialize event queue
    event_queue_init(&event_queue);

    // Set up a periodic timer to read DHT11 every 5 seconds
    rtc_timer_t timer;
    rtc_timer_init(&timer, timer_callback, NULL);
    rtc_timer_set_periodic(&timer, SLEEP_DURATION * 1000000);

    // Main loop to process events
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
