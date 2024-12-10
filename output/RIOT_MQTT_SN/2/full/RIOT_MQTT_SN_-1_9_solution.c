#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mqttclient.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <msg.h>
#include <assert.h>
#include <mutex.h>

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (1U)
#define TOPIC_MAXLEN    (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topicsubs[NUMOFSUBS][TOPIC_MAXLEN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    printf("%.*s\n", (int)len, in);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, "esp32client");

    emcute_topic_t tsub;
    tsub.name = "device/state";
    emcute_reg(&tsub);

    printf("Publishing to topic %s\n", tsub.name);

    char *message = "work";

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            res = emcute_pub(&tsub, message, strlen(message), EMCUTE_QOS_0);
            if (res != EMCUTE_OK) {
                printf("Publish failed\n");
            }
            emcute_discon();
        }
        else {
            printf("Connection failed\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}