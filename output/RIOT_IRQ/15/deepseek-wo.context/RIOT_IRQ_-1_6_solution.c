
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <periph/cpuid.h>
#include <periph/pm.h>
#include <periph/atomic_utils.h>

#define TEST_VALUE 42

static atomic_t atomic_var = ATOMIC_INIT(TEST_VALUE);

void test_thread(void)
{
    while (1) {
        unsigned state = irq_disable();
        atomic_var = atomic_var + 1;
        irq_restore(state);
        xtimer_usleep(100000); // Sleep for 100 ms
    }
}

int main(void)
{
    puts("Interrupt handling test with atomic variables");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, test_thread, NULL, "test_thread");

    while (1) {
        unsigned state = irq_disable();
        if (atomic_var != TEST_VALUE) {
            printf("Atomic variable modified: %d\n", atomic_var);
        }
        irq_restore(state);
        xtimer_usleep(500000); // Sleep for 500 ms
    }

    return 0;
}
