/* Necessary includes */
#include <sys/socket.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>

/* Definitions and initializations */
#define PERIODIC_INTERVAL (1U * 1000U * 1000U) /* 1 second in microseconds */

/* Handler for the periodic event */
void on_periodic_event(void)
{
    /* Custom code to be executed every second */
    printf("Periodic event triggered\n");
}

/* Main execution function */
int main(void)
{
    /* Initialize the periodic event */
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    /* Event message (dummy message) */
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        /* Simulate periodic callback using message and timer */
        if (msg_receive(&msg_queue[0])) {
            on_periodic_event();
        }
        /* Sleep to simulate periodic check, assuming one second */
        usleep(1000000);
    }

    /* This point is never reached */
    return 0;
}
