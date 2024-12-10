#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include "shell.h"

#define MAIN_QUEUE_SIZE     (8)
#define MSG_INTERVAL_USEC   (1000000)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42; // Just a test value
        msg_send(&msg, thread_getpid());
        xtimer_usleep(MSG_INTERVAL_USEC);
    }

    return NULL;
}

static int msg_handler(int argc, char **argv) {
    (void)argc;
    (void)argv;
    msg_t msg;

    msg_receive(&msg);
    printf("Received message with value: %d\n", msg.content.value);

    return 0;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Starting the sender thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    // Shell command to manually read from the message queue
    shell_command_t shell_commands[] = {
        { "recv_msg", "Receive a message", msg_handler },
        { NULL, NULL, NULL }
    };
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}