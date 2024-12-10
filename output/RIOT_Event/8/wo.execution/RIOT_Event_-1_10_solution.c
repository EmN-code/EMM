#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <xtimer.h>

static void callback_function(void *arg)
{
    (void)arg;
    printf("Callback function triggered every second.\n");
}

int main(void)
{
    printf("RIOT application starting on ESP32.\n");

    /* Set a timer to call the callback every second */
    xtimer_ticks32_t interval = xtimer_usec_to_ticks(1000000);
    while (1) {
        callback_function(NULL);
        xtimer_sleep(interval.sec);
    }

    return 0;
}