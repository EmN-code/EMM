
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str(remote.addr.ipv4, UDP_SERVER_IP);

    char buf[64];
    snprintf(buf, sizeof(buf), "%s", DEVICE_STATE_MSG);

    if (sock_udp_send(NULL, buf, strlen(buf), &remote) < 0) {
        puts("Failed to send UDP message");
    } else {
        puts("Device state sent successfully");
    }
}

int main(void)
{
    puts("Starting UDP client...");

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
