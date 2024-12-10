#include <net/inet.h>
#include <net/emcute.h>
#include <xtimer.h>

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    emcute_topic_t t;
    t.name = "state";

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker.\n");
        return -1;
    }

    // Register the topic
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        return -1;
    }

    // Publish data every 5 seconds
    while (1) {
        const char *data = "work";  // or "not work" depending on your condition
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data.\n");
        }
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}