#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"

#define GPIO_LED GPIO_PIN(0, 5)  // Example pin for LED
#define DHT_PIN GPIO_PIN(0, 7)   // Example pin for DHT
#define TEMPERATURE_THRESHOLD 30 // Threshold temperature

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;

    // Initialize the DHT sensor on the specified pin
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        printf("Error: Could not initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold
            if (temp / 10 >= TEMPERATURE_THRESHOLD) {
                printf("Warning: Temperature exceeded threshold!\n");
                gpio_set(GPIO_LED); // Turn on LED
            } else {
                gpio_clear(GPIO_LED); // Turn off LED
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(GPIO_LED, GPIO_OUT) < 0) {
        printf("Error: Could not initialize LED GPIO\n");
        return -1;
    }
    
    // Create a thread for the DHT sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    // Main thread should continue to run
    while (1) {
        thread_sleep();
    }

    return 0;
}