
#include "event/thread.h"
#include "thread.h"
#include "irq.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static volatile int atomic_var = 0;

static void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Simulate atomic variable modification in interrupt context
}

static void test_thread(void *arg)
{
    (void)arg;

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify atomic variable in thread context
    atomic_var = 2;

    // Restore interrupts
    irq_restore(state);

    // Simulate interrupt handler execution
    interrupt_handler(NULL);

    // Validate the result
    if (atomic_var == 1) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    // End the thread
    thread_exit();
}

int main(void)
{
    // Create a thread for the test
    thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        test_thread,
        NULL,
        "test_thread"
    );

    // Main thread continues its execution
    while (1) {
        // Main thread's work
    }

    return 0;
}
