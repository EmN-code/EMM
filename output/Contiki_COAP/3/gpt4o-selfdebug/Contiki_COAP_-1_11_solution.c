#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/udp-socket.h"
#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

PROCESS(hello_world_process, "Hello World Process");
AUTOSTART_PROCESSES(&hello_world_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Starting CoAP POST Request to 47.102.103.1:5683 at /coap/test\n");

  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  COAP_BLOCKING_REQUEST(&server_ep, request,
    {
      printf("Response received with code %u\n", response->code);
    });

  PROCESS_END();
}