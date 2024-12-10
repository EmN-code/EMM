#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static const char *device_state_working = "Device is working";
static const char *device_state_not_working = "Device is not working";

/**
 * @brief Sends the device state to the designated server over UDP
 *
 * @param working A flag indicating whether the device is working.
 */
static void send_device_state(bool working) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    if (inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to encode IP address");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    const char *msg = working ? device_state_working : device_state_not_working;
    if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
        puts("Error: unable to send data");
    }

    sock_udp_close(&sock);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    bool device_status = false; // Let's assume it's starting as not working

    while (1) {
        // Toggle device status for simulation purposes
        device_status = !device_status;
        send_device_state(device_status);
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}