
#include <thread.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN (sizeof(MSG_CONTENT) - 1)
#define FLAGS (0)  // No special flags for QoS and retain

static emcute_topic_t topic;
static sock_udp_ep_t remote;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, FLAGS);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Start the publishing thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
