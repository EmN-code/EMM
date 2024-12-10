#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (8U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t emcute_thread;
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_PORT, client_id);
    return NULL;
}

int main(void) {
    sock_udp_ep_t gateway = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gateway.addr.ipv4);

    emcute_topic_t topic;
    size_t len;
    const char *msg = "work";

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    printf("Connecting to gateway at %s:%d...\n", "47.102.103.1", (int)gateway.port);

    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* start emcute thread */
    pthread_create(&emcute_thread, NULL, mqtt_thread, NULL);

    /* setup the topic we want to subscribe/publish to */
    strcpy(topics[0], "0212/work");
    topic.name = topics[0];

    while (1) {
        /* publish the data every 5 seconds */
        len = sizeof(msg);
        if (emcute_pub(&topic, msg, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        sleep(5);
    }

    return 0;
}