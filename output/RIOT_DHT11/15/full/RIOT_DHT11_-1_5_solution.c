#include "thread.h"
#include "mqtt.h" // This includes paho_mqtt.h indirectly
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"
#include "dht_params.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define MQTT_BROKER_IP "192.168.1.10"
#define MQTT_BROKER_PORT 1883

#define SLEEP_TIME 5

static msg_t main_msg_queue[8];

void *mqtt_thread(void *arg) {
    (void)arg;
    static MQTTClient client;
    static Network network;
    static unsigned char sendbuf[80], readbuf[80];
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        int rc;
        if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
            printf("Network connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("Network connected\n");
        break;
    }

    while(1) {
        // Placeholder for MQTT publish code
        printf("Publishing data to MQTT broker\n");
        xtimer_sleep(SLEEP_TIME);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, 8);
    dht_t dev;

    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("DHT sensor initialization failed\n");
        return 1;
    }

    printf("DHT sensor initialized\n");

    // Thread setup
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN + 1, 0, mqtt_thread, NULL, "mqtt");

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}