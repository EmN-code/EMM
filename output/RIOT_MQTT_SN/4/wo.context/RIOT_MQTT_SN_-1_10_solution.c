#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <emcute.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "esp32_mqtt_client"
#define EMCUTE_TOPIC    "device/state"
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUBLISH_INTERVAL (5U)

static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Message received on topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    /* setup gateway address */
    ipv6_addr_from_str((ipv6_addr_t *) &gw.addr, MQTT_BROKER_IP);

    /* initialize emcute */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    printf("Connecting to MQTT-SN broker at [%s]:%u\n", MQTT_BROKER_IP, EMCUTE_PORT);

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }
    puts("successfully connected to gateway");

    /* prepare and register subscription */
    emcute_topic_t t = { .name = EMCUTE_TOPIC, .id = 0 }; /* will be assigned a valid id by emcute_reg() */
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    while (1) {
        const char *state = "working";
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish message");
        }
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * MS_PER_SEC);
    }

    return 0;
}