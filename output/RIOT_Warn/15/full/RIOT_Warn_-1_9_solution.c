#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define DHT_PIN 5           // DHT sensor connected to GPIO 5
#define LED_PIN 2           // LED connected to GPIO 2
#define TEMP_THRESHOLD 30   // Temperature threshold
#define BROKER_IP "192.168.1.100"
#define BROKER_PORT 1883

void *dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != ESP_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    // Initialize LED as output
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error initializing GPIO\n");
        return NULL;
    }

    while (1) {
        // Read from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check temperature threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  dht_read_thread, NULL, "dht_reader");

    // MQTT network connection setup
    Network network;
    MQTTClient client;
    int rc;

    // Loop to ensure network connectivity
    while (1) {
        rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
        if (rc != 0) {
            printf("Failed to connect to MQTT broker, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000); // Retry every 1 second
        } else {
            printf("Connected to MQTT broker\n");
            break;
        }
    }

    // Additional MQTT setup and loop
    return 0;
}
