#include <xtimer.h>
#include <thread.h>
#include <sock_udp.h>

#define SERVER_PORT     (20001)
#define SLEEP_INTERVAL  (5U * 1000000U) // 5 seconds

static const char *server_ip = "47.102.103.1";

void *udp_sender(void *arg) {
    (void)arg;
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char *state_message;
    char *working_state = "working";
    char *not_working_state = "not working";

    // Set remote server address
    remote.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, server_ip);
    remote.port = SERVER_PORT;

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    while (1) {
        // Alternate between 'working' and 'not working'
        state_message = (state_message == working_state) ? not_working_state : working_state;
        
        // Send the message
        if (sock_udp_send(&sock, state_message, strlen(state_message), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", state_message);
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL/1000000);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, udp_sender, NULL, "udp_sender");
    return 0;
}