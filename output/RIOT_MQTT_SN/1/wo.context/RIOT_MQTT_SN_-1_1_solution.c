#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

#define EMCUTE_PORT      (1883U)
#define MQTT_BROKER_IP   "47.102.103.1"
#define DEVICE_ID        "esp32-device"
#define TOPIC_NAME       "device/state"
#define MSG_INTERVAL     (5U * US_PER_SEC)

static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    emcute_topic_t t_topic;
    emcute_sub_t t_sub;
    xtimer_ticks32_t last;
    char *publish_state = "work";
    
    // Initialize EMCUTE
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, 
                  emcute_run, NULL, "emcute");

    // Connect to MQTT-SN broker
    sockaddr_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    if (emcute_con(&gw, true, DEVICE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Set topic
    t_topic.name = TOPIC_NAME;

    // Main loop
    while (1) {
        last = xtimer_now();

        if (emcute_pub(&t_topic, publish_state, strlen(publish_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }

        // Wait for the next time interval
        xtimer_periodic_wakeup(&last, MSG_INTERVAL);
        
        // Toggle state
        publish_state = (strcmp(publish_state, "work") == 0) ? "not work" : "work";
    }

    // Disconnect before exiting
    emcute_discon();

    return 0;
}