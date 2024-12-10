#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/mqttsn.h"
#include "xtimer.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     1883
#define MQTT_CLIENT_ID  "riot_esp32"
#define MQTT_TOPIC      "device/state"
#define MQTT_MESSAGE    "work"
#define DELAY           (5U * US_PER_SEC)

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

void publish_state(void)
{
    char *topic = MQTT_TOPIC;
    char *message = MQTT_MESSAGE;
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &gw, 0);

    sock_udp_ep_t remote;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    sock_udp_send(&sock, message, strlen(message), &remote);
    printf("Sent message: %s\n", message);

    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        publish_state();
        xtimer_sleep(DELAY);
    }
    return 0;
}