#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <driver/socket_err.h>

#define MUD 0
#define STREAM 1
#define DATAGRAM 2
#define STREAM_BINARY 3
#define DATAGRAM_BINARY 4
#define STREAM_TLS 5
#define STREAM_TLS_BINARY 6

#define SOCKET_STREAM 1
#define SOCKET_STREAM_LISTEN 2
#define SOCKET_DATAGRAM 3
#define SOCKET_MUD 4
#define SOCKET_MUD_LISTEN 5
#define SOCKET_ACQUIRE 6
#define SOCKET_STREAM_TLS 7
#define SOCKET_STREAM_TLS_BINARY 8

#define SO_TLS_VERIFY_PEER 1
#define SO_TLS_SNI_HOSTNAME 2

#endif // __SOCKET_H__
