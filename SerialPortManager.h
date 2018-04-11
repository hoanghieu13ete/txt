#pragma once
#ifndef  SERIALPORTMANGER_HEADER
#define SERIALPORTMANGER_HEADER
#include "SerialPortManager.h"

#include <msclr\marshal_cppstd.h>
#include <string>

ref class SerialPortManager
{
public:
	SerialPortManager();
	~SerialPortManager();
	//bool writeSignal(std::string singal);
	//std::string readSignal();
	bool write(std::string signal);
	std::string read();
	System::IO::Ports::SerialPort^ getSerialPort();

private:
	System::IO::Ports::SerialPort^ m_serialPort;//port PC

};
#endif

