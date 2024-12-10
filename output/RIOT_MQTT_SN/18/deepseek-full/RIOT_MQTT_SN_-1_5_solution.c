
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *state = "work"; // or "not work" based on your logic
        emcute_pub(&topic, state, strlen(state), 0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "publisher");

    return 0;
}
