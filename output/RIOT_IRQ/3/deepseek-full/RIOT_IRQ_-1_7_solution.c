
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "irq.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "timex.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "mma8x5x.h"

#define UART_DEV UART_DEV(0)
#define UART_BAUDRATE_115200 115200

static volatile atomic_bool interrupt_flag = ATOMIC_VAR_INIT(false);

static void uart_rx_cb(void *arg) {
    (void)arg;
    atomic_store(&interrupt_flag, true);
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    uart_t uart = UART_DEV;
    if (uart_init(uart, UART_BAUDRATE_115200, uart_rx_cb, NULL) != 0) {
        puts("UART initialization failed");
        return 1;
    }

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            unsigned state = irq_disable();
            puts("Interrupt received and handled");
            atomic_store(&interrupt_flag, false);
            irq_restore(state);
        }
        xtimer_sleep(1);
    }

    return 0;
}
