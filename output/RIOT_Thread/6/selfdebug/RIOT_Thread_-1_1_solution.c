#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN (1)
#define THREAD_PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN + 1)

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *secondary_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg)) {
            printf("Secondary thread received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    kernel_pid_t secondary_thread_pid = 
        thread_create(thread_stack, sizeof(thread_stack),
                      THREAD_PRIORITY_SECONDARY, 0,
                      secondary_thread, NULL, "secondary_thread");

    printf("Main thread: Secondary thread started with PID %d\n", secondary_thread_pid);

    msg_t msg;
    msg.type = 0;
    for (int i = 0; i < 5; ++i) {
        msg.content.value = i;
        msg_send(&msg, secondary_thread_pid);
        xtimer_sleep(1);
    }

    return 0;
}