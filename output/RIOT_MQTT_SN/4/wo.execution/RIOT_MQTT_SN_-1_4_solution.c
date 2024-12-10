#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("mqtt-sample")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static int pub_state(void) {
    emcute_topic_t topic;
    unsigned flags = 0;
    const char *state_msg = "work";  // Default message

    /* Connect to broker at 47.102.103.1:1883 */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", "47.102.103.1", EMCUTE_PORT);
        return 1;
    }

    /* Setup topic */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* Publish the message in an infinite loop */
    while (1) {
        if (emcute_pub(&topic, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(5);

        /* Alternate message */
        state_msg = (strcmp(state_msg, "work") == 0) ? "not work" : "work";
    }

    return 0;
}

int main(void) {
    puts("MQTT-SN sample application\n");
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, pub_state, NULL, "pub_thread");

    return 0;
}