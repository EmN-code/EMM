#include <thread.h>
#include <timex.h>
#include <udp.h>
#include <net/af_unspec.h>
#include <net/protnum.h>
#include <xtimer.h>
#include <stdio.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL 5 // Time interval in seconds

static kernel_pid_t udp_thread_pid;
static char udp_thread_stack[THREAD_STACKSIZE_DEFAULT];

static void *udp_sender_thread(void *arg) {
    (void)arg; // Unused parameter
    struct sockaddr_storage server_addr;
    sock_udp_ep_t udp_endpoint = {.family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF};
    udp_endpoint.port = UDP_SERVER_PORT;
    inet_pton(AF_INET, UDP_SERVER_ADDR, &udp_endpoint.addr.ipv4);
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, (sock_udp_ep_t *)&server_addr, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }
    
    while (1) {
        ssize_t bytes_sent = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &udp_endpoint);
        if (bytes_sent <= 0) {
            puts("Error sending UDP message");
        }
        else {
            puts("Message sent");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("UDP Sender started");
    udp_thread_pid = thread_create(udp_thread_stack, sizeof(udp_thread_stack),
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST,
                                   udp_sender_thread, NULL, "UDP sender thread");
    thread_sleep();
    return 0;
}