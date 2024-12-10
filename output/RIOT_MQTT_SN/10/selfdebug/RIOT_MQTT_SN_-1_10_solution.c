#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>
#include <errno.h>

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static const char *topic_name = "esp32/state";
static const sock_udp_ep_t emcute_remote = {{.ipv4 = {47, 102, 103, 1}}, .port = 1883};

void *emcute_thread(void *arg) {
    emcute_run(&emcute_remote, "esp32_client", stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

int main(void) {
    int res;
    char message[] = "work";

    // Connect to MQTT-SN broker
    res = emcute_con((sock_udp_ep_t *)&emcute_remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect [%d] \n", res);
        return 1;
    }

    // Get topic ID
    topic.name = topic_name;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic [%d] \n", res);
        return 1;
    }

    while (1) {
        res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish [%d] \n", res);
        } else {
            printf("Published: %s \n", message);
        }

        // Sleep for 5 seconds before re-publishing
        ztimer_sleep(ZTIMER_MSEC, 5000);

        // Toggle state message
        if (strcmp(message, "work") == 0) {
            strcpy(message, "not work");
        } else {
            strcpy(message, "work");
        }
    }

    return 0;
}