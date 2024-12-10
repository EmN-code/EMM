#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_ID          "mqttsn-client"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define TOPIC_NAME         "state"
#define PUB_INTERVAL       (5LU * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static char buffer[64];
static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic '%s' with payload length %zu\n", topic->name, len);
}

int main(void) {
    printf("Starting MQTT-SN client\n");

    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to MQTT-SN broker */
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = MQTT_BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_IP);

    if (emcute_con(&gw) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        return 1;
    }

    /* Set topic information */
    topic.name = TOPIC_NAME;

    /* Publish every 5 seconds */
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "work");

        if (emcute_pub(&topic, buffer, strlen(buffer), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data
");
        } else {
            on_publish(&topic, buffer, strlen(buffer));
        }

        xtimer_sleep(5);
    }

    /* Disconnect from the broker */
    emcute_discon();
    return 0;
}
