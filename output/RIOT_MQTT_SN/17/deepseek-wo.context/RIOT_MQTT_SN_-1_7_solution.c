
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";

static void on_publish(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received publish on topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    emcute_sub_t sub;
    sub.topic = topic;
    sub.cb = on_publish;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("State published");
        }

        xtimer_periodic_wakeup(&last_wakeup, MSG_INTERVAL);
    }

    return 0;
}
