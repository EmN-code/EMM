
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void network_init(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    int res;

    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    res = sock_tcp_create(&network.sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Error creating TCP socket: %d\n", res);
        return;
    }

    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
}

void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_esp32";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int res = MQTTConnect(&client, &data);
    if (res != 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }
    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int res = MQTTPublish(&client, TOPIC, &message);
    if (res != 0) {
        printf("Error publishing message: %d\n", res);
        return;
    }
    printf("Published message: %s\n", PAYLOAD);
}

int main(void) {
    network_init();

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
