#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "periph/rtc.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "unistd.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define DHT11_PIN  GPIO_PIN(GPIO_PORT_A, 0) // Example pin, adjust as needed
#define LED_PIN    GPIO_PIN(GPIO_PORT_A, 1) // Example pin, adjust as needed

#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

void *read_dht_thread(void *arg) {
    (void)arg;

    dht_params_t dht_params;
    dht_params.pin = DHT11_PIN;
    dht_params.type = DHT11;

    dht_t dht_dev;
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature threshold exceeded!\n");
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht_thread, NULL, "read_dht_thread");

    return 0;
}