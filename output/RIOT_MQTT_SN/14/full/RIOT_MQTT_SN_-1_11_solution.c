#include "ztimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "status"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds

static emcute_topic_t status_topic;

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = MQTT_BROKER_PORT};
    inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6);
    
    // Initialize emcute
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return 1;
    }

    // Register the topic
    status_topic.name = TOPIC;
    if (emcute_reg(&status_topic) != EMCUTE_OK) {
        return 1;
    }

    // Infinite loop to publish periodically
    while (1) {
        // Simulate getting the state
        const char *state = "work"; // or "not work" based on your logic
        if (emcute_pub(&status_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish to MQTT-SN");
            break;
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Graceful shutdown
    emcute_discon();
    return 0;
}