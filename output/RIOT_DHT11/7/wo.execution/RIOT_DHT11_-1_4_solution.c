#include <thread_arch.h>
#include <thread.h>
#include <paho_mqtt.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>
#include <architecture.h>
#include <MQTTClient.h>
#include <event/thread.h>

#include "periph/gpio.h"
#include "ztimer.h"

#define DHT11_PIN GPIO_PIN(PORT_C, 2)
#define SLEEP_DURATION_SEC 5

void *sensor_thread(void *arg) {
    (void)arg;
    int16_t temp, hum; // DHT11 temperature and humidity

    // Initialize the pin
    if (gpio_init(DHT11_PIN, GPIO_IN) < 0) {
        puts("Error initializing GPIO");
        return NULL;
    }

    while (1) {
        // Read DHT11 sensor
        if (dht_read(DHT11_PIN, &temp, &hum, 0) != DHT_OK) {
            puts("Error reading DHT11");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for defined duration
        ztimer_sleep(ZTIMER_SEC, SLEEP_DURATION_SEC);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  sensor_thread, NULL, "sensor_thread");
    return 0;
}
