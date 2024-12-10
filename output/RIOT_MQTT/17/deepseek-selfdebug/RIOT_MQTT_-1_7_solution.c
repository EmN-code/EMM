
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;

void publish_state(void) {
    char message[64];
    snprintf(message, sizeof(message), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    if (sock_tcp_send(&sock, message, strlen(message)) < 0) {
        puts("Failed to send message");
    } else {
        puts("Message sent successfully");
    }
}

int main(void) {
    sock_ip_ep_t remote;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid IP address");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    sock_tcp_close(&sock);
    return 0;
}
