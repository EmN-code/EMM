#include net/ipv6/uiplib.h
#include net/ipv6/udp-socket.h
#include net/ipv6/tcp-socket.h
#include net/ipv6/uip-udp-packet.h
#include net/ipv6/uip-nameserver.h
#include net/ipv6/uip-ds6-route.h
#include net/ipv6/uip-ds6-nbr.h
#include net/ipv6/psock.h
#include net/ipv6/uip-icmp6.h
#include net/ipv6/uip-ds6.h

#include "contiki.h"
#include "contiki-net.h"
#include "erbium.h"

static char buf[512];
static struct etimer et;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();
    
    // Set timer for sending requests every 10 seconds
    etimer_set(&et, CLOCK_SECOND * 10);

    address_t coap_server;
    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv("47.102.103.1", &server_ipaddr);
    coap_server.port = UIP_HTONS(5683);
    uip_ipaddr_copy(&coap_server.address, &server_ipaddr);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        coap_packet_t request[1];
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        PRINTF("Sending CoAP POST request\n");
        COAP_BLOCKING_REQUEST(&coap_server, request, NULL);

        // Wait for the timer to expire again
        etimer_reset(&et);
    }

    PROCESS_END();
}