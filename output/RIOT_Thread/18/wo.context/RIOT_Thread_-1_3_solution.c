#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 10
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];
msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;  // Unused argument
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    
    while(1) {
        msg_send(&msg, (kernel_pid_t)arg);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;  // Unused argument
    msg_init_queue(msg_queue, 8);  
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE_DEFAULT,
                                               RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                               receiver_thread, NULL, "receiver_thread");
    
    thread_create(sender_stack, THREAD_STACKSIZE_DEFAULT,
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, (void *)receiver_pid, "sender_thread");

    return 0;
}
