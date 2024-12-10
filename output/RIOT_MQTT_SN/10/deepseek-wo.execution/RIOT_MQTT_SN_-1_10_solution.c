
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static emcute_topic_t topic;

void *pub_thread(void *arg)
{
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    char msg_buf[] = "work";
    while (1) {
        if (emcute_pub(&topic, msg_buf, sizeof(msg_buf) - 1, 0) != EMCUTE_OK) {
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
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "publisher");
    return 0;
}
