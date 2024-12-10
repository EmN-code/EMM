
#include "contiki.h"
#include "coap.h"
#include "lib/random.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct uip_udp_conn *coap_conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_post_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  printf("CoAP response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  static struct etimer et;
  static coap_endpoint_t server_ep;
  static coap_message_t request;
  static coap_message_t response;
  static uint8_t buffer[128];

  PROCESS_BEGIN();

  // Initialize CoAP endpoint
  coap_endpoint_init(&server_ep, COAP_SERVER_IP, COAP_SERVER_PORT);

  // Initialize CoAP message
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, COAP_URI_PATH);
  coap_set_payload(&request, "Hello, COAP Cloud", 16);

  // Send CoAP request
  coap_send_request(&request, &server_ep, coap_post_handler);

  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  PROCESS_END();
}
