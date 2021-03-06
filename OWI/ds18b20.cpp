#include "ds18b20.h"

void DS18B20::init(OWI* bus)
{
    this->bus = bus;
}

#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0

unsigned char DS18B20::read_scratchpad(unsigned char *data )  /// returns config bitfield
{
    if (!bus->DetectPresence()){
      return 0;
    };

    if (skip_romid == 1)
    {
    	// skip rom
    	bus->SendByte(CMD_SKIPROM);
    }
    else
    {
    	//rom
		bus->SendByte(CMD_MATCHROM);
		bus->SendByte(id[0]);
		bus->SendByte(id[1]);
		bus->SendByte(id[2]);
		bus->SendByte(id[3]);
		bus->SendByte(id[4]);
		bus->SendByte(id[5]);
		bus->SendByte(id[6]);
		bus->SendByte(id[7]);
    };

	bus->SendByte(CMD_READSCRATCHPAD);

    data[0] = bus->ReceiveByte();    /// 0 Tlsb
    data[1] = bus->ReceiveByte();    /// 1 Tmsb
    data[2] = bus->ReceiveByte();    /// 2 Thlim
    data[3] = bus->ReceiveByte();    /// 3 Tllim
    data[4] = bus->ReceiveByte();    /// 4 Config
    data[5] = bus->ReceiveByte();    /// 5 RES0
    data[6] = bus->ReceiveByte();    /// 6 RES1
    data[7] = bus->ReceiveByte();    /// 7 RES2
    data[8] = bus->ReceiveByte();    /// 8 CRC

	if ((data[5] != 0xff) || (data[7] != 0x10) || (bus->CheckScratchPadCRC(data) != CRC_OK)) {
		return 2; // fixed data is not presented
	}
	
    return 1;
}


unsigned char DS18B20::write_scratchpad(unsigned char *data)
{
	if (!bus->DetectPresence()){
		return 0;
	};

	if (skip_romid == 1)
	{
		// skip rom
		bus->SendByte(CMD_SKIPROM);
	}
	else
	{
		//rom
		bus->SendByte(CMD_MATCHROM);
		bus->SendByte(id[0]);
		bus->SendByte(id[1] );
		bus->SendByte(id[2]);
		bus->SendByte(id[3]);
		bus->SendByte(id[4]);
		bus->SendByte(id[5]);
		bus->SendByte(id[6]);
		bus->SendByte(id[7]);
	};

	bus->SendByte(CMD_WRITESCRATCHPAD);
	
	bus->SendByte(data[0]); //Th
	bus->SendByte(data[1]); //Tl
	bus->SendByte(data[2]); //Config

	return 1;
}

unsigned char DS18B20::start_conv()
{
    if (!bus->DetectPresence()){
      return 0;
    };

    if (skip_romid == 1)
    {
    	// skip rom
    	bus->SendByte(CMD_SKIPROM);
    }
    else
    {
    	//rom
		bus->SendByte(CMD_MATCHROM);
		bus->SendByte(id[0]);
		bus->SendByte(id[1] );
		bus->SendByte(id[2]);
		bus->SendByte(id[3]);
		bus->SendByte(id[4]);
		bus->SendByte(id[5]);
		bus->SendByte(id[6]);
		bus->SendByte(id[7]);
    };

    bus->SendByte(CMD_CONVERTTEMP);

	return 1;
}

//�������� ��������� ������� �� ���� � �������� �� ������ ��18S20 ������ ����������
/***********************************************************************************
    Return :
        0 - if termal sensor is off
        1 - if termal sensor is on
        2 - if termal sensor not on not off
************************************************************************************/
unsigned char DS18B20::is()
{
    if (!bus->DetectPresence()){
      return 0;
    };

    return 1;
}

void DS18B20::dataOrConversionReady() {
	for (int i = 0; i < 17000; i++){
		if (bus->ReadBit() == 1) {
			// ready
			return;
		}
	}
}

void DS18B20::setResolution(enum Resolution resol) {
	resolution = resol;
	uint8_t data[3] = {0};
		
	data[2] = ((resol) << 5);
	write_scratchpad(data);
}

#include "cmsis_os.h"
/// read temperature wit delay
float DS18B20::exec()
{
    unsigned char scratchpad[9];
    signed int T = 0;
    float Tem = 1.0;
    unsigned char high = 0;
    unsigned char low = 0;

    if (start_conv() == 0)
    {
        // error cant start conversion
		return -999;
    };
	
/*	switch (resolution) {
		case k9bit:
			osDelay(TCONV9BIT_MS);
		break;
		case k10bit:
			osDelay(TCONV10BIT_MS);
		break;
		case k11bit:
			osDelay(TCONV11BIT_MS);
		break;
		case k12bit:
			osDelay(TCONV12BIT_MS);
		break;
	}
	*/
	dataOrConversionReady();
	
	///read temperature register
	if (read_scratchpad (scratchpad) == 1)	{
		// read good
		T = scratchpad[1];
		T <<= 8;
		T |= scratchpad[0];

		Tem = 1.0;

		if (T < 0)
		{
			Tem = -1.0;
			T = T * -1;
		};

		high = T >> 4;
		low = T & 0x0F;

		Tem = Tem * high;
		Tem = Tem + (low * 0.0625);
	} else {
		return -999;
	}
	
	return Tem;
}

