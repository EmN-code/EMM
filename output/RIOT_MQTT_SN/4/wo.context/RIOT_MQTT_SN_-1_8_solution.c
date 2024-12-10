#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/mqttsn.h>
#include <net/emcute.h>

#define BROKER_IP    "47.102.103.1"
#define BROKER_PORT  1883
#define TOPIC_NAME   "device/status"

#define EMCUTE_ID    "riot-node"
#define EMCUTE_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define CONF_QOS     EMCUTE_QOS_0

static char stack[EMCUTE_STACK_SIZE];
static emcute_topic_t topic;
static char msg[] = "work";

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published %s on topic %s\n", (char *)data, topic->name);
}

static void *mqttsn_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .port = BROKER_PORT };

    /* parse broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        puts("Error: unable to parse IP address\n");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return NULL;
    }

    topic.name = TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, msg, sizeof(msg), CONF_QOS) != EMCUTE_OK) {
            puts("error: unable to publish\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    /* initialize emcute */
    emcute_run(BROKER_PORT, EMCUTE_ID);

    /* start the MQTT-SN thread */
    kernel_pid_t emcute_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqttsn_thread, NULL, "mqttsn");

    if (emcute_thread_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to start MQTT-SN thread\n");
        return 1;
    }

    return 0;
}