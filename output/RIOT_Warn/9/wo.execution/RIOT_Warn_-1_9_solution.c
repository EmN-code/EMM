#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <MQTTClient.h>
#include "esp_idf_gpio.h"

#define DHT_PIN GPIO_PIN(1, 5) // Example pin
#define LED_PIN GPIO_PIN(0, 2) // Example pin
#define TEMP_THRESHOLD 30 // Example threshold temperature in °C

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dev.pin = DHT_PIN;
    dev.type = DHT11;

    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    esp_idf_gpio_set_drive_capability(LED_PIN, GPIO_DRIVE_WEAK);

    // Main loop
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Could not read data from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check temperature against threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}