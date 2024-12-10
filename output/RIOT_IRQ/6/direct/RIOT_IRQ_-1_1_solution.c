#include <xtensa/hal.h>
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "xtimer.h"

#define STACKSIZE  (THREAD_STACKSIZE_DEFAULT)
#define PRIO       (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[STACKSIZE];
atomic_t interrupt_occurred = ATOMIC_INIT(0);

void *myThread(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(5);
        printf("Checking interrupts...\n");
        int was_disabled = xtensa_int_enable();
        if (atomic_read(&interrupt_occurred) == 1) {
            printf("Interrupt was handled while interrupts were disabled!\n");
            atomic_set(&interrupt_occurred, 0);
        }
        xtensa_int_disable();

        if (was_disabled) {
            xtensa_int_restore(was_disabled);
        }
    }
}

static void irq_handler(void *arg) {
    (void)arg;
    atomic_set(&interrupt_occurred, 1);
}

int main(void) {
    puts("RIOT Interrupt Handling Example");

    thread_create(thread_stack, sizeof(thread_stack), PRIO, 0, myThread, NULL, "myThread");

    irq_enable(); // or specific irq_enable(N);
    // Simulate an interrupt by calling the handler directly
    irq_handler(NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}