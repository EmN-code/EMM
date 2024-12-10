
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT 1883
#define BROKER_IP "47.102.103.1"
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    ipv4_addr_t broker_addr;

    if (ipv4_addr_from_str(&broker_addr, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker IP address");
        return NULL;
    }

    gw.addr.ipv4 = broker_addr;
    gw.family = AF_INET;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    puts("Connected to MQTT-SN broker");

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", state);
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN example");

    thread_create(emcute_stack, sizeof(emcute_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
