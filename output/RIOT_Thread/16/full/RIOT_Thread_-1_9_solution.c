#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_DEFAULT (1024)

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

#define MESSAGE_QUEUE_SIZE (4)

msg_t message_queue[MESSAGE_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 100; // Assign a value to the content
    kernel_pid_t receiver_pid = thread_getpid() - 1;

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent: %d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    while (1) {
        int rc;
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Network connected successfully.\n");
        break;
    }

    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    while (1) {
        xtimer_sleep(1);  // Main thread sleep indefinitely
    }
    return 0;
}