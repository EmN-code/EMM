
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <thread.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

#define TOPIC "state"
#define MSG "work"
#define MSG_LEN (sizeof(MSG) - 1)

static char stack[STACKSIZE];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, MSG, MSG_LEN, 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), PRIORITY, 0, emcute_thread, NULL, "emcute");

    return 0;
}
