
#include "xtimer.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char device_state[10];

static void udp_send_state(void) {
    static udp_socket_t socket;
    static uint8_t buffer[10];
    int res;

    udp_socket_init(&socket);
    udp_socket_bind(&socket, 0);

    if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
        strcpy((char *)buffer, DEVICE_STATE_WORK);
    } else {
        strcpy((char *)buffer, DEVICE_STATE_NOT_WORK);
    }

    res = udp_socket_sendto(&socket, buffer, strlen((char *)buffer), UDP_SERVER_IP, UDP_SERVER_PORT);
    if (res < 0) {
        printf("Failed to send UDP packet\n");
    } else {
        printf("Sent device state: %s\n", buffer);
    }

    udp_socket_close(&socket);
}

int main(void) {
    strcpy(device_state, DEVICE_STATE_WORK); // Initial state

    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
