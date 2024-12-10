#include "ztimer/ztimer.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID "esp32_client"
#define EMCUTE_PORT (1883U)
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (1883U)
#define TOPIC_NAME "device/state"
#define MSG_INTERVAL (5U) // every 5 seconds

static emcute_topic_t _topic;
static char _message[8]; // message buffer for "work" or "not work"

static int setup_mqtt(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[FAILED] Connection to the broker\n");
        return -1;
    }

    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("[FAILED] Registration of topic\n");
        return -1;
    }
    return 0;
}

static void periodic_pub(void) {
    static bool work_state = false;

    work_state = !work_state;
    snprintf(_message, sizeof(_message), "%.4s", work_state ? "work" : "not work");

    if (emcute_pub(&_topic, _message, strlen(_message), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("[FAILED] Publish state\n");
    }
}

int main(void) {
    puts("MQTT-SN client started\n");
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    if (setup_mqtt() != 0) {
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, MSG_INTERVAL * MS_PER_SEC);

    while (1) {
        periodic_pub();
        ztimer_sleep(ZTIMER_MSEC, MSG_INTERVAL * MS_PER_SEC);
    }

    return 0;
}