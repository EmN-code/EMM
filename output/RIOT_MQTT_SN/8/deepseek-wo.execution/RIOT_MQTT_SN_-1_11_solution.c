
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <dht_params.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    const char *msg = MSG;
    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    // Initialize the MQTT-SN client
    emcute_init();

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
