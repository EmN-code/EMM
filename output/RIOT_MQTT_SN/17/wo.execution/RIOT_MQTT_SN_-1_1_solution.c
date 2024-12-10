#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include "mqttsn_publisher.h"
#include "net/emcute.h"
#include "shell.h"

#define EMCUTE_ID       "esp32"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT     (1883)
#define EMCUTE_ADDRESS  "47.102.103.1"
#define PUB_TOPIC_NAME  "state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char* pub_messages[] = {"work", "not work"};

static void *publisher_thread(void *arg) {
    (void)arg;
    int counter = 0;
    emcute_sub_t subscription;
    subscription.cb = NULL;

    /* get topic id */
    topic.name = PUB_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[ERROR] Unable to obtain Topic ID\n");
        return NULL;
    }

    while (1) {
        /* publish work state */
        if (emcute_pub(&topic, pub_messages[counter], strlen(pub_messages[counter]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[ERROR] Unable to publish data\n");
        }
        else {
            printf("Published: %s\n", pub_messages[counter]);
        }

        /* alternate message */
        counter = (counter + 1) % 2;

        /* wait 5 seconds */
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    /* Initialize the emcute network stack */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher_thread, NULL, "publisher");

    /* Connect to the MQTT-SN broker */
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr, EMCUTE_ADDRESS);

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[ERROR] unable to connect to gateway\n");
        return 1;
    }

    /* Start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}