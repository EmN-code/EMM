#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <inttypes.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>
#include <timex.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <event/thread.h>
#include <netinet/in.h>
#include <mbox.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30
#define DHT11_INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;

void *dht_reader_thread(void *arg) {
    int16_t temp, hum;
    (void)arg;

    while(1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > (TEMP_THRESHOLD * 10)) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11\n");
        }
        ztimer_sleep(ZTIMER_MSEC, DHT11_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    if (dht_init(&dht_dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN-1,
                  0, dht_reader_thread, NULL, "dht_reader");

    return 0;
}
