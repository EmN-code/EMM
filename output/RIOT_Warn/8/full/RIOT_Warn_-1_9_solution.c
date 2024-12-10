#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <sys/socket.h>
#include <time_units.h>
#include <msg.h>
#include <xtimer.h>
#include <kernel_defines.h>
#include <dht_t.h>
#include <dht_read.h>
#include <dht_params.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <gpio_clear.h>
#include <gpio_set.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <net/emcute.h>

#define LED_PIN GPIO_PIN(0, 0) // Update with actual LED pin
#define DHT_PIN GPIO_PIN(0, 4) // Update with actual DHT11 data pin
#define TEMP_THRESHOLD 30 // Temperature threshold

void read_dht_and_control_led(void) {
    dht_t dev;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("DHT read failed\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize GPIO for DHT and LED
    if (gpio_init(LED_PIN, GPIO_OUT) == -1) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    if (gpio_init(DHT_PIN, GPIO_IN) == -1) {
        printf("Failed to initialize DHT GPIO\n");
        return 1;
    }

    // Start the DHT reading and LED controlling thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, 0, read_dht_and_control_led, NULL, "dht_thread");

    return 0;
}