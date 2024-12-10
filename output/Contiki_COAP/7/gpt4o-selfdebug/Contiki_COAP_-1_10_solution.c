#include "net/ipv6/uip.h"
#include "net/ipv6/udp-socket.h"
#include "contiki.h"
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct udp_socket udp_conn;
static uip_ipaddr_t server_ipaddr;

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer timer;
  static char buf[128];
  static coap_packet_t request[1];

  PROCESS_BEGIN();

  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
  udp_socket_register(&udp_conn, NULL, NULL);
  udp_socket_connect(&udp_conn, &server_ipaddr, UIP_HTONS(COAP_SERVER_PORT));

  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    int msg_len = coap_serialize_message(request, buf);

    udp_socket_send(&udp_conn, buf, msg_len);
    printf("CoAP message sent\n");

    etimer_reset(&timer);
  }

  PROCESS_END();
}