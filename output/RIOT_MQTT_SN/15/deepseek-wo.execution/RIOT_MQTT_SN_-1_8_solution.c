
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <dht_params.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 10

static msg_t msg_queue[MSG_QUEUE_SIZE];

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    const char *state = states[state_index];
    emcute_pub(&topic, state, strlen(state), 0);

    state_index = (state_index + 1) % 2;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    emcute_con(&remote, true, NULL, NULL, 0, 0);

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
