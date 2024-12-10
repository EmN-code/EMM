#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/if.h>

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "state"
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    "1883"
#define MQTT_PAYLOAD        "work or not work"
#define SLEEP_INTERVAL      (5U * US_PER_SEC)

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_recv(&msg);
    return NULL;
}

int main(void) {
    char *topic_name = TOPIC_NAME;
    emcute_sub_t subscription;
    thread_t emcute_thread_pid;

    /* start emcute thread */
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* setup address struct for MQTT broker */
    sock_udp_ep_t gw = {.family = AF_INET6, .port = atoi(MQTT_BROKER_PORT)};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);

    /* initialize the emcute module */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%s\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return 1;
    }
    printf("Successfully connected to %s
", MQTT_BROKER_IP);

    /* setup subscription */
    subscription.topic_name = topic_name;
    subscription.cb = NULL;

    while (1) {
        /* publish message */
        if (emcute_pub(topic_name, MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data
");
        } else {
            printf("Published: %s
", MQTT_PAYLOAD);
        }

        /* sleep for 5 seconds */
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}