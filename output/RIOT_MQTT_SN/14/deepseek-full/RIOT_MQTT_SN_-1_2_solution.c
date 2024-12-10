
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_BUF_SIZE 20

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

void register_topic(void) {
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_ADDR) == NULL) {
        puts("Error: unable to parse broker address");
        return;
    }

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register topic");
    } else {
        puts("Topic registered successfully");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, BROKER_ADDR) == NULL) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    register_topic();

    char msg_buf[MSG_BUF_SIZE];
    while (1) {
        snprintf(msg_buf, sizeof(msg_buf), "work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_sleep(5);

        snprintf(msg_buf, sizeof(msg_buf), "not work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_sleep(5);
    }

    return 0;
}
