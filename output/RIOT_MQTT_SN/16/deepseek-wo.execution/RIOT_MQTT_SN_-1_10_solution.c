
#include <net/emcute.h>
#include <net/af.h>
#include <net/conn/udp.h>
#include <net/netif.h>
#include <shell.h>
#include <xtimer.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <thread.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <time_units.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>

#define TOPIC "state"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % ARRAY_SIZE(states);

    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(BROKER_PORT) };
    inet_aton(BROKER_ADDR, &gw.addr.ipv4);

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
