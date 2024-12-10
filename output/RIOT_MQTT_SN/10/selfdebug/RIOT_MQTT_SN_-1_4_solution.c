#include <stdio.h>
#include <xtimer.h>
#include <string.h>
#include <net/sock/util.h> // to resolve inet_pton
#include <net/sock/udp.h>
#include <emcute.h>

#define EMCUTE_ID           "gertrud"
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUBLISH_INTERVAL    (5 * 1000000U)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published data: %s\n", (char*)data);
}

int main(void) {
    /* network config */
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv6);

    /* initialize the thread message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute */
    emcute_run(THREAD_PRIORITY_MAIN - 1, stack, sizeof(stack), 0);

    /* connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", SERVER_ADDR, (int)gw.port);
        return 1;
    }

    /* publish continuously every 5 seconds */
    while (1) {
        const char* topics[] = { EMCUTE_ID "/status" };
        const char* data = "work";
        emcute_topic_t t;

        t.name = topics[0];

        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("error: unable to obtain topic ID");
            return 1;
        }

        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data to topic '%s'\n", t.name);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}