
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/tcp.h"
#include "net/sock/util.h"
#include "net/mqttsn.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "mqtt_esp32"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

static sock_tcp_t sock;
static sock_tcp_ep_t server = SOCK_IPV4_EP_ANY;

void connect_to_broker(void) {
    server.port = PORT;
    server.family = AF_INET;
    inet_pton(AF_INET, ADDRESS, &server.addr.ipv4);

    if (sock_tcp_connect(&sock, &server, SOCK_NO_TIMEOUT) < 0) {
        printf("Failed to connect to MQTT broker\n");
        exit(EXIT_FAILURE);
    }
}

void publish_message(void) {
    char buffer[100];
    int len = snprintf(buffer, sizeof(buffer), "PUBLISH %s %s\n", TOPIC, PAYLOAD);
    if (sock_tcp_send(&sock, buffer, len, 0) < 0) {
        printf("Failed to send message\n");
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_message();
        sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
