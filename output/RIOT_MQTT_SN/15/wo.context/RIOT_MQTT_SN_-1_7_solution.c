#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    puts("Response received");
}

int main(void)
{
    sock_udp_ep_t gw = { .port = MQTT_BROKER_PORT };
    topic.name = "device/state";

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse MQTT broker IP\n");
        return 1;
    }
    
    while (1) {
        int rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Connection to MQTT-SN broker failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }

        /* Register the topic if needed, not always required in MQTT-SN */
        emcute_reg(&topic);

        while (1) {
            const char *state_msg = (rand() % 2) ? "work" : "not work";
            rc = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
            if (rc != EMCUTE_OK) {
                printf("Publish failed: %d\n", rc);
                break;
            }
            printf("Published: %s\n", state_msg);
            xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
        }
        emcute_discon();
    }
    return 0;
}