
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "riot_esp32"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEP_ALIVE 20

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, BROKER_ADDR);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    Network network;
    network.socket = sock_fd(&sock);

    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    return 0;
}
