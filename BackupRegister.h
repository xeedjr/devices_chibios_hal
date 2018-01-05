/*
 * BackupRegister.h
 *
 *  Created on: 12 ���. 2017 �.
 *      Author: Bogdan
 */

#ifndef BACKUPREGISTER_H_
#define BACKUPREGISTER_H_

#if PORT_ARCHITECTURE_ARM

#include <stdint.h>

class BackupRegister {
public:
	BackupRegister();
	virtual ~BackupRegister();

	void write(uint8_t reg, uint16_t val);
	uint16_t read(uint8_t reg);
};

extern BackupRegister stm32_backup_registers;

#endif /*PORT_ARCHITECTURE_ARM*/
#endif /* BACKUPREGISTER_H_ */
