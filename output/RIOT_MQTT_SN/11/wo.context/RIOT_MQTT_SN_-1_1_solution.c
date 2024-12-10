#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID           "mqtt-client"
#define EMCUTE_PORT         1883
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SERVER_ADDR         "47.102.103.1"
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;
static char *mqtt_packet_topic = "state";
static char *work_state_msg = "work";
static char *not_work_state_msg = "not work";

static void *mqtt_thread(void *arg) {
    (void)arg;
    xtimer_ticks32_t last = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last, PUBLISH_INTERVAL);
        
        const char *msg = (last.ticks32 % (2 * US_PER_SEC) == 0) ? work_state_msg : not_work_state_msg;  // Toggle state

        if (emcute_publish(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish msg \"%s\"\n", msg);
        } else {
            printf("Published: %s\n", msg);
        }
    }
    return NULL;
}

static int mqtt_start(void) {
    int res;
    kernel_pid_t tid;
    static char stack[THREAD_STACKSIZE_MAIN];

    res = emcute_netif_set_config_tio(EMCUTE_ID, EMCUTE_PORT, true, NULL);
    if (res != EMCUTE_OK) {
        printf("Error: unable to configure network interface and connect EMCUTE\n");
        return res;
    }

    topic.name = mqtt_packet_topic;
    topic.id = 0;

    if ((tid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                             mqtt_thread, NULL, "mqtt")) <= KERNEL_PID_UNDEF) {
        puts("thread_create() failed\n");
        return 1;
    }
    
    return 0;
}

int main(void) {
    msg_init_queue(_recv_queue, MSG_QUEUE_SIZE);
    xtimer_sleep(3); // Give RIOT time to be ready

    if (mqtt_start() != 0) {
        puts("Error: unable to start MQTT client\n");
        return 1;
    }
    return 0;
}