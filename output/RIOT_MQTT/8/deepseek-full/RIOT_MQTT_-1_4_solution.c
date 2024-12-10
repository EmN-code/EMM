
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <mutex.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void connect_to_broker(void)
{
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

static void publish_state(void)
{
    char payload[] = PAYLOAD;
    if (sock_udp_send(&sock, payload, sizeof(payload), &remote) < 0) {
        puts("Error sending data");
    } else {
        puts("State published");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
