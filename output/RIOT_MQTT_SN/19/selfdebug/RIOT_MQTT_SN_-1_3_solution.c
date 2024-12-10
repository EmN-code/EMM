#include "shell.h"
#include "msg.h"
#include "emcute.h"
#include "net/emcute.h"
#include "stdio.h"

#define MQTT_BUFFER_SIZE     (128U)
#define EMCUTE_ID            "riot-node"
#define BROKER_IP            "47.102.103.1"
#define BROKER_PORT          (1883)
#define MQTT_TOPIC           "state"
#define PUBLISH_INTERVAL     (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received Publication: %.*s\n", (int)len, (char *)data);
}

static void *mqtt_thread(void *arg) {
    (void) arg;
    emcute_sub_t subscription = {
        .topic = {
            .name = MQTT_TOPIC
        },
        .cb = on_pub,
    };

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t*)&gw.addr.ipv6, BROKER_IP);

    emcute_reg(&topic);
    emcute_sub(&subscription, EMCUTE_QOS_0);

    while (1) {
        static const char *data = "work";
        emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_run(BROKER_PORT, EMCUTE_ID);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    shell_run(shell_commands, line_buf, sizeof(line_buf));
    return 0;
}