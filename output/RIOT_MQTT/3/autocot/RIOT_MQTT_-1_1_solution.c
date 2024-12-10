#include "xtimer.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/network_layer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_TOPIC "state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char mqtt_state[] = "work";
static emcute_client_t mqtt_client;
static emcute_topic_t mqtt_topic;

void *mqtt_publisher_thread(void *arg) {
    (void)arg;
    
    mqtt_topic.name = PUBLISH_TOPIC;
    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        printf("Error: unable to register MQTT topic\n");
        return NULL;
    }

    while (1) {
        int res = emcute_pub(&mqtt_topic, mqtt_state, strlen(mqtt_state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish MQTT message\n");
        } else {
            printf("Published MQTT message successfully\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    emcute_endpoint_t remote = { .family = AF_INET,
                                .port = MQTT_BROKER_PORT };

    /* Convert IPv4 string to bin */
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    /* Initialize EMCUTE and MQTT client */
    emcute_init(&mqtt_client, EMCUTE_PORT, malloc(EMCUTE_STACKSIZE));

    /* Connect to MQTT broker */
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect MQTT broker\n");
        return 1;
    }

    /* Start the MQTT publishing thread */
    thread_create(malloc(THREAD_STACK_DEFAULT),
                  THREAD_STACK_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD, mqtt_publisher_thread, NULL, "mqtt_pub");
    
    return 0;
}