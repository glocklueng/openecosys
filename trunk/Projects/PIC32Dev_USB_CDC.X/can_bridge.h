#ifndef _CAN_BRIDGE_H_
#define _CAN_BRIDGE_H_

#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include <p32xxxx.h>
#include <sys/kmem.h>
#include "plib.h"

CANRxMessageBuffer* can_recv_message();
unsigned char can_send_message(CANTxMessageBuffer * msgPtr);
void netv_init_can_driver();


#endif
