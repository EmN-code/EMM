
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

#define TOPIC_NAME "state"
#define STATE_MSG "work"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void *publish_thread(void *arg)
{
    (void)arg;
    char state_msg[] = STATE_MSG;

    while (1) {
        if (emcute_pub(&topic, state_msg, sizeof(state_msg) - 1, 0) != EMCUTE_OK) {
            puts("Error: Failed to publish message");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Set up the gateway endpoint */
    if (inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: Invalid broker address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Failed to connect to MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Failed to register topic");
        emcute_discon();
        return 1;
    }

    /* Create and start the publish thread */
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish");

    return 0;
}
