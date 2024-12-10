#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/gpio.h"
#include "thread.h"
#include "dht.h"

#define GPIO_PIN 2  // using GPIO2 for LED
#define DHT_PIN 4   // assuming DHT is connected to GPIO4
#define TEMP_THRESHOLD 30  // temperature threshold for the LED to turn on

static dht_t dev;

void *dht_read_task(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != ESP_OK) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN);
                printf("LED ON: Temperature exceeded threshold\n");
            } else {
                gpio_clear(GPIO_PIN);
                printf("LED OFF: Temperature within safe limits\n");
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // wait for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Configure LED pin
    if (gpio_init(GPIO_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }
    
    // Initialize DHT sensor
    dev.params.pin = DHT_PIN;
    dev.type = DHT_TYPE_DHT11;
    if (dht_init(&dev) != ESP_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Start thread to read DHT sensor
    static char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  0, dht_read_task, NULL, "dht_read");

    return 0;
}