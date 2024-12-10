#include "netinet/in.h"
#include "thread.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define EMCUTE_ID       "device"
#define EMCUTE_PORT     (1883)
#define MQTT_SN_ADDR    "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_topic_t topic;

static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

void *mqtt_pub_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    // Connecting to the gateway
    puts("[MQTT-SN] Connecting to gateway...");
    inet_pton(AF_INET, MQTT_SN_ADDR, &gw.addr);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }
    
    // Registering topic
    puts("[MQTT-SN] Registering topic...");
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Create a thread that publishes messages
    puts("[MQTT-SN] Starting publish thread...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub_thread");
    
    return 0;
}