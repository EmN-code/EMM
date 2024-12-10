#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "timex.h"
#include "architecture.h"
#include "xtimer.h"
#include "unistd.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "thread_flags.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_system.h"
#include "netinet/in.h"

#define GPIO_PIN 2
#define SLEEP_INTERVAL (5LU * US_PER_SEC)

void read_dht_sensor(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int result;

    // Initialize the DHT sensor
    if (dht_init(&dev, GPIO_PIN, DHT11) == 0) {
        while (1) {
            result = dht_read(&dev, &temperature, &humidity);
            if (result == 0) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                       temperature / 10, temperature % 10, 
                       humidity / 10, humidity % 10);
            } else {
                printf("Failed to read from DHT sensor\n");
            }
            ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
        }
    } else {
        printf("Failed to initialize DHT sensor\n");
    }
}

int main(void) {
    // Initialize the NVS flash
    esp_err_t ret_flash = nvs_flash_init();
    if (ret_flash != ESP_OK) {
        printf("Failed to initialize NVS flash. Error: %d\n", ret_flash);
        return -1;
    }

    // Read the DHT sensor
    read_dht_sensor();

    return 0;
}
