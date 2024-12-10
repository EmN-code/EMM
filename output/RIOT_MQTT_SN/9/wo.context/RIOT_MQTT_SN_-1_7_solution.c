#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     (1883U)
#define PUB_INTERVAL    (5U)

static emcute_topic_t topic;
static char *STATE = "work";

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    /* initialize our topic */
    topic.name = "state";
    topic.id = 0;
    
    /* network interface */
    sock_udp_ep_t gw = { .family = AF_INET, .addr = SERVER_ADDR, .port = SERVER_PORT };
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    /* register publish topic */
    emcute_reg(&topic);

    while (1) {
        /* publish the state every PUB_INTERVAL seconds */
        emcute_pub(&topic, STATE, strlen(STATE), EMCUTE_QOS_0);
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}