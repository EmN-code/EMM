
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_KEEPALIVE 30
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static void mqtt_publish(void) {
    static uint8_t buf[128];
    static uint8_t readbuf[128];
    static uint8_t sendbuf[128];
    static struct Network network;
    static MQTTClient client;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int result = inet_pton(AF_INET, MQTT_BROKER_IP, &network.sock.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return;
    }

    network.sock.port = MQTT_BROKER_PORT;

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    if (MQTTPublish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    xtimer_sleep(5);
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
    return 0;
}
