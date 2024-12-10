#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_ID     ("my_emcute")
#define EMCUTE_PORT   (1883)
#define MQTT_TOPIC    ("state")
#define PUBLISH_DATA  ("work")
#define SERVER_ADDR   "2001:db8::1"

static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication: %.*s\n", (int)len, (char *)data);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDR);

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = MQTT_TOPIC;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Unable to register topic %s\n", t.name);
        return 1;
    }

    while (1) {
        if (emcute_pub(&t, PUBLISH_DATA, strlen(PUBLISH_DATA), flags) != EMCUTE_OK) {
            printf("Unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}