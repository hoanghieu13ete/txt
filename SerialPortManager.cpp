#include "SerialPortManager.h"
#include "../config_anprs.h"
#include "config.h"


using namespace System;
using namespace System::IO::Ports;
using namespace std;

SerialPortManager::SerialPortManager()
{
	m_serialPort = gcnew SerialPort();
	m_serialPort->BaudRate = BAUDRATED;
	m_serialPort->PortName = PORT_ARDUINO;
}

SerialPortManager::~SerialPortManager()
{
	throw gcnew System::NotImplementedException();
}

//bool SerialPortManager::writeSignal(std::string singal)
//{
//
//	if (m_serialPort->IsOpen)
//	{
//		String^ str = gcnew String(singal.c_str());
//		m_serialPort->WriteLine(str);
//		return true;
//	}
//	else
//	{
//		//COM doesn't exist
//	}
//	
//	return false;
//}
//
//std::string SerialPortManager::readSignal()
//{
//	std::string data;
//	if (m_serialPort->IsOpen)
//	{
//		data = msclr::interop::marshal_as<std::string>(m_serialPort->ReadLine());
//	}	
//	else 
//	{
//		//COM doesn't exist
//	}
//	return "";
//}

bool SerialPortManager::write(std::string signal)
{
	if (m_serialPort->IsOpen)
	{
		String^ str = gcnew String(signal.c_str());
		m_serialPort->WriteLine(str);
		return true;
	}
	else
	{
		//COM doesn't exist
	}
	return false;
}

std::string SerialPortManager::read()
{
	std::string data;
	if (m_serialPort->IsOpen)
	{
		data = msclr::interop::marshal_as<std::string>(m_serialPort->ReadLine());
		return data;
	}
	else
	{ 
		//COM doesn't exist
	}

	return "";
}

System::IO::Ports::SerialPort ^ SerialPortManager::getSerialPort()
{
	return this->m_serialPort;
}
