#include <xtimer.h>
#include <string.h>
#include <net/emcute.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "device/status"

static emcute_topic_t _topic;
static char _mqtt_message[64];
static kernel_pid_t _emcute_pid;

static void publish_state(void) {
    static int counter = 0;
    snprintf(_mqtt_message, sizeof(_mqtt_message), "Device state: %s", (counter++ % 2) ? "work" : "not work");
    emcute_pub(&_topic, _mqtt_message, strlen(_mqtt_message), EMCUTE_QOS_0);
}

int main(void) {
    emcute_sub_t subscriptions[1];
    _emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_run, NULL, "emcute");
    
    xtimer_ticks32_t last_wakeup = xtimer_now();

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS);

    _topic.name = TOPIC_NAME;
    emcute_reg(&_topic);

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }
    
    return 0;
}