
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "event/thread.h"

#define DHT_PIN            GPIO_PIN(0, 18)
#define LED_PIN            GPIO_PIN(0, 21)
#define TEMP_THRESHOLD     25

static void read_dht_task(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        // Simulate reading DHT11 data
        temperature = 26; // Simulated temperature value
        humidity = 50;    // Simulated humidity value

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for reading DHT11 sensor
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, read_dht_task, NULL, "dht_reader");

    return 0;
}
