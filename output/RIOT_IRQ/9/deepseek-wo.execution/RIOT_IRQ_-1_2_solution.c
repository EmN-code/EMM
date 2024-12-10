
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

#define STACK_SIZE 512

static char thread_stack[STACK_SIZE];
static kernel_pid_t thread_pid;

volatile int atomic_var = 0;

void interrupt_handler(void)
{
    atomic_var = 1;
}

void thread_func(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled successfully!\n");
            atomic_var = 0;
        }
        thread_yield();
    }
}

int main(void)
{
    thread_pid = thread_create(thread_stack,
                               STACK_SIZE,
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_func,
                               NULL,
                               "interrupt_thread");

    irq_set_exclusive_handler(ESP32_IRQ_NUM, interrupt_handler);
    irq_enable(ESP32_IRQ_NUM);

    while (1) {
        thread_yield();
    }

    return 0;
}
