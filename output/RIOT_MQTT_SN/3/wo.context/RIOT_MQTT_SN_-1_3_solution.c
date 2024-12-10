#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <string.h>

#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define PUBLISH_INTERVAL   (5U)
#define MQTT_TOPIC         "device/state"
#define MQTT_QOS           EMCUTE_QOS_0
#define MQTT_RETENTION     1

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  // Unused
    (void)data;   // Unused
    (void)len;    // Unused
    // This callback is a stub. Implement handling of incoming messages here.
}

static void reconnect(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    gw.port = MQTT_BROKER_PORT;
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic %s\n", MQTT_TOPIC);
        return;
    }
}

static void *thread_handler(void *arg) {
    (void)arg;
    reconnect();  // Connect to the broker initially

    while (1) {
        char *message = "work";  // Placeholder for device state

        if (emcute_pub(&topic, message, strlen(message), MQTT_QOS | (MQTT_RETENTION ? EMCUTE_RETAIN : 0)) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
            reconnect();  // Attempt to reconnect if publish fails
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize the emcute instance, ID can be anything unique per client
    emcute_run(stack, sizeof(stack), "example" );

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "mqtt")

    return 0;
}