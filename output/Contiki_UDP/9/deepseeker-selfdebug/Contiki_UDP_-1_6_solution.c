
#include <stdio.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/udp-socket.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct udp_socket sock;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void udp_event_handler(struct udp_socket *sock, void *ptr, const uip_ipaddr_t *src_addr, uint16_t src_port, const uip_ipaddr_t *dest_addr, uint16_t dest_port, const uint8_t *data, uint16_t datalen)
{
    // Handle incoming UDP packets if needed
}

PROCESS_THREAD(udp_client_process, ev, data)
{
    static char device_state[10];
    static uip_ipaddr_t server_ip;

    PROCESS_BEGIN();

    // Initialize UDP socket
    udp_socket_init(&sock);
    udp_socket_register_recv(&sock, NULL, udp_event_handler);

    // Resolve server IP address
    uip_ipaddr_from_str(&server_ip, UDP_SERVER_IP);

    // Set initial device state
    strncpy(device_state, DEVICE_STATE_WORK, sizeof(device_state));

    while (1) {
        // Send device state to server
        udp_socket_sendto(&sock, device_state, strlen(device_state), &server_ip, UDP_SERVER_PORT);

        // Wait for 5 seconds
        etimer_set(&et, SEND_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Toggle device state
        if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strncpy(device_state, DEVICE_STATE_NOT_WORK, sizeof(device_state));
        } else {
            strncpy(device_state, DEVICE_STATE_WORK, sizeof(device_state));
        }
    }

    PROCESS_END();
}
