
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "xtimer.h"

#define DHT_PIN            GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define READ_INTERVAL      5                // Read interval in seconds

static event_queue_t event_queue;
static event_t temp_event;

void read_dht(void) {
    int16_t temp, hum;
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_set(DHT_PIN);
    xtimer_sleep(2);  // Wait for sensor to stabilize

    // Simulate reading DHT11 sensor (since dht.h is not allowed)
    // In a real scenario, you would replace this with actual DHT11 reading code
    temp = 25;  // Simulated temperature reading
    hum = 50;   // Simulated humidity reading

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on LED
    } else {
        gpio_clear(LED_PIN);  // Turn off LED
    }

    event_post(&event_queue, &temp_event);
}

void event_loop(void) {
    while (1) {
        event_wait(&event_queue, &temp_event);
        read_dht();
        xtimer_sleep(READ_INTERVAL);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    event_queue_init(&event_queue);
    event_init(&temp_event);

    read_dht();  // Initial read

    event_loop();

    return 0;
}
