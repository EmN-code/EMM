
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>
#include <esp_system.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_t msg;
    kernel_pid_t pid = thread_getpid();

    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
        msg_send(&msg, pid);
        event_periodic_callback(NULL);
    }

    return 0;
}
