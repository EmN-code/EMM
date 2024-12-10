
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error connecting to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

void publish_state(void) {
    char message[MQTT_MESSAGE_LEN + 1];
    strncpy(message, MQTT_MESSAGE, MQTT_MESSAGE_LEN);
    message[MQTT_MESSAGE_LEN] = '\0';

    int res = sock_tcp_write(&sock, message, MQTT_MESSAGE_LEN);
    if (res < 0) {
        puts("Error publishing state");
        return;
    }
    puts("State published");
}

int main(void) {
    sock_tcp_create(&sock, NULL, NULL, SOCK_FLAGS_REUSE_EP);
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    sock_tcp_close(&sock);
    return 0;
}
