#include "esp_system.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "net/emcute.h"
#include "periph/rtc.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "timex.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdio.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "time_units.h"

#include <stdio.h>
#include <stdlib.h>

#define DHT_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 25

void read_dht(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(NULL, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Create a thread to read the DHT sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, read_dht, NULL, "read_dht");
    
    // Main thread can perform other tasks or sleep indefinitely
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 10000);
    }

    return 0;
}