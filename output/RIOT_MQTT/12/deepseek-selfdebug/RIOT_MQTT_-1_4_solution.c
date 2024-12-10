
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define INTERVAL_MS (5 * 1000000U)

static sock_tcp_t sock;
static struct sockaddr_in remote;

static void connect_to_broker(void) {
    int res;

    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(MQTT_BROKER_PORT);

    res = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.sin_addr);
    if (res != 1) {
        puts("Error: Invalid IP address");
        return;
    }

    res = sock_tcp_connect(&sock, (struct sockaddr *)&remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error: Could not connect to broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_message(void) {
    char message[64];
    int res;

    snprintf(message, sizeof(message), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    res = sock_tcp_send(&sock, message, strlen(message), 0);
    if (res < 0) {
        puts("Error: Could not send message");
        return;
    }

    puts("Message published");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    connect_to_broker();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        publish_message();
    }

    return 0;
}
