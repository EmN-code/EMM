#include <ztimer.h>
#include <xtimer.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define MAX_ATTEMPTS 5

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t* remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout for msg ID %u\n", coap_get_id(pdu));
        return;
    }
    printf("Response from %s*\n", COAP_SERVER_IP);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    int rc;

    while (1) {
        rc = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (rc <= 0) {
            printf("Invalid IP address\n");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        }
        else {
            printf("IP address conversion successful\n");
            break;
        }
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);

    coap_payload(&pdu, (uint8_t *)PAYLOAD, sizeof(PAYLOAD)-1);

    int attempt = 0;
    while (attempt < MAX_ATTEMPTS) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending request\n");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            attempt++;
        }
        else {
            printf("Request sent successfully\n");
            break;
        }
    }

    return 0;
}