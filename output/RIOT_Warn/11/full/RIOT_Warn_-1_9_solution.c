#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "string.h"
#include "sys/socket.h"
#include "timex.h"
#include "thread.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "irq.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "event/thread.h"

#define DHT11_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_2
#define TEMP_THRESHOLD 30

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // DHT11 sensor for temperature and humidity
    dht_t dev;
    if (dht_init(&dev, DHT11_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        // Read DHT11 sensor data
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from sensor\n");
        } else {
            // Print temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            // If temperature exceeds threshold, turn on LED
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        ztimer_sleep(ztimer_now(), 5000);
    }

    return 0;
}
