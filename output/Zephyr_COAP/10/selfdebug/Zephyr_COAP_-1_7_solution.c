
#include <zephyr/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/conn_mgr.h>
#include <zephyr/net/conn_mgr_connectivity.h>
#include <zephyr/net/conn_mgr_monitor.h>
#include <zephyr/net/conn_mgr_prov.h>
#include <zephyr/net/conn_mgr_net_mgmt.h>
#include <zephyr/net/conn_mgr_wifi.h>
#include <zephyr/net/conn_mgr_ethernet.h>
#include <zephyr/net/conn_mgr_dumb.h>
#include <zephyr/net/conn_mgr_loopback.h>
#include <zephyr/net/conn_mgr_dummy.h>
#include <zephyr/net/conn_mgr_null.h>
#include <zephyr/net/conn_mgr_proxy.h>
#include <zephyr/net/conn_mgr_proxy_client.h>
#include <zephyr/net/conn_mgr_proxy_server.h>
#include <zephyr/net/conn_mgr_proxy_client_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <zephyr/net/conn_mgr_proxy_client_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_client_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <zephyr/net/conn_mgr_proxy_server_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal