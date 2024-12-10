#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "time_units.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "mqttsn_publisher.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "dht.h"
#include "msg.h"
#include "unistd.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "stdatomic.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define MQTT_TOPIC_MAXLEN   (64U)
#define NUMOFSUBS           (16U)
#define INTERVAL            (5000000U) /* 5 seconds in microseconds */
#define STATE_RUNNING       "work"
#define STATE_IDLE          "not work"
#define MSG_QUEUE_SIZE      (8U)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "esp32-client");
    return NULL; /* never reached */
}

int main(void) {
    /* initialize message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* initialize thread for emcute */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* setup gateway address */
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    /* set the topic we want to publish */
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic \"%s\"\n", topic.name);
        return 1;
    }

    while (1) {
        const char *state = (esp_system_state() == ESP_SYSTEM_STATE_ON) ? STATE_RUNNING : STATE_IDLE;

        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        ztimer_sleep(ZTIMER_MSEC, INTERVAL/1000); 
    }

    return 0;
}