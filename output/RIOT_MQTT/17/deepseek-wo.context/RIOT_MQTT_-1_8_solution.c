
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static void mqtt_publish(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[100];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published successfully\n");
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
}

int main(void) {
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
    return 0;
}
