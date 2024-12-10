#include "msg.h"
#include "inttypes.h"
#include "kernel.h"
#include "xtimer.h"

#define MESSAGE_QUEUE_SIZE 8

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static kernel_pid_t main_thread_pid;

void sender(void)
{
    /* Message to send */
    msg_t msg;
    
    while(1) {
        /* Fill the message with the current time tick */
        msg.content.value = xtimer_usec_from_ticks(xtimer_now());
        
        /* Send the message to the main thread */
        msg_send(&msg, main_thread_pid);
        
        /* Wait for a second before sending the next message */
        xtimer_sleep(1);
    }
}

int main(void)
{
    main_thread_pid = thread_getpid();

    /* Initialize the message queue */
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    /* Create a sender thread */
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)sender, NULL, "sender");

    while (1) {
        /* Receive messages in the main thread */
        msg_t msg;
        msg_receive(&msg);

        /* Process received messages (e.g., print the received time) */
        printf("Received message with time: %