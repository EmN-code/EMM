#include "xtimer.h"
#include "periph/gpio.h"

// Define GPIO pins
#define DHT11_PIN   GPIO_PIN(PORT, 0)  // DHT11 data pin
#define LED_PIN     GPIO_PIN(PORT, 2)  // LED pin

// Threshold temperature
#define TEMP_THRESHOLD 30

static int read_dht11(void) {
    // Simplified function to simulate reading DHT11
    // In reality, you need to implement the proper protocol to read data from the DHT11 sensor
    // Returning a mock temperature for demonstration
    return 25; // Mock temperature data
}

void *sensor_thread(void *arg) {
    (void)arg;  // Ignore unused parameter
    
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT11_PIN, GPIO_IN);

    while (1) {
        int temperature = read_dht11();

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a thread for handling the sensor reading
    thread_create(sensor_thread_stack, sizeof(sensor_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sensor_thread, NULL, "sensor_thread");

    return 0;
}