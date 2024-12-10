#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_NUM_4 /* Example pin, adjust as necessary */
#define LED_PIN GPIO_NUM_2 /* Example pin, adjust as necessary */

static dht_t dht_dev;

void init_dht(void) {
    while (1) {
        if (dht_init(&dht_dev, &dht_params[0]) == ESP_OK) {
            printf("DHT initialized successfully\n");
            break;
        } else {
            printf("Failed to initialize DHT. Retrying in 1 second...\n");
            xtimer_sleep(1);
        }
    }
}

void read_temp_humidity(void) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == ESP_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT data.\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    printf("Starting DHT11 temperature monitoring\n");

    /* Initialize GPIO for LED */
    gpio_init(LED_PIN, GPIO_OUT);

    /* Initialize DHT sensor */
    init_dht();

    /* Start reading temperature and humidity */
    read_temp_humidity();

    return 0;
}