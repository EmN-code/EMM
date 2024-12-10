#include <ztimer.h>
#include <emcute.h>
#include <stdio.h>
#include <string.h>

#define EMCUTE_PORT         (1883U)
#define MQTT_TOPIC_NAME     "state"
#define MQTT_BROKER_IP      "47.102.103.1"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication on topic %s [%i] with data: %s\n",
           topic->name, topic->id, (char *)data);
}

int main(void) {
    /* Initialize EMCUTE */
    memset(stack, 0, sizeof(stack));
    emcute_run(EMCUTE_PORT, "your_client_id");

    /* Configure the server address */
    sock_udp_ep_t gw = { .family = AF_INET6, /* corrected to AF_INET */
                         .port = EMCUTE_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);  // Use ipv4 instead of ipv6

    /* Connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway.\n");
        return 1;
    }

    /* Set up topic and subscribe */
    topic.name = MQTT_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.\n");
        return 1;
    }
    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic.\n");
        return 1;
    }

    while (1) {
        char *msg = "work";  // Or "not work" depending on your state logic
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_1) != EMCUTE_OK) {
            puts("Error: unable to publish data.\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Use ztimer_sleep to wait 5 seconds
    }
    return 0;
}
