#include <thread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <event/thread.h>
#include <esp_system.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

static const char *STATE_MSG = "Device state: work";
static const int SLEEP_INTERVAL = 5;

void *udp_sender(void *arg) {
    (void)arg;

    int sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        // Send the state message to the server
        ssize_t sent = sendto(sock, STATE_MSG, strlen(STATE_MSG), 0,
                              (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            perror("sendto");
        }

        // Sleep for the specified interval
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * 1000);
    }

    close(sock);
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, udp_sender, NULL, "udp_sender");

    return 0;
}