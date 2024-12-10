#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time_units.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "msg.h"
#include "muted.h"
#include "xtimer.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           16
#define TOPIC_MAXLEN        64
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUB_INTERVAL        (5 * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static size_t num_subscriptions = 0;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("### received publication for topic '%s' ###\n", topic->name);
    printf("data=%.*s\n", (int)len, in);
}

static void *emcute_thread(void *arg)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    (void)arg;

    msg_init_queue(queue, ARRAY_SIZE(queue));
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    t.name = "work/status";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return NULL;
    }

    while (1) {
        int res = emcute_pub(&t, "work", strlen("work"), flags);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    struct sockaddr_in server_addr;

    /* initialize UDP endpoint to connect to MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &server_addr.sin_addr);
    gw.addr.ipv4 = server_addr.sin_addr.s_addr;

    /* initialize emcute */
    emcute_sock_udp_ep_init(&gw);

    bool connected = false;

    while (!connected) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("error: unable to connect to gateway\n");
            xtimer_sleep(1);
        } else {
            connected = true;
        }
    }

    printf("Connected to the broker\n");

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}