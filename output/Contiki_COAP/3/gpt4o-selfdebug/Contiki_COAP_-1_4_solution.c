#include <contiki-net.h>
#include <contiki-lib.h>
#include <contiki-conf.h>
#include <coap-timer.h>
#include <coap-request-state.h>

#define REMOTE_PORT 5683
#define REMOTE_IP_ADDR "47.102.103.1"
#define URI_PATH "/coap/test"

PROCESS(coap_example_client, "COAP Client Example");
AUTOSTART_PROCESSES(&coap_example_client);

memb(coap_requests, sizeof(coap_message_t), 1);

static struct uip_ip_addr server_ipaddr;
static coap_message_t request[1];

PROCESS_THREAD(coap_example_client, ev, data) {
  PROCESS_BEGIN();

  static etimer_t et;
  etimer_set(&et, CLOCK_SECOND * 2);

  printf("Starting COAP Client\n");

  uiplib_ipaddrconv(REMOTE_IP_ADDR, &server_ipaddr);
  printf("Server IP: %s\n", REMOTE_IP_ADDR);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    etimer_reset(&et);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);

    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    printf("Sending COAP POST request to %s with payload: %s\n", REMOTE_IP_ADDR, msg);

    COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
         NULL);
  }

  PROCESS_END();
}