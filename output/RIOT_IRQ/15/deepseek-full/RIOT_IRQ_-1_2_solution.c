
#include <stdio.h>
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[STACKSIZE];
static volatile int interrupt_flag = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt detected!\n");
    interrupt_flag = 1;
}

void thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (interrupt_flag) {
            printf("Handling interrupt...\n");
            interrupt_flag = 0;
            printf("Interrupt handled.\n");
        }
        thread_yield();
    }
}

int main(void)
{
    puts("RIOT interrupt handling example");

    kernel_pid_t pid = thread_create(thread_stack,
                                     sizeof(thread_stack),
                                     PRIORITY,
                                     THREAD_CREATE_STACKTEST,
                                     thread_func,
                                     NULL,
                                     "interrupt_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread");
        return 1;
    }

    irq_set_handler(0, interrupt_handler, NULL);
    irq_enable(0);

    while (1) {
        thread_yield();
    }

    return 0;
}
