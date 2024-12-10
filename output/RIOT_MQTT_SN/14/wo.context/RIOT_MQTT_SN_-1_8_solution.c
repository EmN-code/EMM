#include <ztimer.h>
#include <msg.h>
#include <net/ipv6/addr.h>
#include <emcute.h>
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, topic.name, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to gateway\n");
        return NULL;
    }

    topic.name = "status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Could not register topic\n");
        return NULL;
    }

    char *messages[2] = {"work", "not work"};
    size_t msg_idx = 0;

    while (1) {
        if (emcute_pub(&topic, messages[msg_idx], strlen(messages[msg_idx]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Could not publish message\n");
        }
        msg_idx = (msg_idx + 1) % 2;
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    return 0;
}