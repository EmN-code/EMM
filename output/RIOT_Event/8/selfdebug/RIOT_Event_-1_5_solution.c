#include <ztimer.h>
#include <stdio.h>
#include <event.h>
#include <cib.h>
#include <msg.h>
#include <errno.h>
#include <periph/gpio.h>

void callback(void *arg) {
    printf("Callback function executed.\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = callback;
    timer.arg = NULL;

    printf("Initializing periodic event callback.\n");
    
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
        timer.callback(timer.arg);
    }

    return 0;
}