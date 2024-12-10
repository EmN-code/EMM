// Include necessary header files in descending order by their first letter
#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/protnet.h>
#include <net/prot/ipv6/netif.h>
#include <net/prot/ipv6/6lowpan.h>
#include <net/interface.h>
#include <net/gnrc/netif.h>
#include <kernel_types.h>
#include <kernel.h>

#define UDP_SERVER_ADDR   "47.102.103.1"
#define UDP_SERVER_PORT   (20001)
#define SEND_INTERVAL     (5U)  // Interval in seconds
#define MSG_BUF_SIZE      (64U) // Maximum size for the message buffer

// Thread stack size
char stack[THREAD_STACKSIZE_MAIN];

// Function to send a message to the UDP server
void *udp_sender(void *arg) {
    (void)arg;
    struct sockaddr_in6 server_addr;
    sock_udp_ep_t remote = { .family = AF_INET6,
                             .port = UDP_SERVER_PORT };
    inet_pton(AF_INET6, UDP_SERVER_ADDR, &server_addr.sin6_addr);

    strcpy((char *)remote.addr.ipv6, (char *)server_addr.sin6_addr.s6_addr);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    char message[] = "work";
    if (condition_not_work) strcpy(message, "not work");
    size_t msg_len = strlen(message);

    while (1) {

        // Sending data to the UDP server
        if (sock_udp_send(&sock, message, msg_len, &remote) < 0) {
            printf("Error sending UDP packet\n");
        }
        else {
            printf("Sent: %s\n", message);
        }

        xtimer_sleep(SEND_INTERVAL); // Sleep for the defined interval
    }
    return NULL;
}

int main(void) {
    // Placeholder variable to determine if device is working or not
    int condition_not_work = 0; // 0 for work, 1 for not work

    // Create a thread for sending UDP data
    kernel_pid_t udp_sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN-1,
                                                THREAD_CREATE_STACKTEST, udp_sender, NULL,
                                                "udp_sender");
    if (udp_sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating UDP sender thread\n");
        return 1;
    }

    return 0;
}