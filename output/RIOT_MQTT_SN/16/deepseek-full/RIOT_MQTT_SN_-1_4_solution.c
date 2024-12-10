
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>

#define MQTT_SN_BROKER_ADDR "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN sizeof(MSG_CONTENT)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };

void *mqtt_sn_thread(void *arg)
{
    (void)arg;

    // Initialize the MQTT-SN client
    emcute_init();

    // Set up the gateway endpoint
    inet_aton(MQTT_SN_BROKER_ADDR, (struct in_addr *)&gw_ep.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return NULL;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return NULL;
    }

    while (1) {
        // Publish the message
        if (emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0) != EMCUTE_OK) {
            puts("Error: Could not publish message");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Create a thread for MQTT-SN operations
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_sn_thread, NULL, "mqtt_sn");

    return 0;
}
