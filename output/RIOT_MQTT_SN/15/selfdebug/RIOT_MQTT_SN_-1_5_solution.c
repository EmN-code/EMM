#include <string.h>
#include <stdio.h>
#include <arpa/inet.h> // Include for inet_pton
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define PUBLISH_INTERVAL (5U)

enum { WORK, NOT_WORK };

static emcute_topic_t _topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Published data: %.*s\n", (int)len, (char *)data);
    return NULL;
}

static int publish_state(int state) {
    char *data = state == WORK ? "work" : "not work";
    return emcute_pub(&_topic, data, strlen(data), EMCUTE_QOS_0);
}

int main(void) {
    printf("Setting up MQTT-SN client...\n");

    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    while (1) {
        printf("Connecting to gateway...\n");

        if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
            _topic.name = "device/state";
            _topic.id = 0;
            printf("Registering topic...\n");
            if (emcute_reg(&_topic) != EMCUTE_OK) {
                printf("Error: unable to register topic\n");
                continue;
            }

            // Publish state every 5 seconds
            while (1) {
                printf("Publishing state...\n");
                publish_state(WORK);
                xtimer_sleep(PUBLISH_INTERVAL);
            }
        }
        else {
            printf("Error: unable to connect to gateway\n");
            xtimer_sleep(5);
        }
    }

    return 0;
}