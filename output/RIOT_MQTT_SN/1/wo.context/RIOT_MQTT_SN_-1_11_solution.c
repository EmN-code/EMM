#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/mqttsn.h>
#include <net/ipv6/addr.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define STATE_WORK "work"
#define STATE_NOT_WORK "not_work"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"

static emcute_topic_t _state_topic;

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static void *mqttsn_thread(void *arg) {
    (void)arg; // Avoid unused parameter warning

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN gateway\n");
        return NULL;
    }

    _state_topic.name = TOPIC_NAME;
    if (emcute_reg(&_state_topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return NULL;
    }

    char *state_msg = STATE_WORK;

    while (1) {
        if (emcute_pub(&_state_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish message\n");
        }
        sleep(5);
        // Optionally toggle between states to simulate working/not working
        state_msg = strcmp(state_msg, STATE_WORK) == 0 ? STATE_NOT_WORK : STATE_WORK;
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, mqttsn_thread, NULL, "mqttsn");

    puts("MQTT-SN example application
");

    return 0;
}