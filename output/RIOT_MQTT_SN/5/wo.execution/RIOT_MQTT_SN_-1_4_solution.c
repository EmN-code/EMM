#include <xtimer.h>
#include <ztimer.h>
#include <thread.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <timer.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <timex.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <assert.h>
#include <time_units.h>

#define EMCUTE_ID           "RIOT_MQTT"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "status"
#define BROKER_IP           "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

void publish_state(void) {
    const char *message = "work or not work";
    int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error publishing: %d\n", res);
    }
}

void *publish_thread(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = TOPIC_NAME;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Not able to connect to broker.\n");
        return 1;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic." );
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_thread, NULL, "publish_thread");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);  // Blocking call

    return 0;
}