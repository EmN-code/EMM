
#include <stdlib.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <thread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MESSAGE "work"

static emcute_topic_t topic;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            puts("Error publishing message");
        }
        thread_sleep();
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_aton(BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publisher");

    emcute_run(BROKER_PORT, CLIENT_ID);

    return 0;
}
