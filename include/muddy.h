#ifndef __MUDDY_H
#define __MUDDY_H

#include <websocket.h>

// muddy opcodes
#define MUDDY_DISPATCH 0x00
#define MUDDY_HEARTBEAT 0x01
#define MUDDY_IDENTIFY 0x02
#define MUDDY_HELLO 0x0A
#define MUDDY_HEARTBEAT_ACK 0x0B

// muddy events
#define EVENT_MESSAGE_CREATE "MESSAGE_CREATE"
#define EVENT_ECHO "ECHO"

#endif // __MUDDY_H
