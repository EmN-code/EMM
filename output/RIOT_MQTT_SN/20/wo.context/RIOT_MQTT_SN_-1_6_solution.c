#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define EMCUTE_ID "esp32-device"
#define PUB_INTERVAL (5U * ZTIMER_MSEC_SEC)
#define EMCUTE_TOPIC_MAXLEN (64U)
#define EMCUTE_BUFSIZE (128U)

static char publish_data[] = "work";

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void *thread(void *arg) {
    (void)arg;
    while (1) {
        emcute_pub(&topic, publish_data, sizeof(publish_data), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    emcute_run(BROKER_PORT, EMCUTE_ID);

    emcute_reg(&topic);

    ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread, NULL, "emcute thread");

    return 0;
}