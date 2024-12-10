#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL 5
#define MQTT_MESSAGE "work"

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    char addr_str[IPV6_ADDR_MAX_STR_LEN];

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Connect TCP socket to MQTT broker
    sock_tcp_connect(&sock, &remote);

    while (1) {
        // Publish message
        sock_tcp_send(&sock, MQTT_MESSAGE, strlen(MQTT_MESSAGE));
        
        // Sleep for the publish interval
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Close socket (unreachable but good practice)
    sock_tcp_disconnect(&sock);
    return 0;
}