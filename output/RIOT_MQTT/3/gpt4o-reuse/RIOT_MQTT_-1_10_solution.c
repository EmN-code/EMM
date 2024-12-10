
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/tcp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "mqtt_esp32"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

#define USERNAME    "mqtt_user"
#define PASSWORD    "123456"

static sock_tcp_t sock;
static sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_tcp_ep_t remote;

void connect_to_broker(void)
{
    remote.family = AF_INET;
    remote.port = PORT;
    inet_pton(AF_INET, ADDRESS, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        printf("Failed to connect to MQTT broker\n");
        exit(EXIT_FAILURE);
    }
}

void publish_message(void)
{
    char message[64];
    snprintf(message, sizeof(message), "CONNECT %s\n", CLIENTID);
    sock_tcp_write(&sock, message, strlen(message));

    snprintf(message, sizeof(message), "PUBLISH %s %s\n", TOPIC, PAYLOAD);
    sock_tcp_write(&sock, message, strlen(message));
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_message();
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
