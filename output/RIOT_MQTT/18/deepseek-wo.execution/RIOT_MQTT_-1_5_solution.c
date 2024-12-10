
#include <stdlib.h>
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1
#define MQTT_CMD_TIMEOUT 30000
#define MQTT_SEND_BUF_SIZE 100
#define MQTT_READ_BUF_SIZE 100
#define MQTT_STACK_SIZE 1024
#define MQTT_THREAD_PRIORITY THREAD_PRIORITY_MAIN - 1

static void mqtt_thread(void *arg)
{
    (void)arg;

    Network network;
    MQTTClient client;
    unsigned char sendbuf[MQTT_SEND_BUF_SIZE], readbuf[MQTT_READ_BUF_SIZE];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    int rc;

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEANSESSION;

    MQTTClientInit(&client, &network, MQTT_CMD_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    ipv4_addr_t broker_addr;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr) <= 0) {
        puts("Invalid IP address");
        return;
    }

    if (NetworkConnect(&network, (char *)&broker_addr, MQTT_BROKER_PORT) != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }

    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }
}

int main(void)
{
    thread_create(NULL, MQTT_STACK_SIZE, MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
