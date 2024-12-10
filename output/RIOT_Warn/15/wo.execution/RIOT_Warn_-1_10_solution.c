#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <time_units.h>

#define DHT_PIN GPIO_NUM_4  // Correct the GPIO number to be used.
#define LED_PIN GPIO_NUM_2
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius.

void setup_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);
}

int main(void) {
    setup_gpio();

    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    // Initialize the DHT sensor.
    dht_init(&dev, &params);

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED if temperature is above threshold
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED otherwise
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);  // Delay for 5 seconds.
    }

    return 0;
}