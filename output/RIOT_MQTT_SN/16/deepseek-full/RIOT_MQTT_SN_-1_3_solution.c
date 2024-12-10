
#include "event/thread.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN (sizeof(MSG_CONTENT) - 1)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void *pub_thread(void *arg)
{
    (void)arg;
    while (1) {
        int res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0);
        if (res != EMCUTE_OK) {
            puts("Error publishing message");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the MQTT-SN client */
    emcute_init();

    /* Set the gateway endpoint */
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    /* Connect to the MQTT-SN gateway */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Start the publishing thread */
    thread_create(_main_msg_queue, sizeof(_main_msg_queue), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "publisher");

    return 0;
}
