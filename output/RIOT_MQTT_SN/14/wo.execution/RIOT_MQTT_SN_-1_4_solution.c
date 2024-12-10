#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

/**
 * Configuration
 */
#define MQTT_SN_BROKER_IP   "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define PUB_INTERVAL_SEC    5

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];
static char *message = "work or not work";

void *emcute_thread(void *arg) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    printf("[MQTT-SN] Connecting to gateway at [%s]:%d...\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[MQTT-SN] Failed to connect to gateway\n");
        return NULL;
    }

    printf("[MQTT-SN] Connected\n");

    strcpy(topic.name, "state/update");
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[MQTT-SN] Unable to register topic\n");
        return NULL;
    }

    printf("[MQTT-SN] Registered publish topic '%s'\n", topic.name);

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[MQTT-SN] Error: unable to publish data\n");
        } else {
            printf("[MQTT-SN] Published message: %s\n", message);
        }
        xtimer_sleep(PUB_INTERVAL_SEC);
    }

    return NULL;
}

int main(void) {
    emcute_run(MQTT_SN_BROKER_PORT, EMCUTE_ID);
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    return 0;
}