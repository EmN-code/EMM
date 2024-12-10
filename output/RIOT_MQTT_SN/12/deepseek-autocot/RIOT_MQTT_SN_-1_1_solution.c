
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_t gw_addr = { .u32 = IPv4_ADDR(47, 102, 103, 1) };
    gw.addr.ipv4 = gw_addr;

    printf("Connecting to MQTT-SN broker at %d.%d.%d.%d:%d\n",
           (int)gw.addr.ipv4.u8[0], (int)gw.addr.ipv4.u8[1],
           (int)gw.addr.ipv4.u8[2], (int)gw.addr.ipv4.u8[3], EMCUTE_PORT);

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    printf("Connected to MQTT-SN broker\n");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register to the topic");
        return NULL;
    }

    while (1) {
        char payload[20];
        strcpy(payload, "work");  // Change this to "not work" if needed
        emcute_pub(&topic, payload, strlen(payload), 0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
