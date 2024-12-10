
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uiplib.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"

static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_device_state(void) {
    static char message[] = DEVICE_STATE_MESSAGE;
    uip_ipaddr_t ipaddr;

    // Convert the server IP address to uIP format
    uiplib_ipaddrconv(UDP_SERVER_IP, &ipaddr);

    // Set the destination IP address and port
    uip_ipaddr(&ipaddr, 0x2f, 0x66, 0x67, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    uip_udp_packet_sendto(udp_conn, message, strlen(message), &ipaddr, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    // Initialize the UDP connection
    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(0));

    // Set the timer to send the message every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state message
        send_device_state();

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    process_start(&udp_client_process, NULL);
    return 0;
}
