
#include "event/thread.h"
#include "net/emcute.h"
#include "thread.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "sys/socket.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_BUF_SIZE 20

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void *pub_thread(void *arg)
{
    (void)arg;
    char msg_buf[MSG_BUF_SIZE];
    int res;

    while (1) {
        snprintf(msg_buf, sizeof(msg_buf), "work or not work");
        res = emcute_pub(&topic, msg_buf, strlen(msg_buf), 0);
        if (res != EMCUTE_OK) {
            puts("Error publishing message");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (result != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    result = emcute_reg(&topic);
    if (result != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "publisher");

    return 0;
}
