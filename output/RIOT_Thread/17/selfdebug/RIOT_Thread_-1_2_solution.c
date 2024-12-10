#include <stdio.h>  // Include standard input/output header for printing
#include <inttypes.h>  // Include integer types for correct format specifiers
#include "msg.h"  // Include RIOT's messaging API
#include "thread.h"  // Include thread management functions

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack_sender[STACK_SIZE];
static char stack_receiver[STACK_SIZE];

static msg_t queue[8];  // Message queue of 8 messages

static msg_t msg;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void) arg;
    while (1) {
        msg.content.value = 42;  // Arbitrary value
        printf("Sending message with value: %