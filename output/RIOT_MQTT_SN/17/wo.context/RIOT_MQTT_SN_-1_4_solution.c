#include <xtimer.h>
#include <net/emcute.h>
#include <msg.h>

#define EMCUTE_PORT      (1883U)
#define MQTT_BROKER_IP   "47.102.103.1"
#define PUB_TOPIC        "state"

static emcute_topic_t my_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static msg_t queue[8];

int main(void) {
    msg_init_queue(queue, 8);

    emcute_topic_t t;
    t.name = PUB_TOPIC;

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%u\n", MQTT_BROKER_IP, EMCUTE_PORT);
        return 1;
    }

    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        const char *state_msg = "work or not work";
        if (emcute_pub(&my_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}