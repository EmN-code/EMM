#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define CLIENT_ID "esp32_device"
#define MQTT_MSG "work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

#define NUMOFSUBS 16
#define EMCUTE_PORT   1883
#define EMCUTE_ID     "device"
#define EMCUTE_PRIO   (THREAD_PRIORITY_MAIN - 1)
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(EMCUTE_PORT, CLIENT_ID);
    return NULL;
}

static int mqtt_pub(const char *topic, const char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    if (qos == 1) {
        flags |= EMCUTE_QOS_1;
    } else if (qos == 2) {
        flags |= EMCUTE_QOS_2;
    }

    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("[MQTT-SN] error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("[MQTT-SN] error: unable to publish data\n");
        return 1;
    }

    printf("[MQTT-SN] published %s on topic %s\n", data, topic);
    return 0;
}

int main(void) {
    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to broker */
    sock_udp_ep_t gw = {
        .port = BROKER_PORT
    };
    memcpy(&gw.addr.ipv4, &netif_get_any(NETDEV_IPV4)->ipv4, sizeof(ipv6_addr_t));
    if (emcute_con(&gw, true, CLIENT_ID, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT-SN] error: unable to connect to broker\n");
        return 1;
    }

    /* start the periodic publish loop */
    while (1) {
        mqtt_pub(MQTT_TOPIC, MQTT_MSG, 0);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    /* disconnect from broker */
    emcute_discon();

    return 0;
}