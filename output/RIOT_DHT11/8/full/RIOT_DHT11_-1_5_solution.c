#include "mma8x5x_params.h"
#include "mqtt/MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin where DHT11 is connected

// Function for reading temperature and humidity
void read_dht(void) {
    
    dht_t dev;
    
    // Initialize the DHT device on the specified GPIO pin
    if (dht_init(&dev, DHT11, DHT_PIN) == 0) {
        printf("DHT11 initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11\n");
        return;
    }

    while (1) {
        int16_t temp, hum;
        int res = dht_read(&dev, &temp, &hum);
        if (res == 0) {
            // Successfully read the sensor data
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds before reading again
        xtimer_sleep(5);
    }
}

int main(void) {
    // Start the DHT reading thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)read_dht, NULL, "read_dht");

    // Main thread can perform other tasks or just sleep
    while (1) {
        thread_sleep();
    }
    return 0;
}