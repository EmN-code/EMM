
#include "irq.h"
#include "event/thread.h"
#include "thread.h"

#define STACKSIZE 512

static char thread_stack[STACKSIZE];
static kernel_pid_t thread_pid;

volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
}

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_var == 1) {
            unsigned state = irq_disable();
            atomic_var = 0;
            irq_restore(state);
        }
    }
    return NULL;
}

int main(void)
{
    thread_pid = thread_create(thread_stack,
                               STACKSIZE,
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_func,
                               NULL,
                               "interrupt_thread");

    irq_set_exclusive_handler(ESP32_IRQ_NUM, interrupt_handler);
    irq_enable();

    return 0;
}
