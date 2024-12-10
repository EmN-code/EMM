#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#include "dht.h"
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Assuming DHT11 is connected to GPIO 4
#define LED_PIN GPIO_PIN(0, 2)  // Assuming LED is connected to GPIO 2
#define TEMP_THRESHOLD (30)    // Temperature threshold for turning the LED on
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // Sleep interval of 5 seconds

// Initialization for DHT sensor
dht_t dht_dev;

// Thread stack size
char stack[THREAD_STACKSIZE_MAIN];

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", (float)temp/10, (float)hum/10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT sensor\n");
        }        
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize DHT sensor
    if (dht_init(&dht_dev, DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    // Start the thread that reads the sensor and controls the LED
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, read_dht_and_control_led, NULL, "dht_reader");
    return 0;
}