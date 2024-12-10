
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
#define TOPIC_NAME "state"
#define MSG_CONTENT "work"

static emcute_topic_t topic;
static sock_udp_ep_t gw;

int main(void)
{
    // Initialize the MQTT-SN gateway endpoint
    gw.family = AF_INET;
    gw.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish the state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MSG_CONTENT, sizeof(MSG_CONTENT) - 1, 0) != EMCUTE_OK) {
            puts("Error: Could not publish message");
        }
        xtimer_sleep(5);
    }

    return 0;
}
