#include <ztimer.h>
#include <stddef.h>
#include <mutex.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <msg.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <assert.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>

#define MAIN_QUEUE_SIZE     (8)
#define STATE_MSG           "work"
#define EMCUTE_ID           "mqttsn"
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         (1883)
#define PUBLISH_INTERVAL_MS (5000U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MAIN_QUEUE_SIZE];

int main(void) {
    msg_init_queue(queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

    inet_pton(AF_INET, SERVER_ADDR, &gw.addr);

    emcute_topic_t topic;
    strcpy(topic.name, STATE_MSG);

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            puts("Error connecting to broker");
            continue;
        }

        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Error registering topic");
            emcute_discon();
            continue;
        }

        if (emcute_pub(&topic, STATE_MSG, strlen(STATE_MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        }

        emcute_discon();

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL_MS);
    }

    return 0;
}
