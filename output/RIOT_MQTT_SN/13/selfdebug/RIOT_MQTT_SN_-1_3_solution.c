#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"  // Including the right header
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "esp32/state"
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)  // every 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Publication received on topic '%s' [%i]: %s\n", topic->name, topic->id, (char *)data);
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };

    // Convert IP address
    if (inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr) == 0) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    // Connect to the MQTT broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");
    return 0;
}

static void mqtt_publish(const char* message) {
    // Set topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Publish message
    if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish message: %s\n", message);
    } else {
        printf("Published message: %s\n", message);
    }
}

int main(void) {
    puts("MQTT-SN Publisher");

    // Connect to the broker
    if (mqtt_connect() != 0) {
        return 1;
    }

    // Setup a periodic timer to publish messages
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        mqtt_publish(MESSAGE);
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }
    
    return 0;
}