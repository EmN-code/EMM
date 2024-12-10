
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <kernel_defines.h>

#define UART_DEV UART_DEV(0)

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void uart_rx_callback(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    uart_t uart = UART_DEV;
    uart_init(uart, UART_BAUDRATE_115200, uart_rx_callback, NULL);

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt received!\n");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_usleep(100000); // Sleep for 100ms
    }

    return 0;
}
