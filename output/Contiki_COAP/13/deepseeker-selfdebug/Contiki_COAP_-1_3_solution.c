
#include <contiki.h>
#include <coap.h>
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_link_format_s coap_link_format;
static struct coap_endpoint_s coap_endpoint;

void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }

    char *payload = (char *)coap_get_payload(response);
    if (payload) {
        printf("Response received: %s\n", payload);
    } else {
        printf("Empty response payload\n");
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_message_t request;
    static coap_message_t response;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, URI_PATH);
        coap_set_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

        coap_endpoint_connect(&coap_endpoint, SERVER_IP, SERVER_PORT);
        coap_send_request(&coap_endpoint, &request, &response, coap_response_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
