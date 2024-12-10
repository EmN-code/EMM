
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-nbr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset)
{
    const char *payload = "Hello, COAP Cloud";
    coap_set_header_content_format(response, COAP_CONTENT_FORMAT_TEXT_PLAIN);
    coap_set_payload(response, (uint8_t *)payload, strlen(payload));
}

static void coap_periodic_handler(void)
{
    static coap_message_t request;
    static coap_message_t response;
    static uint8_t buffer[128];
    int32_t offset = 0;

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);

    coap_call_handlers(&request, &response, buffer, sizeof(buffer), &offset);
}

int main(void)
{
    /* Initialize the COAP stack */
    coap_init();

    /* Register the periodic handler */
    PERIODIC_RESOURCE("coap_test", COAP_GET | COAP_POST, NULL, coap_post_handler, NULL, NULL, 1000, coap_periodic_handler);

    /* Main loop */
    while (1) {
        /* Process COAP events */
        coap_process();

        /* Sleep or yield to other processes */
        process_poll(&coap_process);
    }

    return 0;
}
