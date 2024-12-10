
#include "periph/uart.h"
#include "periph/irq.h"

static volatile int atomic_var = 0;

void uart_isr(void *arg)
{
    (void)arg;
    atomic_var = 1;
}

int main(void)
{
    uart_t uart = UART_DEV(0);
    uart_init(uart, 115200, NULL, NULL);

    irq_disable();
    atomic_var = 0;
    irq_restore(irq_enable());

    uart_irq_callback_set(uart, uart_isr);
    uart_irq_enable(uart, UART_IRQ_RX);

    while (1) {
        if (atomic_var) {
            printf("Interrupt handled, atomic_var: %d\n", atomic_var);
            atomic_var = 0;
        }
    }

    return 0;
}
