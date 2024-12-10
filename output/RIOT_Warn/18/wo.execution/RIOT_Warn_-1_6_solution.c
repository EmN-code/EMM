#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "periph/rtc.h"
#include "thread.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "periph/gpio.h"

#define TEMP_THRESHOLD 30  // Temperature threshold
#define DHT_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN + 1)

// Global variables
static kernel_pid_t dht_pid;
static char dht_stack[DHT_STACKSIZE];
static msg_t dht_msg_queue[8];

dht_t dev;

void *read_dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);

            // Check temperature threshold
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN(0));  // Turn on LED
            } else {
                // Code to turn off LED
            }
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5000 ms
    }

    return NULL;
}

int main(void) {
    dht_params_t params = {
        .pin = GPIO_PIN(0),
        .type = DHT11,
    };

    // Initialize DHT11
    dht_init(&dev, &params);

    // Create a thread to read DHT11
    dht_pid = thread_create(dht_stack, sizeof(dht_stack), PRIORITY,
                            0, read_dht_thread, NULL, "dht");

    return 0;
}