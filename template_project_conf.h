#undef NETSTACK_CONF_WITH_IPV6
#define NETSTACK_CONF_WITH_IPV6 0

#undef NETSTACK_CONF_NETWORK
#define NETSTACK_CONF_NETWORK rime_driver
#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC nullsec_driver
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC csma_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER framer_contikimac
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC contikimac_driver

#undef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8


