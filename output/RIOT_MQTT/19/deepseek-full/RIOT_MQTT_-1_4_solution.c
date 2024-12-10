
#include <evtimer_msg.h>
#include <stdio.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <dht_params.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * US_PER_SEC)

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void publish_state(void)
{
    char payload[PAYLOAD_LEN + 1];
    memcpy(payload, PAYLOAD, PAYLOAD_LEN);
    payload[PAYLOAD_LEN] = '\0';

    if (sock_udp_send(&sock, payload, PAYLOAD_LEN, &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("Message sent");
    }
}

static void timer_callback(void *arg)
{
    (void)arg;
    publish_state();
    ztimer_set(ZTIMER_USEC, (ztimer_t *)arg, INTERVAL);
}

int main(void)
{
    local.port = 0;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating socket");
        return 1;
    }

    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL);
    timer_callback(&timer);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    sock_udp_close(&sock);
    return 0;
}
