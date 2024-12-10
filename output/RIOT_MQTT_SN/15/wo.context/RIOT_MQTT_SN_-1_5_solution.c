#include <xtimer.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <emcute.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    (1883)
#define TOPIC_NAME          "device/state"
#define PUBLISH_INTERVAL    (5U)

static emcute_topic_t mqtt_topic;
static char message[128];

static void publish_state(const char* state) {
    snprintf(message, sizeof(message), "state: %s", state);
    while (1) {
        int res = emcute_pub(&mqtt_topic, message, strlen(message), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Message published: %s\n", message);
            break;
        }
        else {
            printf("Failed to publish message, retrying...\n");
            xtimer_sleep(1);
        }
    }
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: Unable to parse gateway address\n");
        return 1;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
            continue;
        }

        mqtt_topic.name = TOPIC_NAME;
        if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
            printf("Failed to register topic\n");
            xtimer_sleep(1);
            continue;
        }

        while (1) {
            publish_state("work");
            xtimer_sleep(PUBLISH_INTERVAL);
            publish_state("not work");
            xtimer_sleep(PUBLISH_INTERVAL);
        }
    }

    return 0;
}