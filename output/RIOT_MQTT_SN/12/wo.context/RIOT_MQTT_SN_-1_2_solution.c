#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <periph/gpio.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define STATE_INTERVAL  (5U)

static emcute_topic_t state_topic;
static char state_message[64];

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received message on topic %s: %.*s\n", topic->name, (int)len, (char *)data);
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, EMCUTE_PORT);
        return 1;
    }

    return 0;
}

static int publish_state(const char *state) {
    state_topic.name = "device/state";
    state_topic.id = 0;  // Let the broker assign it

    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    size_t len = snprintf(state_message, sizeof(state_message), "%s", state);

    if (emcute_pub(&state_topic, state_message, len, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
        return 1;
    }

    return 0;
}

int main(void) {
    puts("MQTT-SN Publisher for ESP32");

    emcute_run(EMCUTE_PORT, "device");

    if (connect_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_SEC, STATE_INTERVAL);
        publish_state("not work");
        ztimer_sleep(ZTIMER_SEC, STATE_INTERVAL);
    }

    return 0;
}