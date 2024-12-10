
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"
#include "msg.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static emcute_sub_t sub;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IPv4 address");
        return NULL;
    }

    if (sock_udp_create(&sub.sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_run(EMCUTE_PORT, &sub.sock);
    return NULL;
}

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the broker

    MQTTMessage message;
    message.qos = EMCUTE_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)states[state_index];
    message.payloadlen = strlen(states[state_index]);

    if (emcute_pub(&topic, &message) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published successfully");
    }

    state_index = (state_index + 1) % 2;  // Toggle between "work" and "not work"
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue));

    puts("Starting MQTT-SN client...");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
