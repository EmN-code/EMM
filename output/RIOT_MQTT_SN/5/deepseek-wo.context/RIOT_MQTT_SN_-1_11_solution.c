
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_BUFSIZE         (128U)
#define EMCUTE_TOPIC_MAXLEN    (32U)
#define MQTT_SERVER_IP         "47.102.103.1"
#define MQTT_SERVER_PORT       1883
#define MQTT_CLIENT_ID         "esp32_client"
#define MQTT_TOPIC             "state"
#define MQTT_MESSAGE           "work or not work"

static char emcute_buf[EMCUTE_BUFSIZE];

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SERVER_PORT };
    emcute_topic_t t;

    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, MQTT_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    /* Initialize the emcute thread */
    emcute_run(MQTT_SERVER_PORT, MQTT_CLIENT_ID);

    /* Register the topic */
    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    /* Subscribe to the topic */
    emcute_sub_t sub;
    sub.topic = t;
    sub.cb = _on_pub;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&t, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("State published");
        }
        xtimer_sleep(5);
    }

    return 0;
}
