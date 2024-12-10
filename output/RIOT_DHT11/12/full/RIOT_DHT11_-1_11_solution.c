#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "architecture.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "unistd.h"
#include "dht.h"
#include "dht_params.h"

#define DHT_PIN             2      // GPIO2 for the DHT11 sensor
#define DHT_TYPE            DHT11_TYPE
#define MQTT_BROKER_IP      "192.168.1.10"
#define MQTT_BROKER_PORT    1883
#define SLEEP_INTERVAL      (5U * 1000000U) // 5 seconds in microseconds

// Structure to hold the DHT sensor details
static dht_t dht_device;

int main(void) {
    // Initialize the GPIO pin for the DHT sensor
    if (gpio_init(DHT_PIN, GPIO_IN) != 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    // Initialize the DHT sensor
    if (dht_init(&dht_device, DHT_PIN, DHT_TYPE) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    int16_t temperature, humidity;

    // Enter main loop
    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dht_device, &temperature, &humidity) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds before the next reading
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}