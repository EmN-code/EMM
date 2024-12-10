#include "stdlib.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"
#include "MQTTClient.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "dht_params.h"

#define EMCUTE_PORT 1883

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

enum { WORK, NOT_WORK };

typedef struct {
    sock_udp_ep_t gw;
    emcute_topic_t t;
} mqtt_sn_client_t;

#define STATE_TOPIC_NAME "state"

void *mqtt_sn_thread(void *arg) {
    mqtt_sn_client_t *client = (mqtt_sn_client_t *)arg;

    client->t.name = STATE_TOPIC_NAME;
    if (emcute_reg(&(client->t)) != EMCUTE_OK) {
        puts("Failed to register topic");
        return NULL;
    }

    unsigned flags = EMCUTE_QOS_0;
    char message[11];  // max length "not work"

    while (1) {
        strcpy(message, ((rand() % 2) == WORK) ? "work" : "not work");
        if (emcute_pub(&(client->t), message, strlen(message), flags) != EMCUTE_OK) {
            puts("Failed to publish");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

static mqtt_sn_client_t initialize_client() {
    mqtt_sn_client_t client;
    memset(&client, 0, sizeof(client));
    client.gw.port = EMCUTE_PORT;

    inet_pton(AF_INET, "47.102.103.1", &client.gw.addr.ipv4);

    client.gw.family = AF_INET;
    emcute_con(&client.gw, true, NULL, NULL, 0, 0);

    return client;
}

int main(void) {
    msg_init_queue(queue, 8);

    mqtt_sn_client_t client = initialize_client();

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_sn_thread, &client, "mqtt_sn_thread");

    return 0;
}