#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_ID "riot"
#define EMCUTE_PORT 1883
#define MQTT_STATE_TOPIC "state"

static emcute_topic_t topics[1];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *publisher(void *arg) {
    (void)arg;
    char *publish_message = "work";
    while (1) {
        xtimer_sleep(5);
        if (emcute_pub(&topics[0], publish_message, strlen(publish_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    topics[0].name = MQTT_STATE_TOPIC;
    topics[0].id = 0;

    if (emcute_reg(&topics[0]) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publisher, NULL, "publisher");

    return 0;
}

static char stack[THREAD_STACKSIZE_DEFAULT];