#include thread.h
#include msg.h
#include kernel_defines.h

#define MAIN_THREAD_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define SENDER_THREAD_PRIO     (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_THREAD_PRIO   (THREAD_PRIORITY_MAIN - 3)

#define STACK_SIZE             (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE         (8)

static msg_t message_queue[MSG_QUEUE_SIZE];

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

void *sender(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    msg_t msg;
    msg_init_queue(message_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Message received with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACK_SIZE, RECEIVER_THREAD_PRIO, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, STACK_SIZE, SENDER_THREAD_PRIO, 0, sender, &receiver_pid, "sender");
    return 0;
}