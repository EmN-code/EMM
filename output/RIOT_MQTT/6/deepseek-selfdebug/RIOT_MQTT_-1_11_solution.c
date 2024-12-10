
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
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS (5 * 1000000U)

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void connect_to_broker(void) {
    int res;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Error creating TCP socket: %d\n", res);
        return;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

void publish_state(void) {
    char message[64];
    int res;

    snprintf(message, sizeof(message), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);
    res = sock_tcp_write(&sock, message, strlen(message));
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    connect_to_broker();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        publish_state();
    }

    sock_tcp_close(&sock);
    return 0;
}
