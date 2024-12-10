#include <ztimer.h>
#include <stdio.h>
#include <emcute.h>  // Ensure the emcute library path is included in the build environment
#include <net/ipv6/addr.h>

#define BROKER_IP  "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID   "esp32_client"
#define TOPIC_NAME  "state/status"
#define MSG_WORK    "work"
#define MSG_NOTWORK "not work"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication on topic %s [%i]: %s\n", topic->name, topic->id, (char*)data);
}

static int connect_gateway(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);

    printf("Connecting to gateway at [%s]:%d\n", BROKER_IP, BROKER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }
    return 0;
}

static int setup_topic(void) {
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }
    return 0;
}

int main(void) {
    printf("MQTT-SN Demo using emcute\n");
    emcute_run(BROKER_PORT, CLIENT_ID);

    if (connect_gateway() != 0) {
        return 1;
    }

    if (setup_topic() != 0) {
        return 1;
    }

    while (1) {
        printf("Publishing work status...\n");
        if (emcute_pub(&topic, MSG_WORK, strlen(MSG_WORK), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        }
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}