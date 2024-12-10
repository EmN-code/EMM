#include <xtimer.h>  // Include required for timer functionalities
#include <thread.h>  // Required for thread functionalities
#include <msg.h>     // Required for message functionalities

#define MAIN_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define SECOND_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 3)
#define STACKSIZE_MAIN_THREAD  (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_SECOND_THREAD (THREAD_STACKSIZE_DEFAULT)

static char stack1[STACKSIZE_MAIN_THREAD];
static char stack2[STACKSIZE_SECOND_THREAD];

msg_t msg_queue[8]; // Define a message queue

void *main_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    msg.type = 0x2222;

    while(1) {
        msg_receive(&msg);  // Receive message
        printf("Main thread received a message with type: %ld\n", msg.type);
    }

    return NULL;
}

void *second_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t main_pid = thread_getpid_of(thread_get(1));

    msg.type = 0x2222;

    while(1) {
        msg_send(&msg, main_pid);  // Send message
        printf("Second thread sent a message\n");
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t main_tid = thread_create(stack1, STACKSIZE_MAIN_THREAD,
                                          MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                          main_thread, NULL, "main_thread");
    
    kernel_pid_t second_tid = thread_create(stack2, STACKSIZE_SECOND_THREAD,
                                            SECOND_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                            second_thread, NULL, "second_thread");

    (void)main_tid;
    (void)second_tid;
    return 0;
}