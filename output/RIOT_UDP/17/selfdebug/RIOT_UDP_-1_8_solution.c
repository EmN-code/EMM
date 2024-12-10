#include <netinet/in.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001

int main(void) {
    // Define the UDP endpoint structure
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    
    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    // Define the message to be sent
    const char *msg_working = "Device state: work";
    const char *msg_not_working = "Device state: not working";

    while(1) {
        // In a real-world scenario, you'll check the actual device state here
        bool is_working = true; // For simulation we assume it's always working

        // Send the message every five seconds
        const char *msg = is_working ? msg_working : msg_not_working;
        sock_udp_send(&sock, msg, strlen(msg), &remote);

        // Wait for five seconds
        xtimer_sleep(5);
    }

    return 0;
}