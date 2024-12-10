#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <iostream>
#include <arpa/inet.h>
#include <architecture.h>
#include <assert.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <ar.h>
#include <atomic>
#include <architecture.h>
#include <atomic>
#include <architecture.h>
#include <atomic>
#include <architecture.h>
#include <arpa/inet.h>
#include <atomic>
#include <atomic>
#include <atomic>
#include <atomic>
#include <architecture.h>
#include <atomic>
#include <atomic>
#include <atomic>
#include <atomic>
#include <arpa/inet.h>
#include <atomic>
#include <atomic>
#include <architecture.h>
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evevent/event_thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/system.h"
#include "freertos/timers.h"
#include "kernel_defines.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/netif.h"
#include "lwip/err.h"
#include "net/emcute.h"
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "paho_mqtt.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"

#define LED_PIN     GPIO_NUM_2
#define DHT_PIN     GPIO_NUM_4
#define TEMP_THRESHOLD 30

dht_t dev;

gpio_t led_pin = LED_PIN;
dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE_DHT11
};

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d *C, Humidity: %d %%\n", temp, hum);
        // Check temperature threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(led_pin);
            printf("Warning: Temperature threshold exceeded!\n");
        } else {
            gpio_clear(led_pin);
        }
    } else {
        printf("Error reading from DHT sensor\n");
    }
}

void *dht_thread(void *arg) {
    (void) arg;
    while (1) {
        read_dht();
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 reading and LED control\n");

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED GPIO
    gpio_init(led_pin, GPIO_OUT);

    // Create thread to read DHT and control LED
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t dht_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                                dht_thread, NULL, "dht_thread");

    if (dht_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating DHT thread\n");
        return 1;
    }

    return 0;
}