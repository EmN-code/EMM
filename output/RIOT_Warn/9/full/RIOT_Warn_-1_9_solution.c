#include "ztimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "msg.h"
#include "xtimer.h"
#include "dht.h"
#include "mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_Params.h"
#include "mma8x5x_Regs.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "MQTTClient.h"
#include "unistd.h"

#define DHT_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 30 // temperature threshold in degree Celsius

kernel_pid_t dht_thread_pid;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                // Turn on the LED if temperature exceeds the threshold
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED otherwise
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    // Create the DHT thread
    dht_thread_pid = thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1,
                                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    if (dht_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: failed to create thread\n");
        return 1;
    }

    return 0;
}