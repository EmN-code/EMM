#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "sys/ctimer.h"

#define UDP_PORT 20001
#define SERVER_IP_ADDR "47.102.103.1"

static struct uip_udp_conn *client_conn;
static struct ctimer ct;
char *state_message = "work";

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_message(void *ptr) {
    uip_ipaddr_t addr;
    uiplib_ip4addrconv(SERVER_IP_ADDR, &addr);
    uip_udp_packet_sendto(client_conn, state_message, strlen(state_message), 
                          &addr, UIP_HTONS(UDP_PORT));

    printf("Message sent: %s\n", state_message);
    
    ctimer_reset(&ct);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    client_conn = udp_new(NULL, UIP_HTONS(UDP_PORT), NULL);
    udp_bind(client_conn, UIP_HTONS(UDP_PORT));

    ctimer_set(&ct, CLOCK_SECOND * 5, send_message, NULL);

    PROCESS_END();
}