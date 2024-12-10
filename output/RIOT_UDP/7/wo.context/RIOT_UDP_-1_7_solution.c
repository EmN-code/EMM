#include <string.h>    // For string operations
#include <arpa/inet.h> // For inet_pton
#include <net/sock/udp.h> // For UDP socket APIs
#include <xtimer.h>    // For timed waits

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Specify the remote server's IP and port
    remote.port = 20001;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Create the UDP socket
    sock_udp_create(&sock, &local, NULL, 0);

    // Define the message
    char *msg = "work";
    size_t msg_len = strlen(msg);

    while (1) {
        // Send the device state to the server every five seconds
        sock_udp_send(&sock, msg, msg_len, &remote);
        xtimer_sleep(5);
    }

    // This part of the code will never be reached due to the infinite loop
    return 0;
}