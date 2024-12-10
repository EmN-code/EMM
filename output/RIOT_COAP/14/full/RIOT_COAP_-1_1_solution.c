#include <xtimer.h>
#include <net/gcoap.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&server.addr, SERVER_IP);

    int rc;
    while (1) {
        rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        if (rc < 0) {
            printf("Failed to initialize request\n");
            xtimer_sleep(1);
            continue;
        }

        char payload[] = "Hello, COAP Cloud";
        size_t len = gcoap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
        memcpy(pdu.payload, payload, strlen(payload));
        len += strlen(payload);

        rc = gcoap_req_send(buf, len, &server, _resp_handler, NULL);
        if (rc < 0) {
            printf("Failed to send request\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Request sent successfully\n");
        break; // Exit loop after successful send
    }

    while (1) {
        xtimer_sleep(10); // Keep program running
    }

    return 0;
}
