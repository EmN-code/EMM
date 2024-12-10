#include <thread.h>  // Include the thread header for managing threads
#include <net/gnrc/udp.h>  // Include the network UDP header
#include <cstring>  // Include C string header for handling C-style strings

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define PERIOD 5  // Time period for sending messages in seconds

static msg_t msg_queue[8];  // Message queue for handling messages

void *udp_client_thread(void *arg) {
    (void)arg;
    gnrc_netreg_entry_t netreg;
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
	sock_udp_t sock;

    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR);
    server.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Failed to create UDP sock");
        return NULL;
    }

    char state_message[] = "device state: work";  // Initial device state
    msg_init_queue(msg_queue, 8);

    while (1) {
        if (sock_udp_send(&sock, state_message, strlen(state_message), NULL) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(PERIOD);
    }
    return NULL;
}

int main(void) {
    puts("UDP client application started");
    
    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  udp_client_thread, NULL, "udp_client_thread");

    return 0;
}