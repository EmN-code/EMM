#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/af.h"
#include "stdio.h"
#include "string.h"

#define EMCUTE_PORT     (1883)
#define TOPIC_MAXLEN    (64U)
#define DELAY           (5U) // Delay in seconds

static char *state_work = "work";
static char *state_not_work = "not work";

static emcute_topic_t mqtt_topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT};

void pub_state(void) {
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, state_work); // Using strcpy safely

    int res = emcute_pub(&mqtt_topic, buffer, strlen(buffer), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
}

int main(void) {
    // Convert IP address
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Connect to broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Prepare topic
    mqtt_topic.name = "device/state";
    res = emcute_reg(&mqtt_topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        pub_state();
        ztimer_sleep(ZTIMER_MSEC, DELAY * 1000U); // Sleep for 5 seconds
    }

    return 0;
}