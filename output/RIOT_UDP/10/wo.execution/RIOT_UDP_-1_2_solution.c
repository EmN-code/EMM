#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <udp.h>
#include <thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define NOT_MESSAGE "not work"
#define INTERVAL 5

static void *send_state(void *arg) {
    (void)arg;  // argument not used
    int sock;
    struct sockaddr_in server_addr;
    char *message = MESSAGE;

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        return NULL;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            // handle error
        }
        thread_sleep(INTERVAL);
        message = (strcmp(message, MESSAGE) == 0) ? NOT_MESSAGE : MESSAGE;
    }

    close(sock);
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, send_state, NULL, "send_state");
    return 0;
}
