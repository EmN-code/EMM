#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "periph/rtc.h"

#define DHT_PIN GPIO_PIN(0, 2)

void *dht11_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &dht_params[DHT_PIN]) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        rtc_sleep(RTC_PRESCALER_1024, 5);
    }
    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                                      dht11_read_thread, NULL, "dht11_read");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create the DHT11 thread\n");
        return 1;
    }

    MQTTClient client;
    Network network;
    int rc = 0;

    NetworkInit(&network);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Reconnecting...\n");
            rtc_sleep(RTC_PRESCALER_1024, 1);
            continue;
        } else {
            printf("Connected to network successfully\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));
    // Further MQTT client setup and usage can go here

    return 0;
}