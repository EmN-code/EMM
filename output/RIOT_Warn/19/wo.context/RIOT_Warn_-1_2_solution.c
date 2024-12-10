#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread.h"
#include "msg.h"
#include "periph/gpio.h"
#include "timex.h"
#include "ztimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  /* Define the correct GPIO for DHT11 */
#define LED_PIN GPIO_PIN(0, 5)  /* Define the correct GPIO for LED */
#define TEMP_THRESHOLD 30        /* Temperature threshold in Celsius */
#define INTERVAL (5U * US_PER_SEC) /* 5 seconds interval */

static dht_t dev;

void read_sensor_and_control_led(void) {
    int16_t temp, hum;
    int res = dht_read(&dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("LED ON\n");
        } else {
            gpio_clear(LED_PIN);
            printf("LED OFF\n");
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    puts("DHT11 reader with LED warning application starting...");
    
    /* Initialize DHT sensor */
    if (dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    /* Initialize LED pin */
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("LED GPIO init failed");
        return 1;
    }

    while (1) {
        read_sensor_and_control_led();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}