#include <thread.h>
#include <string.h>
#include <stdio.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT        1883
#define MQTT_BROKER_IP    "47.102.103.1"
#define STATE_TOPIC        "device/state"
#define MAIN_QUEUE_SIZE    (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static emcute_topic_t state_topic = {
    .name = STATE_TOPIC,
    .id = 0
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *str = (char *)data;
    str[len] = '\0';
    printf("Received publication on topic '%s': %s\n", topic->name, str);
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    puts("Connecting to gateway...\n");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
        puts("Successfully connected to gateway.\n");
    }
    else {
        puts("Failed to connect to gateway.\n");
        return 1;
    }

    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Could not register topic\n");
        return 1;
    }

    while (1) {
        const char *message = "work"; // Replace with your logic for state.
        if (emcute_pub(&state_topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Published: "%s" on topic "%s"\n", message, state_topic.name);
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
