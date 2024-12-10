#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "kernel_types.h"
#include "net/emcute.h"
#include "msg.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U) // 5 seconds
#define TOPIC "device/state"

static sock_tcp_t sock;
static msg_t queue[8];

void setup_mqtt_client(void) {
    sock_tcp_ep_t remote;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    
    sock_tcp_connect(&sock, &remote);
}

void publish_state(const char *state) {
    char message[64];
    snprintf(message, sizeof(message), "{\"state\": \"%s\"}", state);
    sock_tcp_send(&sock, message, strlen(message), SOCK_NO_FLAGS);
    printf("Published state: %s\n", state);
}

int main(void) {
    msg_init_queue(queue, 8);
    setup_mqtt_client();

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}