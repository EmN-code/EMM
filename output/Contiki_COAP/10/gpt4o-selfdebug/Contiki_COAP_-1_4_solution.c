#include <contiki-net.h>
#include <net/ipv6/uip-ds6-nbr.h>
#include <net/ipv6/uip-ds6.h>
#include <net/ipv6/uip-sr.h>
#include <net/ipv6/uiplib.h>
#include <net/ipv6/uip-nameserver.h>
#include <net/ipv6/simple-udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */

  uip_ipaddr_t server_ipaddr;
  if (!uiplib_ipaddrconv(SERVER_IP, &server_ipaddr)) {
      printf("Failed to configure server IP address\n");
      PROCESS_EXIT();
  }

  coap_endpoint_init(&server_endpoint, &server_ipaddr, SERVER_PORT);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)MESSAGE, sizeof(MESSAGE) - 1);

  COAP_BLOCKING_REQUEST(&server_endpoint, request,
                        NULL);

  printf("CoAP POST request sent to %s at port %d\n", SERVER_IP, SERVER_PORT);

  PROCESS_END();
}