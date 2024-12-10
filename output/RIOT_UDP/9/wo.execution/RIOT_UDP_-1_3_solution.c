#include <thread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <event/thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64

char stack[THREAD_STACKSIZE_DEFAULT];

void *udp_sender(void *arg) {
    (void)arg;
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char state[BUFFER_SIZE];
    size_t state_len;
    char addr_str[INET_ADDRSTRLEN];
    int result = sock_udp_create(&sock, NULL, NULL, 0);
    if (result < 0) {
        return NULL;  // Failure in creating socket
    }
    
    // Convert IP string to network address structure
    if (sock_udp_str_to_ep(&remote, SERVER_IP) < 0) {
        return NULL;  // Failure in converting IP string to endpoint
    }

    while (1) {
        snprintf(state, BUFFER_SIZE, "Device state: %s", "work");
        state_len = strlen(state);

        // Send data to the specified remote endpoint
        if (sock_udp_send(&sock, state, state_len, &remote) < 0) {
            // Handle error
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");

    // Keep main thread alive
    while (1) {
        sleep(1);
    }

    return 0;
}