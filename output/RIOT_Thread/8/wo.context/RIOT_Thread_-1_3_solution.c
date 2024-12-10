#include <xtimer.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message of type %u with value %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

void send_message(kernel_pid_t receiver_pid, uint16_t type, uint32_t value) {
    msg_t msg;
    msg.type = type;
    msg.content.value = value;
    msg_send(&msg, receiver_pid);
}

int main(void)
{
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, /* Priority */
                                            THREAD_CREATE_STACKTEST, /* Flags */
                                            thread_handler, /* Function */
                                            NULL, /* Argument */
                                            "message_receiver_thread" /* Thread name */);

    while (1) {
        send_message(thread_pid, 1, 42);
        xtimer_sleep(2);
    }

    return 0;
}