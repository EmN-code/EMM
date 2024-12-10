#include <string.h>
#include <stdio.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t my_topic;

void publish_state(void) {
    const char *state = "work or not work";
    size_t len = strlen(state);
    
    int res = emcute_pub(&my_topic, state, len, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("[Error] Unable to publish data\n");
    }
    else {
        printf("Published: %s\n", state);
    }
}

static void *publisher(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };  
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);
    gw.port = MQTT_BROKER_PORT;

    char *topic_name = "state";
    my_topic.name = topic_name;

    int ret = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (ret != EMCUTE_OK) {
        printf("[Error] Connection to broker failed\n");
        return 1;
    }
    printf("Connected to broker at %s\n", MQTT_BROKER_IP);

    ret = emcute_reg(&my_topic);
    if (ret != EMCUTE_OK) {
        printf("[Error] Topic registration failed\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}