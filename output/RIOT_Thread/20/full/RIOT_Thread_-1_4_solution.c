#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread2_pid;

void *thread1_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42;  // Send some value
        msg_send(&msg, thread2_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                              THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");
    
    // Error handling demonstration for a hypothetical NetworkConnect call
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0) {
            printf("connect failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("connect successfully\n");
            break;
        }
    }

    return 0;
}