
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t thread_pid;
static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

void *interrupt_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt detected!\n");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    // Initialize the interrupt handler
    irq_disable();
    irq_set_handler(IRQ_NUM, interrupt_handler);
    irq_enable(IRQ_NUM);

    // Create a thread to handle the interrupt
    thread_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        interrupt_thread,
        NULL,
        "interrupt_thread"
    );

    // Main loop to simulate some work
    while (1) {
        printf("Main loop running...\n");
        xtimer_sleep(2);
    }

    return 0;
}
