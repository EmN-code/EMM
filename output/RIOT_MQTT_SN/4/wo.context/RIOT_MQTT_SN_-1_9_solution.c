#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <event.h>
#include <net/sock/udp.h>

#define MQTT_SN_PORT     (1883U)
#define DEFAULT_EVENTQ_SIZE (8U)
#define STATE_MSG_WORK   "work"
#define STATE_MSG_NOT_WORK "not work"

static char* broker_ip = "47.102.103.1";
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_SN_PORT };

static char message_buffer[16];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int publish_state(const char *state) {
    snprintf(message_buffer, sizeof(message_buffer), "%s", state);
    if (emcute_pub(&topic, message_buffer, strlen(message_buffer), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }
    return 0;
}

int main(void) {
    char stack_buffer[THREAD_STACKSIZE_DEFAULT];
    ztimer_sleep(ZTIMER_MSEC, 1000);

    gw.port = MQTT_SN_PORT;
    gw.family = AF_INET;
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, broker_ip);

    topic.name = "state";

    puts("Connecting...");
    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state(STATE_MSG_WORK);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}