#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "timex.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "msg.h"
#include "dht.h"
#include "dht_params.h"
#include "stdatomic.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "time_units.h"

#define DHT_PIN GPIO2

dht_t dev;

int main(void) {
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Simulate MQTT connection attempt
        int rc;
        while (1) {
            rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
            if (rc != 0) {
                printf("Network connection failed, retrying...\n");
                xtimer_sleep(1);
                continue;
            } else {
                printf("Connected to network successfully\n");
                break;
            }
        }

        xtimer_sleep(5);
    }
    return 0;
}