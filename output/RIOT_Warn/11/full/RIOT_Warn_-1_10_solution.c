#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <time_units.h>
#include <unstd.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <event/thread.h>
#include <mutex.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <periph/rtc.h>
#include <kernal_defines.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/gcoap.h>

// Constants for GPIO pins
#define DHT_PIN 2
#define LED_PIN 4
#define TEMP_THRESHOLD 30  // temperature threshold for the warning LED

// Function to initialize GPIO
void initialize_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

// Function to turn on/off LED
void led_control(bool state) {
    if (state) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

// Main function
int main(void) {
    printf("Starting DHT11 sensor and LED controller\n");

    // Initialize GPIO
    initialize_gpio();

    // DHT11 Device Structure
    dht_t dev;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return -1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("DHT read error\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp / 10, hum / 10);

            // Control LED based on temperature threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                led_control(true);
            } else {
                led_control(false);
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}