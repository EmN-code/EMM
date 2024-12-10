
#include <net/sock.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
static sock_udp_t sock;

static void _publish(void)
{
    uint8_t buf[PAYLOAD_LEN];
    memcpy(buf, PAYLOAD, PAYLOAD_LEN);

    sock_udp_ep_t remote = server;
    remote.port = SERVER_PORT;

    if (sock_udp_send(&sock, buf, sizeof(buf), &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("Message sent");
    }
}

int main(void)
{
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) <= 0) {
        puts("Error parsing server IP");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        _publish();
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
