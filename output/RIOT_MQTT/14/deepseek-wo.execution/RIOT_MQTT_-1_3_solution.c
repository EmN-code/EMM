
#include <net/ipv6/addr.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <mbox.h>
#include <net/sock/udp.h>

#include "MQTTClient.h"  // Ensure this is the last header file included

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static void network_init(Network *network) {
    network->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (network->socket < 0) {
        puts("Failed to create socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(network->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Failed to connect to server");
        return;
    }

    network->read = sock_tcp_read;
    network->write = sock_tcp_write;
    network->disconnect = sock_tcp_disconnect;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    network_init(&network);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
