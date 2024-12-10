#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_err.h"
#include "illuminate.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"

#define TEMP_THRESHOLD 25
#define DHT_PIN 4
#define LED_PIN 2

void *dht_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != ESP_OK) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED if temperature drops below the threshold
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // Initializing GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    // Create a thread for reading DHT sensor
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht");

    // Main loop
    while (1) {
        // Sleep to allow other threads to execute
        thread_sleep();
    }

    return 0;
}