#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(0, 0)
#define LED_PIN GPIO_PIN(0, 1)
#define TEMP_THRESHOLD 30  // Temperature threshold for warning LED

void *sensor_thread(void *arg) {
    (void) arg;
    int16_t temperature, humidity;
    dht_t dev;
    // Simulate a DHT device initialization.
    // Initialize the LED pin.
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialization of the sensor
    if (dht_init(&dev, DHT_11, DHT_PIN) != 0) {
        printf("Failed to init DHT device\n");
        return NULL;
    }

    while(1) {
        // Attempt to read the temperature and humidity from the DHT sensor.
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", 
                    temperature / 10, temperature % 10, 
                    humidity / 10, humidity % 10);

            // Turn on/off the LED based on the temperature threshold.
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        
        // Sleep for 5 seconds
        ztimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, sensor_thread, NULL, "sensor");
    return 0;
}
