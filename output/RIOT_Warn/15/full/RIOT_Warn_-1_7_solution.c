// This code demonstrates reading from a DHT11 sensor and controlling an LED based on the temperature
// using RIOT for an ESP32. It implements error checking and reconnect logic for interactions with
// the sensor and handles LED control via GPIO.

#include "thread.h"
#include "string.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define DHT_PIN 4                // GPIO pin for DHT11
#define LED_PIN 2                // GPIO pin for LED
#define TEMP_THRESHOLD 30        // Temperature threshold for LED warning
#define SLEEP_INTERVAL_SEC 5     // Time interval between readings

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;

    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
            printf("DHT init failed\n");
            continue;
        }

        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("DHT read failed\n");
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
                               THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating DHT thread\n");
        return 1;
    }

    return 0;
}
