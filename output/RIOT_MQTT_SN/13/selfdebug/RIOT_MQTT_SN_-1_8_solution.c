#include "net/emcute.h"
#include "kernel.h"
#include "msg.h"
#include "stdio.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         (1883)
#define BROKER_IP           "47.102.103.1"
#define TOPIC_NAME          "state"
#define MSG_WORK            "work"
#define MSG_NOT_WORK        "not work"
#define PUB_INTERVAL        (5U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, const void *data, size_t len)
{
    (void)topic;
    printf("Received publication: '%.*s'\n", (int)len, (char *)data);
}

int main(void)
{
    kernel_pid_t emcute_pid;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    msg_init_queue(queue, ARRAY_SIZE(queue));
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    /* Initialize the MQTT-SN stack */
    emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                               emcute_thread, NULL, "emcute");

    /* Connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);
        return 1;
    }

    /* Set up subscription */
    topic.name = TOPIC_NAME;
    emcute_sub(&topic, EMCUTE_QOS_0);

    while (1) {
        const char *msg = MSG_WORK;
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
            return 1;
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}