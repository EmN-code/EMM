#include <timex.h>
#include <event/periodic.h>
#include <event/callback.h>

static void periodic_callback(void *arg)
{
    (void)arg;
    /* Here you can put the code you want to execute periodically */
    printf("Periodic event triggered\n");
}

int main(void)
{
    /* Define event periodic structure */
    event_periodic_t periodic_event;

    /* Initialize the periodic event with a 1 second interval */
    event_periodic_init(&periodic_event, EVENT_PRIO_MEDIUM,
                        periodic_callback, NULL, timex_set(1, 0));

    /* Start the periodic event */
    event_periodic_start(&periodic_event);

    /* Enter an idle loop (or any other application logic) */
    while (1) {
        /* Perform any ongoing tasks here */
    }

    return 0;
}