#include "thread.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "sys/socket.h"

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define BUF_SIZE 128

static char state[] = "work";

void *udp_sender_thread(void *arg) {
    (void)arg;
    int sock_fd;
    struct sockaddr_in6 server_addr;
    size_t len;
    
    /* Create UDP socket */
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd < 0) {
        perror("socket");
        return NULL;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(SERVER_PORT);

    /* Convert the server address */
    if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin6_addr) <= 0) {
        perror("inet_pton");
        return NULL;
    }

    while (1) {
        /* Send the device state to the server */
        len = strlen(state);
        if (sendto(sock_fd, state, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto");
        }
        
        /* Sleep for 5 seconds */
        ztimer_sleep(); // Updated function from xtimer_sleep
    }

    close(sock_fd);
    return NULL;
}

int main(void) {
    thread_create(udp_sender_thread, NULL);
    return 0;
}