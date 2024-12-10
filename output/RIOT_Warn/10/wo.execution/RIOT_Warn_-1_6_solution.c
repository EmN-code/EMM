#include "inttypes.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "periph/rtc.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "timex.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"
#include "xtimer.h"
#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "mbox.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define THRESHOLD_TEMPERATURE 30 // Temperature threshold in Celsius
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static dht_t dev;

void *temperature_monitor(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature / 10 > THRESHOLD_TEMPERATURE) {
                puts("Temperature exceeds threshold! Turning on LED.");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg, ARRAY_SIZE(msg));

    if (dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                      temperature_monitor, NULL, "temperature_monitor") <= KERNEL_PID_UNDEF) {
        puts("Failed to create temperature monitoring thread");
        return 1;
    }

    return 0;
}