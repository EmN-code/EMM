
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <mutex.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL_SEC 5

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void publish_state(void) {
    char message[] = MQTT_MESSAGE;
    ssize_t res;

    res = sock_tcp_send(&sock, message, sizeof(message), &remote);
    if (res < 0) {
        puts("Failed to send message");
    } else {
        printf("Message sent: %s\n", message);
    }
}

int main(void) {
    int res;

    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = htons(MQTT_BROKER_PORT);
    res = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Invalid IP address");
        return 1;
    }

    /* Create a TCP socket */
    res = sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Failed to create socket");
        return 1;
    }

    /* Main loop to publish state every 5 seconds */
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_SEC * 1000);
    }

    /* Close the socket */
    sock_tcp_close(&sock);

    return 0;
}
