
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>
#include <msg.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static volatile int atomic_var = 0;
static kernel_pid_t timer_pid;

void timer_callback(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Timer interrupt triggered, atomic_var: %d\n", atomic_var);
}

void timer_thread(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        timer_callback(NULL);
    }
}

int main(void)
{
    // Initialize the timer thread
    timer_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, timer_thread, NULL, "timer_thread");

    // Initialize the xtimer
    xtimer_t timer;
    xtimer_init(&timer, TIMER_DEV(0), timer_callback, NULL);

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 10;

    // Restore interrupts
    irq_restore(state);

    // Wait for a few seconds to see the timer interrupts
    xtimer_usleep(5 * TIMER_INTERVAL);

    // Disable the timer
    xtimer_remove(&timer);

    printf("Final atomic_var: %d\n", atomic_var);

    return 0;
}
