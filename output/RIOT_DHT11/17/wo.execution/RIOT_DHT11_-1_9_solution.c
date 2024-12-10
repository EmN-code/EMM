#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "event/timeout.h"
#include "event/periodic.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"

// DHT configuration
#define DHT_PIN GPIO2
#define INTERVAL_SEC (5U)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Failed to initialize DHT\n");
        return 1;
    }

    // Main loop
    while (1) {
        // Read values from DHT
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(INTERVAL_SEC);
    }
    return 0;
}