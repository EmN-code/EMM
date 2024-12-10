#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"

#define EMCUTE_ID           "device"
#define EMCUTE_PORT         1883
#define MQTT_BROKER_IP      "47.102.103.1"

static emcute_topic_t topic;
static char pub_msg[] = "work";
static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static int mqtt_connect(void) {
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Connected to MQTT-SN broker.");
            return res;
        }
        else {
            printf("Connection failed with error code %d. Retrying...\n", res);
        }
        xtimer_sleep(1);
    }
}

static int mqtt_publish(void) {
    topic.name = "state";
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return res;
    }
    
    res = emcute_pub(&topic, pub_msg, sizeof(pub_msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    }
    else {
        puts("Published message successfully.");
    }
    return res;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    if (sock_udp_str2ep(&gw, MQTT_BROKER_IP) < 0) {
        puts("Error resolving address");
        return -1;
    }
    
    emcute_run(NULL, EMCUTE_PORT, EMCUTE_ID);

    if (mqtt_connect() == EMCUTE_OK) {
        while (1) {
            mqtt_publish();
            xtimer_sleep(5);
        }
    }
    return 0;
}