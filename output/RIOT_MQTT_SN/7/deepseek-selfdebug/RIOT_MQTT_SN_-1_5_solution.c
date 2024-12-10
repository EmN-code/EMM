
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define MQTT_THREAD_PRIORITY            (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE           (THREAD_STACKSIZE_LARGE)
#define EMCUTE_TOPIC_MAXLEN             (32U)
#define EMCUTE_BUFSIZE                  (128U)
#define PUBLISH_INTERVAL                (5 * 1000000U) // 5 seconds in microseconds

static char mqtt_stack[MQTT_THREAD_STACKSIZE];
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
static emcute_topic_t topic;

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t remote = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return NULL;
    }

    const char *topic_name = "state";
    topic.name = topic_name;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *message = "work";
        res = emcute_pub(&topic, message, strlen(message), 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    thread_create(mqtt_stack, sizeof(mqtt_stack), MQTT_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}
