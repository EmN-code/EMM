#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"
#include "architecture.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "unistd.h"
#include "mbox.h"
#include "thread.h"
#include "event/thread.h"
#include "timex.h"
#include "mutex.h"
#include "arpa/inet.h"

#define DHT11_PIN GPIO_PIN(0,2)
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

void *dht_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    (void)arg;

    if (dht_init(&dev, DHT11_PIN, DHT11) != 0) {
        printf("DHT init failed\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    char dht_thread_stack[THREAD_STACKSIZE_MAIN];

    thread_create(dht_thread_stack, sizeof(dht_thread_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  dht_thread, NULL, "dht_reader");
    
    return 0;
}
