
#include <net/ipv6/addr.h>
#include <thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_sub_t sub;
static emcute_topic_t topic;

static void *pub_thread(void *arg) {
    (void)arg;

    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = EMQX_SERVER_PORT;

    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error parsing IP address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, pub_thread, NULL, "publisher");

    return 0;
}
