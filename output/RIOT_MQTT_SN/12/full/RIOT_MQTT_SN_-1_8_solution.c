#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/emcute.h>
#include <msg.h>
#include <arpa/inet.h>

#define EMCUTE_ID       "gertrud"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define NUMOF_MSGS      (8U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[NUMOF_MSGS];

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static emcute_topic_t topic;
static char *topic_name = "topic_work_status";

void *mqtt_thread(void *arg) {
    (void)arg;

    emcute_topic_t t;
    t.name = topic_name;

    while (1) {
        int conn_attempts = 0;
        while (emcute_con((sock_udp_ep_t *)arg, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            if (conn_attempts++ > 5) {
                printf("Failed to connect to MQTT broker after 5 attempts.\n");
                return NULL;
            }
            printf("Retrying connection in 1 second...\n");
            sleep(1);
        }

        while (emcute_reg(&t) != EMCUTE_OK) {
            printf("Failed to register topic. Retrying in 1 second...\n");
            sleep(1);
        }

        while (1) {
            const char *pub_data = "work";
            if (emcute_pub(&t, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
                printf("Failed to publish data.\n");
                break;
            }
            printf("Published data: %s\n", pub_data);
            sleep(5);
        }

        emcute_discon();
        printf("Disconnected from broker, will retry in 1 second\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, NUMOF_MSGS);

    /* initialize our MQTT-SN stack */
    emcute_setup(stack, sizeof(stack), EMCUTE_PRIO);

    /* prepare target address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    /* start the MQTT thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, &gw, "mqtt_thread");

    return 0;
}