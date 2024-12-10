
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    char buf[128];
    int res;

    remote.family = AF_INET;
    remote.port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    res = sock_tcp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Error creating TCP socket: %d\n", res);
        return 1;
    }

    res = sock_tcp_connect(&sock, &remote, 0);
    if (res < 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        // Prepare MQTT message
        snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);
        res = sock_tcp_send(&sock, buf, strlen(buf), 0);
        if (res < 0) {
            printf("Error sending data: %d\n", res);
            break;
        }

        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
