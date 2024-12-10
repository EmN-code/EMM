
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void udp_callback(void *arg, int src_addr_len, uint8_t *src_addr, uint16_t src_port, uint8_t *data, uint16_t data_len)
{
    (void)arg;
    (void)src_addr_len;
    (void)src_addr;
    (void)src_port;
    (void)data;
    (void)data_len;
}

int main(void)
{
    simple_udp_register(SERVER_PORT, udp_callback, NULL);

    while (1) {
        char buffer[10];
        strncpy(buffer, DEVICE_STATE, sizeof(buffer));
        simple_udp_sendto(SERVER_IP, SERVER_PORT, buffer, strlen(buffer));
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
