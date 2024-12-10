#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define MQTT_SN_PORT (1883U)
#define BUF_SIZE    (128U)
#define PUBLISH_INTERVAL (5U)
#define MQTT_TOPIC_NAME  "device/state"

static char publish_stack[THREAD_STACKSIZE_DEFAULT];
static char state_message[] = "work or not work";
static sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_SN_PORT };

static void *publish_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC_NAME;

    while (1) {
        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Error: unable to register topic");
            continue;
        }

        if (emcute_pub(&topic, state_message, sizeof(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];

    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);
    msg_init_queue(queue, 8);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    thread_create(publish_stack, sizeof(publish_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    return 0;
}