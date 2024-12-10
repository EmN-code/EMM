
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/constants.h>
#include <net/sock/socket.h>
#include <net/sock/socket_base.h>
#include <net/sock/socket_base_internal.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <esp_wifi.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static void network_init(Network* n) {
    n->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (n->socket < 0) {
        puts("Failed to create socket");
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("Failed to connect to server");
        return;
    }

    n->read = read;
    n->write = write;
    n->disconnect = close;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    network_init(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}
