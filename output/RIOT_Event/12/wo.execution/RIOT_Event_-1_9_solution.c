#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "mutex.h"
#include "stdio.h"
#include "stdlib.h"

#define INTERVAL (1U * US_PER_SEC)

void callback_function(void)
{
    printf("Callback function called every second.\n");
}

int main(void)
{
    printf("Initializing periodic callback every second.\n");

    while (1) {
        xtimer_sleep(1);
        callback_function();
    }

    return 0;
}