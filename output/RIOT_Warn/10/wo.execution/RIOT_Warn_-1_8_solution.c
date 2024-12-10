#include <ztimer.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include "MQTTClient.h"
#include <arpa/inet.h>
#include <timex.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define DHT_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *read_sensor(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    dht_t dev;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return NULL;
    }

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Current temperature: %d.%d°C\n", temp / 10, temp % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5000 milliseconds (5 seconds)
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, read_sensor, NULL, "sensor_thread");
    return 0;
}
