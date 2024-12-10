
#include "periph/uart.h"

volatile int interrupt_flag = 0;

void uart_rx_callback(void *arg) {
    (void)arg;
    interrupt_flag = 1;
}

int main(void) {
    uart_t uart = UART_DEV(0);
    uart_init(uart, 115200, UART_PARITY_NONE, UART_DATA_BITS_8, UART_STOP_BITS_1);
    uart_irq_callback_set(uart, uart_rx_callback);
    uart_irq_enable(uart, UART_IRQ_RX);

    while (1) {
        if (interrupt_flag) {
            interrupt_flag = 0;
            // Handle the interrupt
        }
    }

    return 0;
}
