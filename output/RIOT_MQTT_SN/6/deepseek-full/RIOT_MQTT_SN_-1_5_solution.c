
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <stddef.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t remote;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)publish_state, NULL, "publish_state");

    return 0;
}
