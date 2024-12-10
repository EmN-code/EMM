// Required includes (headers in descending alphabetical order by first letter)
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sched.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sock/udp.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/nanocoap/cache.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mutex.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    (void)memo;
    // Implementation of a simple response handler
    if (pdu) {
        printf("Response received: %u\n", pdu->payload_len);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;
    int result;

    // Initialize buffer and set destination endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (result <= 0) {
        puts("Error: unable to encode IPv4 address\n");
        return 1;
    }

    // Initialize CoAP packet
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len <= 0) {
        puts("Error: unable to initialize CoAP request\n");
        return 1;
    }

    // Set payload
    size_t payload_len = strlen(COAP_PAYLOAD);
    pdu.payload = (uint8_t *)COAP_PAYLOAD;
    pdu.payload_len = payload_len;

    // Send request
    gcoap_req_send(buf, len + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("Request sent\n");

    // Sleep to wait for the response
    ztimer_sleep(ZTIMER_MSEC, 2000);

    return 0;
}