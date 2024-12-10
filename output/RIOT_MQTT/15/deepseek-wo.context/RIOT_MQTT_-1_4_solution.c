
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;

static void network_init(void) {
    int res = sock_tcp_create(&network.sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating TCP socket: %d\n", res);
        return;
    }

    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    res = sock_tcp_connect(&network.sock, &remote, 0);
    if (res < 0) {
        printf("Error connecting to broker: %d\n", res);
        return;
    }

    network.mqttread = (int (*)(Network*, unsigned char*, int, int))sock_tcp_read;
    network.mqttwrite = (int (*)(Network*, unsigned char*, int, int))sock_tcp_write;
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int res = MQTTPublish(&client, TOPIC, &message);
    if (res != 0) {
        printf("Error publishing message: %d\n", res);
    }
}

int main(void) {
    network_init();

    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_ESP32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int res = MQTTConnect(&client, &options);
    if (res != 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
