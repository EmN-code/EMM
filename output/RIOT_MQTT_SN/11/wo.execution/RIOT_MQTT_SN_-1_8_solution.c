#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_TOPIC          "state"
#define MQTT_MSG            "work"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, "mqttsn-client");
    return NULL;
}

int main(void)
{
    /* initialize the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    
    /* declare and connect to a gateway */
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };    
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    /* setup the subscription */
    strncpy(topic.name, MQTT_TOPIC, TOPIC_MAXLEN);
    emcute_reg(&topic);

    /* run the main loop */
    while (1) {
        emcute_pub(&topic, MQTT_MSG, strlen(MQTT_MSG), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}