#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define EMCUTE_ID           (0x1234)
#define EMCUTE_PORT         (1883)
#define EMCUTE_TOPIC_LEN    (64)
#define SERVER_ADDR         "47.102.103.1"
#define PUBLISH_INTERVAL    (5U)  // seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = EMCUTE_PORT };
    char *server = SERVER_ADDR;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, server);

    /* initialize emcute */
    emcute_run(stack, sizeof(stack), EMCUTE_ID, queue, sizeof(queue));
    
    /* Connect to Broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return 1; /* connection failed */
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    char topic[] = "work_status";
    t.name = topic;

    if (emcute_reg(&t) != EMCUTE_OK) {
        return 1; /* registration failed */
    }

    /* Publish data */
    while (1) {
        char *data = "work";

        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            return 1; /* publication failed */
        }

        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}