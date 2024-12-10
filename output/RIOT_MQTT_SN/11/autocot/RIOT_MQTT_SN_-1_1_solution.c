#include <string.h> // For memset and other string functions
#include <stdio.h> // For printf
#include "net/emcute.h" // For emcute functionalities and data structures
#include "kernel.h" // For thread_sleep
#include "xtimer.h" // For xtimer sleep function

#define EMQX_SERVER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define STATE_TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC) // Publish every 5 seconds

static emcute_topic_t state_topic;

static emcute_topic_t *topics[] = {
    &state_topic,
};

static char mqtt_buf[128]; // Buffer for MQTT messages

void *publish_thread(void *arg) {
    (void)arg;
    const char *state = "work";
    emcute_topic_t *t = &state_topic;
    t->name = STATE_TOPIC;

    while (1) {
        snprintf(mqtt_buf, sizeof(mqtt_buf), "{\"state\": \"%s\"}", state);
        if (emcute_pub(t->name, mqtt_buf, strlen(mqtt_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish to topic '%s'\n", t->name);
        }
        xtimer_sleep(5);
        // Switch state
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }

    return NULL;
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_PORT};
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", EMQX_SERVER_IP, MQTT_PORT);
        return 1;
    }
    printf("Connected to MQTT-SN broker at [%s]:%d\n", EMQX_SERVER_IP, MQTT_PORT);

    return 0;
}

int main(void) {
    state_topic.name = STATE_TOPIC;
    if (mqtt_connect() != 0) {
        printf("Error: connection to MQTT-SN broker failed\n");
        return 1;
    }

    kernel_pid_t pub_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, publish_thread, NULL, "publish_thread");
    thread_yield_higher();

    return 0;
}