
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
            printf("Interrupt received and handled.\n");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    // Initialize the thread for handling interrupts
    thread_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        interrupt_thread,
        NULL,
        "interrupt_thread"
    );

    // Set up the interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, interrupt_handler);
    irq_enable(ESP32_IRQ_NUM);

    // Main loop to simulate some work
    while (1) {
        printf("Main thread running...\n");
        xtimer_sleep(2);

        // Simulate an interrupt by triggering the handler manually
        irq_disable(ESP32_IRQ_NUM);
        interrupt_handler();
        irq_restore(ESP32_IRQ_NUM);
    }

    return 0;
}
