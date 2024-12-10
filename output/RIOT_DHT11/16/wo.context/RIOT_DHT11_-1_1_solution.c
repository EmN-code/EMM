#include <timers.h>
#include <stdio.h>
#include <dht.h>

#define DHT11_DATA_PIN 2
#define READ_INTERVAL_S 5

static dht_t dht11;

void read_dht11(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    if (dht_read(&dht11, &temperature, &humidity) == 0) {
        printf("Temp: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    } else {
        printf("Error reading from DHT11 sensor\n");
    }
}

void *sensor_thread(void *arg) {
    (void)arg;
    
    while (1) {
        read_dht11();
        xtimer_sleep(READ_INTERVAL_S);
    }

    return NULL;
}

int main(void) {
    /* Initialize the DHT11 sensor */
    dht_params_t params;
    params.pin = DHT11_DATA_PIN;
    params.type = DHT11;
    
    if (dht_init(&dht11, &params) != 0) {
        printf("DHT11 init failed\n");
        return 1;
    }

    /* Create a new thread for the sensor readings */
    thread_create(sensor_thread, NULL, KERNEL_THREAD_STACKSIZE_DEFAULT, "sensor_thread");

    /* Keep the main function alive */
    while (1) {
        xtimer_sleep(3600); /* sleep for a long time */
    }

    return 0;
}