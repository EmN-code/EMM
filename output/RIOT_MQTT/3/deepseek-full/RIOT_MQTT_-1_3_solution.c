
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define QOS 0
#define KEEPALIVE 60

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void _publish_state(void)
{
    static char buf[128];
    int res;

    /* Prepare the message */
    sprintf(buf, "PUBLISH %s %s", TOPIC, PAYLOAD);

    /* Send the message */
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending message");
    } else {
        printf("Message sent: %s\n", buf);
    }
}

int main(void)
{
    int res;

    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Create the UDP socket */
    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Set up the periodic timer for publishing state */
    evtimer_msg_t evtimer;
    evtimer_msg_init_periodic(&evtimer, ztimer_now(ZTIMER_MSEC), timex_set(5, 0), _publish_state);

    /* Main loop */
    while (1) {
        msg_t msg;
        if (evtimer_msg_recv(&evtimer, &msg) > 0) {
            _publish_state();
        }
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}
