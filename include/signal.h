#ifndef __SIGNAL_H__
#define __SIGNAL_H__

// These values are for slot regsitration
#define SIG_SLOT_OK              1
#define SIG_SLOT_INVALID_CALLER  0
#define SIG_MISSING_SIGNAL      -1
#define SIG_MISSING_OBJECT      -2
#define SIG_MISSING_FUNCTION    -3
#define SIG_INVALID_FUNCTION    -4
#define SIG_INVALID_OBJECT      -5

// Standard system signals
#define SIG_SYS_BOOT            0x01
#define SIG_SYS_CRASH           0x02

#endif // __SIGNAL_H__
