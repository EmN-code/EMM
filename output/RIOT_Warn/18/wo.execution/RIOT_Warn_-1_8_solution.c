// This code will interface RIOT OS with an ESP32 to read the DHT11 sensor
// and actuate an LED based on a temperature threshold using the Periph/GPIO 
// and threading system in RIOT OS.

#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "dht.h"

#define TEMP_THRESHOLD 30  // Define the temperature threshold for the warning.
#define DHT_PIN GPIO_PIN(0, 5)  // Assumed DHT sensor connected to pin GPIO0/5
#define LED_PIN GPIO_PIN(0, 6)  // Assumed LED connected on pin GPIO0/6

void *read_dht_thread(void *arg) {
    (void) arg;
    dht_t sensor_dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&sensor_dev, &params) != 0) {
        puts("Failed to initialize DHT sensor");
        return NULL;
    }

    while (1) {
        // Read from the DHT sensor
        int result = dht_read(&sensor_dev, &temperature, &humidity);
        if (result == 0) {
            printf("Temperature: %d.%d C
Humidity: %d.%d %%\n",
                 temperature / 10, temperature % 10,
                 humidity / 10, humidity % 10);

            // Check if temperature exceeds defined threshold
            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Error reading DHT sensor");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    puts("DHT Sensor and Warning LED example\n");

    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create thread to read DHT sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht_thread, NULL, "dht_thread");
    return 0;
}