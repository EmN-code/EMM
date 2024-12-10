#include <stdio.h>
#include <xtimer.h>
#include <timex.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "work_status"
#define CLIENT_ID "mqtt-sn-client-esp32"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PORT (1883U)

static char _stack[STACK_SIZE];
static msg_t _queue[8];

static emcute_sub_t subscriptions[1];
static char status[16];
static bool work = true;

static void *mqtt_thread(void *arg) {
    (void) arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* setup our own subscription */
    t.name = TOPIC_NAME;
    t.id = 0; /* will be automatically assigned by the broker */

    while (1) {
        /* alternate the work status */
        work = !work;
        snprintf(status, sizeof(status), "work: %s", work ? "yes" : "no");

        /* connect to broker */
        sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
        ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "::ffff:" BROKER_IP);

        if (emcute_con(&gw, true, CLIENT_ID, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("error: unable to connect to [%s]:%i\n", BROKER_IP, gw.port);
            return NULL;
        }

        /* publish our status message */
        if (emcute_pub(&t, status, strlen(status), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }

        /* disconnect from broker */
        emcute_discon();

        /* wait 5 seconds before publishing next status update */
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_queue, ARRAY_SIZE(_queue));
    thread_create(_stack, sizeof(_stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    /* start the connection to the broker */
    puts("MQTT-SN client started\n");

    return 0;
}