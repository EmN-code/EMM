#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT 1883
#define SERVER_ADDR "47.102.103.1"

static emcute_topic_t state_topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr) != 1) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", SERVER_ADDR, EMCUTE_PORT);
        return NULL;
    }

    state_topic.name = "state";
    emcute_reg(&state_topic);

    while (1) {
        const char *msg = "work";
        res = emcute_pub(&state_topic, msg, strlen(msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    static char mqtt_stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t mqtt_pid = thread_create(mqtt_stack, sizeof(mqtt_stack),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          mqtt_thread, NULL, "mqtt_thread");
    emcute_run(EMCUTE_PORT, "device_id");
    
    return 0;
}