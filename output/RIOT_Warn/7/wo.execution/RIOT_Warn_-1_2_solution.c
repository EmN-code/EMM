#include <periph/gpio.h>
#include <event/thread.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 2) // Assuming the DHT11 pin is GPIO 2 on PORT A
#define LED_PIN GPIO_PIN(PORT_B, 3)  // Assuming the LED pin is GPIO 3 on PORT B
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define DHT_TYPE 11 // DHT11

void *read_dht11(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    // Initialize DHT11
    dht_init(&dev, DHT11_PIN, DHT_TYPE);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Start thread to read DHT11 values
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "read_dht11");

    // Main thread can perform other tasks or stay idle
    while (1) {
        pause(); // Keep the main thread alive
    }
    return 0;
}