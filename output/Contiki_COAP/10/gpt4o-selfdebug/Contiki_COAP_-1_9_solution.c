#include <stdio.h>
#include <string.h>
#include <net/ipv6/uiplib.h>
#include <net/ipv6/udp-socket.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/uip-nameserver.h>
#include <net/ipv6/uip-icmp6.h>
#include <coap-engine.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define CLIENT_IDENTITY "client_identity"
#define CLIENT_SECRET "client_secret"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static uip_ipaddr_t server_ipaddr;
  static struct uip_udp_conn *client_conn;
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  uiplib_ipaddrconv("47.102.103.1", &server_ipaddr);
  client_conn = udp_new(&server_ipaddr, UIP_HTONS(5683), NULL);
  if(!client_conn) {
    printf("Failed to establish connection\n");
    PROCESS_EXIT();
  }

  coap_init_engine();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
  coap_set_header_uri_path(request, "/coap/test");

  coap_blocking_request(client_conn, request, coap_receiver);

  PROCESS_END();
}

static void coap_receiver(coap_message_t *response) {
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  const uint8_t *payload = NULL;
  int len = coap_get_payload(response, &payload);
  printf("Response Received: %.*s\n", len, (char *)payload);
}