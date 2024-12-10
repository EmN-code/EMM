#include "arpa/inet.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "thread.h"
#include "xtimer.h"

#define EMCUTE_PORT      (1883U)
#define EMCUTE_ID        ("esp32-client")
#define MQTT_TOPIC_NAME  ("device/status")
#define MQTT_BROKER_IP   ("47.102.103.1")
#define PUBLISH_INTERVAL (5U)  // Publish every 5 seconds

enum { 
    STATE_WORKING = 0,
    STATE_NOT_WORKING
};

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
emcute_topic_t t;

static void *publisher_thread(void *arg) {
    (void)arg;
    unsigned state = STATE_WORKING;

    while (1) {
        const char *msg = (state == STATE_WORKING) ? "work" : "not work";
        if (emcute_pub(&t, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        // Toggle state for next publish
        state = (state == STATE_WORKING) ? STATE_NOT_WORKING : STATE_WORKING;
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("error: unable to parse broker IP address");
        return -1;
    }

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return -1;
    }

    memset(&t, 0, sizeof(t));
    t.name = MQTT_TOPIC_NAME;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return -1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}