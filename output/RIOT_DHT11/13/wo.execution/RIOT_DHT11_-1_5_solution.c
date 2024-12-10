#include "thread.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DELAY_BETWEEN_READS (5U)
#define GPIO_PIN DHT_PARAM_PIN

static char stack[THREAD_STACKSIZE_MAIN];

void *read_sensor(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    /* Initialize DHT sensor */
    if (dht_init(&dev, GPIO_PIN) == 0) {
        puts("DHT sensor initialization successful.");

        while (1) {
            /* Read values */
            if (dht_read(&dev, &temp, &hum) == 0) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                puts("DHT reading error.");
            }
            
            /* Wait for next reading */
            xtimer_sleep(DELAY_BETWEEN_READS);
        }
    } else {
        puts("Failed to initialize DHT sensor.");
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL, "sensor_reader");

    /* start the main loop */
    while (1) {
        xtimer_sleep(DELAY_BETWEEN_READS);
    }

    return 0;
}