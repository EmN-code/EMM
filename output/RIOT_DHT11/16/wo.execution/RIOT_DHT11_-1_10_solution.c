#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

// Define the DHT type and the GPIO pin
#define DHT_TYPE DHT11
#define DHT_PIN GPIO2

// Function to read sensor data
void *sensor_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return NULL;
    }

    while (1) {
        // Read the sensor data
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        else {
            printf("Failed to read sensor data\n");
        }

        // Wait for 5 seconds before the next read
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Create a thread for reading sensor data
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sensor_read_thread, NULL, "sensor_read");

    // The main thread just idles
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}