#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "event/thread.h"
#include "thread.h"

#define EMCUTE_ID "device123"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS 16
#define TOPIC_MAXLEN 64

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char *topic_name = "state_topic";

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received data: %s\n", (char *)data);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to gateway\n");
            break;
        }
        else {
            printf("Connection failed, retrying...\n");
            xtimer_sleep(1);
        }
    }

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID\n");
        return 1;
    }

    char *message = "work or not work";
    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Data published: %s\n", message);
        }
        xtimer_sleep(5);
    }
    return 0;
}