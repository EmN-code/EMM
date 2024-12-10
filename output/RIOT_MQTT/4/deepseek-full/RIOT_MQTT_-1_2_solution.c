
#include "paho_mqtt.h"
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static MQTTClient client;

void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    int rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

void mqtt_publish(const char *payload) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str((uint8_t *)&remote.addr.ipv4, BROKER_IP);

    sock_udp_t sock;
    int res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    Network network;
    NetworkInit(&network);
    network.socket = sock_fd(&sock);

    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish(PAYLOAD);
        xtimer_sleep(5);
    }

    return 0;
}
