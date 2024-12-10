
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/netopt.h"
#include "net/netif.h"
#include "net/netif/hdr.h"
#include "net/netif/ethernet.h"
#include "net/netif/ethernet_hdr.h"
#include "net/netif/ethernet_params.h"
#include "net/netif/ethernet_types.h"
#include "net/netif/ethernet_common.h"
#include "net/netif/ethernet_socket.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h"
#include "net/netif/ethernet_socket_internal.h"
#include "net/netif/ethernet_socket_params.h"
#include "net/netif/ethernet_socket_types.h"
#include "net/netif/ethernet_socket_common.h

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_mqtt_client"
#define TOPIC "state"
#define BROKER_ADDR "47.102.103.1"

static emcute_topic_t pub_topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));
    sub.cb = NULL;
    sub.topic = &pub_topic;

    if (emcute_con(&sub, EMCUTE_ID, true, &sock) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        char payload[20];
        snprintf(payload, sizeof(payload), "work");
        emcute_pub(&sub, &pub_topic, payload, strlen(payload), 0);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example");

    if (inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error parsing broker address");
        return 1;
    }

    pub_topic.name = TOPIC;
    pub_topic.id = 0;

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute_thread");

    return 0;
}
