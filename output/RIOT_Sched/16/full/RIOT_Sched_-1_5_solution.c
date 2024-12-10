#include "paho_mqtt.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "architecture.h"
#include "thread.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "kernel.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread %s running\n", (char *)arg);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_id, thread2_id;
    char *thread_1_name = "thread_1";
    char *thread_2_name = "thread_2";

    thread1_id = thread_create(stack1, sizeof(stack1), PRIORITY_MAIN,
                               THREAD_CREATE_STACKTEST, thread_handler,
                               (void *)thread_1_name, thread_1_name);

    thread2_id = thread_create(stack2, sizeof(stack2), PRIORITY_SECOND,
                               THREAD_CREATE_STACKTEST, thread_handler,
                               (void *)thread_2_name, thread_2_name);

    while (1) {
        if (thread1_id != KERNEL_PID_UNDEF && thread2_id != KERNEL_PID_UNDEF) {
            uint8_t prio1 = thread_get(thread1_id)->priority;
            uint8_t prio2 = thread_get(thread2_id)->priority;

            if (prio1 < prio2) {
                printf("Increasing priority of %s\n", thread_1_name);
                priority_set(thread1_id, prio2 + 1);
            }
            else if (prio2 < prio1) {
                printf("Increasing priority of %s\n", thread_2_name);
                priority_set(thread2_id, prio1 + 1);
            }
        }

        xtimer_sleep(5);
    }
    return 0;
}