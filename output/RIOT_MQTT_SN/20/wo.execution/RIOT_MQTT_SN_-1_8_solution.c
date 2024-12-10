#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mqtt_client.h>
#include <mutex.h>
#include <paho_mqtt.h>
#include <snprintf.h>
#include <sys/socket.h>

define EMCUTE_PORT           (1883)
define EMCUTE_ID             ("my_client_id")
define SERVER_ADDRESS        ("47.102.103.1")
define TOPIC_NAME            ("state")

typedef struct {
    char *name;
    unsigned id;
} emcute_topic_t;

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    static emcute_topic_t state_topic;
    state_topic.name = TOPIC_NAME;

    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};

    if (inet_pton(AF_INET, SERVER_ADDRESS, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse gateway address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&state_topic, message, strlen(message), 0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
        ztimer_sleep(ZTIMER_USEC, 5000000ULL);  // sleep for 5 seconds
    }

    return 0;
}