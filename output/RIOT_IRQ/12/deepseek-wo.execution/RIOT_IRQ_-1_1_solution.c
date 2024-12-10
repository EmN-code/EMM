
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t thread_pid;
static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void *interrupt_handler_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt handled!\n");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void irq_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the thread for handling interrupts
    thread_create(
        thread_pid,
        "interrupt_handler",
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        0,
        interrupt_handler_thread,
        NULL,
        "interrupt_handler");

    // Register the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Main loop to simulate some work
    while (1) {
        printf("Main loop running...\n");
        xtimer_sleep(2);
    }

    return 0;
}
