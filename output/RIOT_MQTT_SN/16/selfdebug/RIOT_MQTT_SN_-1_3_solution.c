#include <ztimer.h>   // For the ztimer_sleep function
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <emcute.h>  // For the emcute MQTT-SN functionalities
#include <net/sock/udp.h>  // Socket for UDP communication

#define SLEEP_TIME      (5U)   // 5 seconds sleep time
#define EMCUTE_PORT     (1883U)

static emcute_topic_t topic;
static const char *topic_name = "state";
static char *message;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    // Assuming 'ipv4' needs to be used based on error message
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv4, "47.102.103.1");

    // Connect to the broker
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register the topic
    topic.name = topic_name;
    emcute_reg(&topic);

    while (1) {
        // Alternately send "work" and "not work"
        message = (strcmp(message, "work") == 0) ? "not work" : "work";

        // Publish the message
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);  // QoS 0 is sufficient here

        // Sleep for the specified time
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME * US_PER_SEC);
    }

    return 0;
}