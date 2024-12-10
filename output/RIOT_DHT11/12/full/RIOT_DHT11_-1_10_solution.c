#include "ztimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN 2  // GPIO pin for DHT11
#define SLEEP_INTERVAL 5 // Sleep interval in seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int result;

    // Initialize GPIO and DHT sensor
    if (gpio_init(DHT_PIN, GPIO_IN) != 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Main loop to read DHT11 sensor every 5 seconds
    while (1) {
        result = dht_read(&dev, &temperature, &humidity);
        if (result == 0) {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature/10.0, humidity/10.0);
        } else {
            printf("Failed to read from sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * MS_PER_SEC);
    }
    return 0;
}