#include "sys/etimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "contiki.h"

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_ADDRESS "47.102.103.1"

static struct simple_udp_connection udp_connection;
static struct etimer periodic_timer;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    PROCESS_BEGIN();
    
    simple_udp_register(&udp_connection, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    /* Set the etimer to trigger every five seconds */
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);
    
    while(1) {
        /* Wait for the etimer event to occur */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        
        /* Send the device state message "work" by default */
        char* message = "work";
        
        /* Send the UDP packet */
        uip_ipaddr_t server_ipaddr;
        uiplib_ipaddrconv(UDP_SERVER_ADDRESS, &server_ipaddr);
        simple_udp_sendto(&udp_connection, message, strlen(message), &server_ipaddr);

        /* Reset the timer to send every 5 seconds */
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}