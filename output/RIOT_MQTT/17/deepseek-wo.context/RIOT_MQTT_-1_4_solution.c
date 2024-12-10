
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void mqtt_publish_thread(void *arg)
{
    (void)arg;

    struct Network network;
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        MQTTPublish(&client, MQTT_TOPIC, &message);

        xtimer_sleep(5);
    }
}

int main(void)
{
    thread_t *mqtt_thread = thread_create(
        (char *)malloc(MQTT_THREAD_STACKSIZE),
        MQTT_THREAD_STACKSIZE,
        MQTT_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        mqtt_publish_thread,
        NULL,
        "mqtt_publish_thread"
    );

    if (mqtt_thread == NULL) {
        printf("Failed to create MQTT thread\n");
        return 1;
    }

    return 0;
}
