#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <msg.h>
#include <kernel.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    printf("This is an example RIOT application on ESP32\n");

    /* setup message box and event handling here */

    while (1) {
        /* Simulate sending events at intervals */
        printf("Sending message...\n");
        xtimer_sleep(5); // wait for 5 seconds before sending the next message
    }

    return 0;
}
