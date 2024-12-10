#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "irq.h"

volatile int atomic_var = 0;

void interrupt_handler(void *arg) {
    (void)arg;
    disableIRQ();
    atomic_var++;
    enableIRQ();
}

int main(void) {
    printf("Initial Atomic Variable: %d\n", atomic_var);

    disableIRQ();
    printf("IRQ disabled\n");

    // Simulate an interrupt
    interrupt_handler(NULL);

    printf("Atomic Variable after interrupt: %d\n", atomic_var);

    enableIRQ();
    printf("IRQ enabled\n");

    return 0;
}