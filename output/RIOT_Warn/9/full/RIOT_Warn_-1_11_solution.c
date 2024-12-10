#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#include "xtimer.h"  // RIOT timer functions
#include "msg.h"     // For using RIOT's messaging system
#include "thread.h"  // For threading in RIOT
#include "mutex.h"   // For mutex locking
#include "timex.h"   // Timer extensions for RIOT
#include "mqtt.h"    // MQTT functions for RIOT
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "event/thread.h"

#include "gpio.h"    // For GPIO operations
#include "esp_system.h"
#include "esp_wifi.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Example pin, adjust according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 0)    // Example pin, adjust according to your setup
#define TEMP_THRESHOLD 30              // Temperature threshold for warning (in Celsius)

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    dev.pin = DHT11_PIN;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            // Ideally, add some error handling here
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t dht_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_thread, NULL, "dht_thread");

    if (dht_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create DHT thread\n");
        return 1;
    }

    return 0;
}