#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "esp_system.h"

#define DHT_PIN 4         // Define the GPIO pin number for DHT11
#define LED_PIN 2         // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30 // Temperature threshold for warning

dht_t dev;

void read_temperature(void) {
    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) == 0) {
        printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on the LED
        } else {
            gpio_clear(LED_PIN); // Turn off the LED
        }
    } else {
        printf("Error reading DHT11 sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dev, gpio_pin(DHT_PIN), DHT11);
    
    while (1) {
        read_temperature();
        xtimer_sleep(5); // Wait for 5 seconds
    }
    return 0;
}