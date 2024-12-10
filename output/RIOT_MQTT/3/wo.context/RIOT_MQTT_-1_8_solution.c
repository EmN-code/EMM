#include <xtimer.h>
#include <string.h>
#include "net/emcute.h"
#include "shell.h"
#include "MQTTClient.h"

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    1883
#define MQTT_TOPIC     "esp32/status"
#define PUBLISH_INTERVAL 5U * US_PER_SEC

static emcute_topic_t my_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

typedef struct {
    Network network;
    MQTTClient client;
} mqtt_context_t;

static mqtt_context_t ctx;

static void on_message(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic; (void)data; (void)len;
    printf("Message received on topic %s\n", topic->name);
}

static void publish_status(mqtt_context_t *ctx, const char *message) {
    emcute_pub(&my_topic, message, strlen(message), EMCUTE_QOS_0);
    printf("Published: %s\n", message);
}

static void *mqtt_thread_func(void *arg) {
    mqtt_context_t *ctx = (mqtt_context_t *)arg;
    (void)ctx;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_IP);
    emcute_con(&&gw, "mqtt", EMCUTE_ID);

    my_topic.name = MQTT_TOPIC;
    my_topic.id = 0;
    emcute_reg(&my_topic);

    bool work = true;
    while (1) {
        publish_status(ctx, work ? "work" : "not work");
        work = !work;
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    mqtt_context_t *ctx = &ctx;

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread_func, ctx, "mqtt");

    return 0;
}