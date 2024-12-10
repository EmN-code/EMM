
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <dht_params.h>
#include <stdio.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define MSG_TYPE_PUB (0x01)

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

static void *pub_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_PUB) {
            const char *state = (msg.content.ptr == 0) ? "not work" : "work";
            emcute_pub(&topic, state, strlen(state), 0);
        }
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t gw = { .port = HTONS(1883) };
    inet_aton("47.102.103.1", (ipv4_addr_t *)&gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    thread_t *pub_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        pub_thread,
        NULL,
        "publisher");

    while (1) {
        msg_t msg;
        msg.type = MSG_TYPE_PUB;
        msg.content.ptr = (void *)1;
        msg_send(&msg, pub_pid);
        xtimer_sleep(5);
    }

    return 0;
}
