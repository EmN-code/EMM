#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mutex.h>

#define EMCUTE_ID           (0U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_MAXLEN        (64U)
#define NUMOFSUBS           (16U)
#define MQTT_TOPIC          "state"
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PAYLOAD             "work"
#define INTERVAL            (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;
static int pub_state(void)
{
    /* setup publication */
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    memset(&t, 0, sizeof(t));
    strncpy(t.name, MQTT_TOPIC, TOPIC_MAXLEN);

    int res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        printf("error: unable to obtain topic ID (%d)\n", res);
        return 1;
    }

    while (true) {
        res = emcute_pub(&t, PAYLOAD, strlen(PAYLOAD), flags);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data (%d)\n", res);
        } else {
            printf("Published: %s\n", (char *)PAYLOAD);
        }
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}

static int connect_broker(void)
{
    sock_udp_ep_t gw;
    memset(&gw, 0, sizeof(gw));

    /* Set the address and port for the target broker */
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("error: unable to parse IPv4 address\n");
        return -1;
    }
    gw.port = MQTT_BROKER_PORT;

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Successfully connected to gateway at %s\n", MQTT_BROKER_IP);
            break;
        } else {
            printf("error: unable to connect to [%s]:%d (%d)\n", MQTT_BROKER_IP, MQTT_BROKER_PORT, res);
            printf("Retrying in 1 second\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }
    return 0;
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Initialize emcute */
    emcute_run(EMCUTE_PRIO, stack, sizeof(stack), EMCUTE_ID);

    /* Connect to the broker */
    if (connect_broker() != 0) {
        return -1;
    }

    /* Publish state every INTERVAL seconds */
    pub_state();

    /* Should never be reached */
    return 0;
}
