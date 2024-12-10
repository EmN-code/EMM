#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mqtt_client.h> // Ensure the mqtt client header

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MAX_TOPIC_LENGTH 64
#define MAIN_QUEUE_SIZE (4)

static kernel_pid_t main_pid;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    main_pid = thread_getpid();

    while (1) {
        char message[20] = "work"; // example message
        emcute_sub_t stack[TOPIC_MAXLEN];

        memset(stack, 0, sizeof(stack));

        /* Create the topic name */
        snprintf(stack, sizeof(stack), "state/0001");

        /* Publish message */
        emcute_pub(stack, message, strlen(message), EMCUTE_QOS_1);

        /* Sleep for 5 seconds */
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("MQTT client started\n");

    /* Start MQTT client thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}