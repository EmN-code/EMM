
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_BUF_SIZE 20

static char msg_buf[MSG_BUF_SIZE];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
    topic.name[sizeof(topic.name) - 1] = '\0';

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        strncpy(msg_buf, "work", sizeof(msg_buf) - 1);
        msg_buf[sizeof(msg_buf) - 1] = '\0';

        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
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
    emcute_init();

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    return 0;
}
