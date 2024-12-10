
#include "periph/gpio.h"
#include "ztimer.h"
#include "thread.h"
#include "fmt.h"

#define DHT_PIN            GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 21)  // Example GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define STACK_SIZE         512              // Stack size for the thread
#define PRIORITY           5                // Priority for the thread

static char stack[STACK_SIZE];

void read_dht_and_check_temp(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        // Simulate reading temperature and humidity from DHT11
        // Replace this with actual DHT11 reading code if allowed
        temp = 25;  // Example temperature reading
        hum = 50;   // Example humidity reading

        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED
        } else {
            gpio_clear(LED_PIN);  // Turn off LED
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to read DHT11 and check temperature
    thread_create(stack, sizeof(stack), PRIORITY, 0, read_dht_and_check_temp, NULL, "temp_check");

    return 0;
}
