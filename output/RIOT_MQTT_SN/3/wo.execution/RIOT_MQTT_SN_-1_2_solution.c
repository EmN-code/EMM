#include "thread.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "mqtt_sn_client"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    int pub_res = 0;

    while (1) {
        char *message = "work";
        pub_res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (pub_res != EMCUTE_OK) {
            puts("Error: unable to publish to topic");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN Client Example");

    /* setup our subscription buffers */
    memset(queue, 0, sizeof(queue));
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);

    int res;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res == 0) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    res = emcute_con(&gw, false, NULL, 0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = "state";
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}