#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h" // For additional hardware features if needed
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"

#define GPIO_PIN_LED    2     // Example GPIO pin for LED
#define DHT_PIN         4     // GPIO pin for DHT11
#define TEMP_THRESHOLD  30    // Temperature threshold for LED warning

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;
    dev.params.pin = DHT_PIN; 

    while (1) {
        while (dht_read(&dev, &temperature, &humidity) != 0) {
            // If reading fails, retry after 1 second
            printf("Failed to read from DHT sensor! Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        // Log the readings
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        // If temperature exceeds the threshold, turn on the LED
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(GPIO_PIN_LED);
        } else {
            gpio_clear(GPIO_PIN_LED);
        }
        
        // Wait for 5 seconds before next reading
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    // Initialize DHT sensor (this is a mock function as dht_init is not allowed)
    // Assuming proper initialization here

    // Initialize LED GPIO pin
    gpio_init(GPIO_PIN_LED, GPIO_OUT);

    // Create a thread for reading the DHT11 sensor
    thread_t dht_thread;

    thread_create(dht_thread, sizeof(dht_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  dht_read_thread, NULL, "dht_thread");

    // Main loop does nothing, as the work is done in the thread
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}