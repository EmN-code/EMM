
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/internal.h

#define TOPIC_NAME "state"
#define MQTT_SN_BROKER_ADDR "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"

static emcute_sub_t sub;
static char *topic = TOPIC_NAME;
static char *message = "work";

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, CLIENT_ID, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("connected to MQTT-SN broker");

    while (1) {
        if (emcute_pub(&sub, topic, message, strlen(message), 0) != EMCUTE_OK) {
            puts("error: unable to publish message");
        } else {
            puts("message published");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN example application");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
