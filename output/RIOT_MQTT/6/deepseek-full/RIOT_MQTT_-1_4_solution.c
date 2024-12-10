
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>
#include <esp_system.h>
#include <dht_params.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void init_udp_socket(void)
{
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
    puts("UDP socket created");
}

static void publish_state(void)
{
    const char *payload = PAYLOAD;
    size_t payload_len = strlen(payload);

    if (sock_udp_send(&sock, payload, payload_len, &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("State published");
    }
}

int main(void)
{
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
