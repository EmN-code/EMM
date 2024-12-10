#include "thread.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define CLIENT_ID           "esp32-client"
#define TOPIC_NAME          "device/state"
#define SEND_INTERVAL       (5U * US_PER_SEC)   /* 5 seconds */

static char stack[THREAD_STACKSIZE_MAIN];
static void mqttsn_publisher(void);

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)mqttsn_publisher,
                  NULL, "mqttsn_publisher");
    return 0;
}

static void mqttsn_publisher(void) {
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

    /* Initialize emCute */
    emcute_init();
    
    /* Connect to the MQTT-SN broker */
    gw.addr.ipv6[0] = 0x1021;
    gw.addr.ipv6[1] = 0x3021;
    gw.addr.ipv6[2] = 0x0;
    gw.addr.ipv6[3] = 0x1;
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return; /* Handle connection error */
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        emcute_discon();
        return; /* Handle registration error */
    }

    while (1) {
        /* Publish message */
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            break; /* Handle publication error */
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    /* Disconnect from the broker */
    emcute_discon();
}