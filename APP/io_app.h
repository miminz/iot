#ifndef _IO_APP_H_
#define _IO_APP_H_

#include "debug.h"
#include "WCHNET.h"
#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"

#define IO_EDATA   0x00
extern struct fdb_kvdb kvdb;
 //const uint8_t boot_count = 1;


void PVD_Config(void);
void init_io(void);


#endif /* _NET_APP_H_ */
