
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <event/thread.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <dht_params.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static Network network;
static MQTTClient client;

void network_connect(Network *network, const char *ip, int port) {
    network->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (network->sock < 0) {
        puts("Failed to create socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(network->sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Failed to connect to server");
        return;
    }
}

void network_disconnect(Network *network) {
    if (network->sock >= 0) {
        close(network->sock);
        network->sock = -1;
    }
}

int network_read(Network *network, unsigned char *buffer, int len, int timeout) {
    return recv(network->sock, buffer, len, 0);
}

int network_write(Network *network, unsigned char *buffer, int len, int timeout) {
    return send(network->sock, buffer, len, 0);
}

int main(void) {
    unsigned char sendbuf[256];
    unsigned char readbuf[256];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    network_connect(&network, SERVER_IP, SERVER_PORT);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        network_disconnect(&network);
        return 1;
    }

    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_sleep(5);
    }

    network_disconnect(&network);
    return 0;
}
