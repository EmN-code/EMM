#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC)
#define CURRENT_STACKSIZE 512

static char stack[CURRENT_STACKSIZE];

void *send_state(void *arg)
{
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *state_msg = "work";
        printf("Sending state: %s\n", state_msg);
        sock_udp_send(&sock, state_msg, strlen(state_msg), &remote);
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT UDP client example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, send_state, NULL, "udp_sender");

    return 0;
}