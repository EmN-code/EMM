
#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "sys/socket.h"

#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN sizeof(MSG_CONTENT) - 1
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define SLEEP_TIME (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

void connect_to_broker(void) {
    if (inet_aton(BROKER_ADDR, &gw.addr.ipv4) == 0) {
        puts("Error: invalid broker address");
        return;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to broker (code: %d)\n", res);
        return;
    }
    puts("Connected to broker.");
}

void register_topic(void) {
    topic.name = TOPIC_NAME;
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (code: %d)\n", res);
        return;
    }
    puts("Topic registered.");
}

void publish_message(void) {
    int res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message (code: %d)\n", res);
        return;
    }
    puts("Message published.");
}

int main(void) {
    connect_to_broker();
    register_topic();

    while (1) {
        publish_message();
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME);
    }

    return 0;
}
