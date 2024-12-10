#include "ztimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "stdint.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "esp_system.h"
#include "timex.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "periph/i2c.h"
#include "mutex.h"
#include "net/emcute.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define THRESHOLD_TEMP 300 // Example threshold for temperature
#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for DHT sensor
#define LED_PIN GPIO_PIN(PORT_B, 1) // Define the GPIO pin for the LED

void read_dht_and_control_led(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Assuming dht_init initializes the DHT device
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("DHT initialization failed\n");
        return;
    }
    
    while (1) {
        
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > THRESHOLD_TEMP) {
                gpio_set(LED_PIN); // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, read_dht_and_control_led, NULL, "dht_thread");
    
    while (1) {
        // Main thread loop
    }
    return 0;
}