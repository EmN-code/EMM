
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ip/uip-debug.h"
#include "net/ip/uip-nameserver.h"
#include "net/ip/uip-icmp6.h"
#include "net/ip/uip-ds6-route.h"
#include "net/ip/uip-ds6-nbr.h"
#include "net/ip/uip-sr.h"
#include "net/ip/uip-udp-socket.h"
#include "net/ip/uip-tcp-socket.h"
#include "net/ip/uip-coap.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  printf("CoAP response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_message_t request;
  static coap_message_t response;
  static uint8_t buffer[128];
  static uint8_t payload[] = "Hello, COAP Cloud";

  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, payload, sizeof(payload) - 1);

    coap_send_request(&request, COAP_SERVER_IP, COAP_SERVER_PORT, &response, coap_response_handler);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  printf("Starting CoAP client on NRF52840\n");
  process_start(&coap_client_process, NULL);
  return 0;
}
