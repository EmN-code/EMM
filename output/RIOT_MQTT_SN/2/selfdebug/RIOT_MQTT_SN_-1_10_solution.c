#include <msg.h>
#include <net/conn/ip.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PRIO         THREAD_PRIORITY_MAIN - 1
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[NUMOFSUBS];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topicname[TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    char *work_msg = "work";
    char *not_work_msg = "not work";
    int work_state = 0;

    while (1) {
        xtimer_sleep(5);
        emcute_topic_t t;
        unsigned flags = EMCUTE_QOS_0;
        t.name = topicname;
        t.id = 0;

        if (work_state) {
            if (emcute_pub(&t, work_msg, strlen(work_msg), flags) != EMCUTE_OK) {
                printf("error: unable to publish data\n");
            }
        } else {
            if (emcute_pub(&t, not_work_msg, strlen(not_work_msg), flags) != EMCUTE_OK) {
                printf("error: unable to publish data\n");
            }
        }
        work_state = !work_state;
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, NUMOFSUBS);

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* setup target address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }

    /* set the topic name we will publish to */
    strcpy(topicname, "state/status");

    /* keep main thread active */
    while (1) {
        xtimer_sleep(1);
    }

    /* disconnect from broker */
    if (emcute_discon() != EMCUTE_OK) {
        printf("error: unable to disconnect from gateway\n");
        return 1;
    }

    return 0;
}