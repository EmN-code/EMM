
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"

#define DHT_PIN            GPIO_PIN(0, 0)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 1)  // Example GPIO pin for LED
#define TEMP_THRESHOLD     30              // Temperature threshold in Celsius

static void dht_read_task(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        // Simulate reading DHT11 sensor
        temperature = 25;  // Simulated temperature value
        humidity = 50;     // Simulated humidity value

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }

        thread_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for DHT reading task
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, dht_read_task, NULL, "dht_read");

    return 0;
}
