
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);
    message.qos = QOS0;
    message.retained = 0;

    MQTTPublish(client, MQTT_TOPIC, &message);
}

static void mqtt_thread(void *arg) {
    (void)arg;

    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }
}

int main(void) {
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
