#include "contiki.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"

PROCESS(coap_post_client, "CoAP POST Client");
AUTOSTART_PROCESSES(&coap_post_client);

static struct etimer timer;
static char *server_ip = "47.102.103.1";
static uint16_t server_port = 5683;
static char *uri_path = "/coap/test";

PROCESS_THREAD(coap_post_client, ev, data)
{
  static struct coap_blocking_request_state request_state;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 10);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    coap_blocking_request(&request_state.state);
    const coap_endpoint_t server_endpoint = { .ipaddr = server_ip, .port = server_port };
    const char post_message[] = "Hello, COAP Cloud!";

    coap_message_t request[1];
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, uri_path);
    coap_set_payload(request, (uint8_t *)post_message, sizeof(post_message) - 1);

    if(COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL)) {
      printf("CoAP POST sent successfully\n");
    } else {
      printf("Failed to send CoAP POST\n");
    }
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&coap_post_client, NULL);
  while(1) {
    process_run();
    etimer_request_poll();
  }
  return 0;
}