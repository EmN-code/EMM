
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(MQTTClient *client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;

    if (MQTTConnect(client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    if (MQTTPublish(client, MQTT_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
        return;
    }
    printf("Message published\n");
}

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[256], readbuf[256];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    mqtt_connect(&client);

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
