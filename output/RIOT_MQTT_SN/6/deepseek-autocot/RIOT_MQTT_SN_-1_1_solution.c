
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_mqtt_client"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char mqtt_sn_broker_addr[IPV6_ADDR_MAX_STR_LEN];
static emcute_topic_t topic;
static emcute_sub_t sub;

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"not work", "work"};

    // Create the message
    char msg[32];
    snprintf(msg, sizeof(msg), "%s", states[state]);

    // Publish the message
    emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0, false);

    // Toggle state
    state = !state;
}

static void *mqtt_sn_thread(void *arg)
{
    (void)arg;

    // Initialize the MQTT-SN client
    emcute_init();

    // Connect to the MQTT-SN broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_t broker_addr;
    inet_pton(AF_INET, mqtt_sn_broker_addr, &broker_addr);
    gw.addr.ipv4 = broker_addr;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Set the MQTT-SN broker address
    strncpy(mqtt_sn_broker_addr, "47.102.103.1", sizeof(mqtt_sn_broker_addr));

    // Start the MQTT-SN thread
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, mqtt_sn_thread, NULL, "mqtt_sn_thread");

    // Run the shell to allow for manual interaction
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
