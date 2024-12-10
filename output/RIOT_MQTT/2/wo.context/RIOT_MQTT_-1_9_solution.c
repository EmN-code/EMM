#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "thread.h"

#define MQTT_BROKER_ADDRESS  "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define MQTT_PUBLISH_TOPIC   "status"
#define MQTT_CLIENT_ID       "esp32_mqtt_client"
#define MQTT_INTERVAL        (5U)
#define EMCUTE_DEFAULT_PORT  1883

static char stack_buffer[THREAD_STACKSIZE_MAIN];
static emcute_topic_t mqtt_topic;

typedef struct {
    sock_udp_ep_t gw_ep;
} mqtt_config_t;

/* Connection context */
mqtt_config_t mqtt_config = {
    .gw_ep = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF }
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    puts("[MQTT] Published successfully.");
}

static int mqtt_eventloop(void *arg) {
    (void)arg;
    char message[32];
    int i = 0;

    while (1) {
        i++;
        if (i % 2 == 0) {
            sprintf(message, "work");
        } else {
            sprintf(message, "not work");
        }

        if (emcute_pub(&mqtt_topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("[MQTT] Unable to publish");
        }
        xtimer_sleep(MQTT_INTERVAL);
    }
    return 0;
}

int main(void) {
    mqtt_config.gw_ep.port = MQTT_BROKER_PORT;
    mqtt_topic.name = MQTT_PUBLISH_TOPIC;
    mqtt_topic.id = rand(); /* Random topic ID */

    if (emcute_con(&mqtt_config.gw_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT] Connection failed");
        return 1;
    }
    puts("[MQTT] Connected successfully");

    if (emcute_subscribe(&mqtt_topic, on_pub) != EMCUTE_OK) {
        puts("Error subscribing to topic");
    }

    thread_create(stack_buffer, sizeof(stack_buffer), 1,
                  THREAD_CREATE_STACKTEST,
                  mqtt_eventloop,
                  NULL, "mqtt event loop");

    while(1) {
        thread_yield();
    }

    return 0;
}
