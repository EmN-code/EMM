#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define EMCUTE_ID        ("device_id")
#define EMCUTE_PORT      (1883U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

void *mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);

    emcute_topic_t topic;
    topic.name = "state";
    printf("Registering topic \"%s\"...\n", topic.name);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic
");
        return NULL;
    }

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway
");
        return NULL;
    }

    while (1) {
        const char *pub_data = "work or not work";
        if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data
");
        } else {
            puts("Published: "pub_data"");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT-SN example\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}