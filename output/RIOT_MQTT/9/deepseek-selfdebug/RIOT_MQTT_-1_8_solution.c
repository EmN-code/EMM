
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

void publish_state(void) {
    char message[64];
    snprintf(message, sizeof(message), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);
    ssize_t len = sock_tcp_write(&sock, message, strlen(message));
    if (len < 0) {
        puts("Error: could not send message to MQTT broker");
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    puts("Starting MQTT client");

    sock_tcp_create(&sock, NULL, NULL, SOCK_FLAGS_REUSE_EP);
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
