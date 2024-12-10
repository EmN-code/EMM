#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/conn/ip.h>
#include <net/conn/mqttsn.h>

#define EMCUTE_ID         "mqtt-client"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IPv4  "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define TOPIC_NAME        "esp32-state"
#define PUB_INTERVAL      (5U)   // Publish interval in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue)/sizeof(msg_t));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    conn_mqttsn_client_init();

    /* Setup connection parameters */
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = MQTT_BROKER_PORT };
    /* Parse the IP address */
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, MQTT_BROKER_IPv4);

    /* Connect to the MQTT broker */
    if (emcute_con(&gw, true, _CONN_PASS) != EMCUTE_OK) {
        puts("mqttsn: unable to connect to MQTT broker");
        return 1;
    }

    /* Setup topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("mqttsn: unable to register MQTT topic");
        return 1;
    }

    char *states[2] = { "work", "not work" };
    unsigned int index = 0;

    while (1) {
        /* Publish the current state */
        if (emcute_pub(&topic, (uint8_t *)states[index], strlen(states[index]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("mqttsn: unable to publish message\n");
        }

        /* Toggle state for next iteration */
        index = (index + 1) % 2;

        /* Wait for the next publish interval */
        xtimer_sleep(PUB_INTERVAL);
    }

    /* Disconnect before exiting the program */
    emcute_discon();
    return 0;
}