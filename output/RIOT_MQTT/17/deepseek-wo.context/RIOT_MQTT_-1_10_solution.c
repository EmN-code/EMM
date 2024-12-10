
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static void *mqtt_thread(void *arg)
{
    (void)arg;

    Network n;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkConnect(&n, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &n, 30000, NULL, 0, NULL, 0);

    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
