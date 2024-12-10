
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_id[16];
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
static emcute_topic_t topic;

void publish_state(void)
{
    static int state = 0;
    const char *msg = (state++ % 2) ? MSG_NOT_WORK : MSG_WORK;
    emcute_pub(&topic, msg, strlen(msg), 0);
}

int main(void)
{
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }

    // Set the gateway address
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    gw.addr.ipv4 = gw_addr;

    // Initialize the EMCUTE thread
    sprintf(emcute_id, "esp32_%04x", (unsigned int)xtimer_now_usec());
    emcute_run(1883, emcute_id);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
