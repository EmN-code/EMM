#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("test_mqtt_client")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC     ("device/state")
#define BROKER_IP       "47.102.103.1"
#define BROKER_PORT     (1883U)
#define PUBLISH_DELAY   (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse broker IP");
        return NULL;
    }

    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    while (1) {
        xtimer_sleep(PUBLISH_DELAY / US_PER_SEC);

        topic.name = STATE_TOPIC;
        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Failed to register topic");
            continue;
        }

        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Failed to publish message");
        }
    }
    return NULL;
}

int main(void) {
    puts("Starting MQTT-SN client...");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}