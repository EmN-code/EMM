#include <xtimer.h> 
#include <net/mqttsn.h>
#include <net/ipv6/addr.h>
#include <stdio.h>

#define QUEUE_SIZE 8
static msg_t queue[QUEUE_SIZE];

static emcute_topic_t topic;

int main(void)
{
    msg_init_queue(queue, QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

    if (inet_pton(AF_INET6, "::ffff:47.102.103.1", &gw.addr) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    printf("Client ID: esp32\n");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }
    puts("Connection successful\n");

    topic.name = "test/topic";

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    while (1) {
        const char *state = "work";  // or "not work"
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        } else {
            puts("Data published\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}