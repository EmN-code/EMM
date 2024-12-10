#include "ztimer.h"
#include "stdbool.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "paho_mqtt.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    int res;

    /* Make sure to initialize the DHT_SENSOR */
    if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        res = dht_read(&dev, &temp, &hum);
        if (res == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create dht_thread\n");
        return 1;
    }

    /* Setup network connection with retry mechanism */
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }
    return 0;
}