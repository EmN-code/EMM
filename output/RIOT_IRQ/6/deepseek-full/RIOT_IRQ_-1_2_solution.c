
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define STACKSIZE (1024)

static char thread_stack[STACKSIZE];
static volatile int interrupt_flag = 0;

static void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_flag = 1;
}

static void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (interrupt_flag) {
            printf("Interrupt handled!\n");
            interrupt_flag = 0;
        }
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(thread_stack,
                                    STACKSIZE,
                                    THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST,
                                    thread_func,
                                    NULL,
                                    "interrupt_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    irq_set_handler(0, interrupt_handler, NULL);

    while (1) {
        thread_yield();
    }

    return 0;
}
