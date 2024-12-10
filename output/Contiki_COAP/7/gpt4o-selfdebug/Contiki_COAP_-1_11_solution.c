#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "contiki.h"
#include "contiki-net.h"
#include "erbium.h"

#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0x2607, 0xf0d0, 0x1002, 0x51, 0x0, 0x0, 0x0, 0x1)
#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(5683)

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static uip_ipaddr_t server_ipaddr;
static struct etimer et;
static struct uip_udp_conn *client_conn;

PROCESS_THREAD(coap_client_example, ev, data)
{
  PROCESS_BEGIN();
  
  static coap_packet_t request[1];
  
  etimer_set(&et, CLOCK_SECOND * 5);
  SERVER_NODE(&server_ipaddr);
  
  client_conn = udp_new(&server_ipaddr, REMOTE_PORT, NULL);
  udp_bind(client_conn, LOCAL_PORT);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    /* Prepare the request */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, CoAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);
    
    /* Send the request */
    printf("Sending CoAP request\n");
    uip_udp_packet_send(client_conn, request, sizeof(request));
    
    /* Wait for the next transmission */
    etimer_reset(&et);
  }

  PROCESS_END();
}
