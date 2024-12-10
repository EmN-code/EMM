#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Include this to declare inet_pton
#include <event.h>
#include <sys/types.h>
#include <unistd.h>

#include "net/emcute.h"
#include "msg.h"

#define EMCUTE_ID     "station"
#define EMCUTE_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS     (16U)
#define TOPIC_MAXLEN  (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[2];
static emcute_topic_t t;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void)
{
    /* initialize the emcute stack */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute thread */
    pthread_t emcute_thr;
    pthread_create(&emcute_thr, NULL, emcute_thread, NULL);

    /* prepare remote endpoint */
    sock_udp_ep_t gw;
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    gw.port = EMCUTE_PORT;

    // This will convert the string IP address to a struct in_addr for IPV4
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        printf("inet_pton failed\n");
        return 1;
    }

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("emcute_con failed\n");
        return 1;
    }

    t.name = "status";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("emcute_reg failed\n");
        return 1;
    }

    /* publish on the topic every 5 seconds */
    while (1) {
        char *state = "work"; /* alternatively "not work" */
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("emcute_pub failed\n");
        }
        sleep(5);
    }

    return 0;
}