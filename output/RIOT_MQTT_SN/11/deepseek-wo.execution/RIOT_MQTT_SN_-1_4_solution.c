
#include <net/ipv6/addr.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static emcute_sub_t sub;

void publish_state(void)
{
    static int state = 0;
    const char *msg = (state++ % 2) ? MSG_NOT_WORK : MSG_WORK;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    emcute_pub(&topic, msg, strlen(msg), 0);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    emcute_init();

    while (1) {
        publish_state();
        xtimer_usleep(5 * US_PER_SEC);
    }

    return 0;
}
