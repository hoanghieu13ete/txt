#include "AdamController.h"
#define IP "10.133.40.130"
#include <Windows.h>

using namespace System::Net::Sockets;
using namespace Advantech::Adam;
using System::Windows::Forms::MessageBox;

AdamController::AdamController()
{
	//int m_iDoTotal, m_iDiTotal, m_iCount;
	m_iPort = 502;				// modbus TCP port is 502
	m_adamModbus = gcnew AdamSocket();
	m_adamModbus->SetTimeout(1000, 1000, 1000); // set timeout for TCP
	m_Adam6000Type = Adam6000Type::Adam6060; // the sample is for ADAM-6060

}

AdamController::~AdamController()
{
	m_adamModbus->Disconnect(); // disconnect slave
}

bool AdamController::connectToAdam()
{
	//m_isConnected = m_adamModbus->Connect(IP, ProtocolType::Tcp, m_iPort);//connect to salve
	if (m_adamModbus->Connect(IP, ProtocolType::Tcp, m_iPort))
	{
		refreshAdam();
		return true;
	}
	return false;
}

void AdamController::openBarrier()
{
	if (m_adamModbus->Connect(IP, ProtocolType::Tcp, m_iPort))
	{
		array<bool> ^data = { 0,1 };//17-0,18-1
		if (m_adamModbus->Modbus()->ForceMultiCoils(17, data))
		{
			m_signalOpen = true;
		}
		else
		{
			MessageBox::Show("Set digital output failed!", "Error");
		}
	}
}

void AdamController::closeBarrier()
{
	if (m_adamModbus->Connect(IP, ProtocolType::Tcp, m_iPort))
	{
		if (m_signalOpen)
		{
			array<bool> ^data = { 1,0 };;//17-1,18-0
			if (m_adamModbus->Modbus()->ForceMultiCoils(17, data))
			{
				//Sleep(1000);
				//refreshAdam();
			}
			else
			{
				MessageBox::Show("Set digital output failed!", "Error");
			}
			
		}
	}
}

void AdamController::refreshAdam()
{
	if (m_adamModbus->Connect(IP, ProtocolType::Tcp, m_iPort))
	{
		array<bool> ^data = { 1,0 };
		if (m_adamModbus->Modbus()->ForceMultiCoils(17, data))
		{
			m_signalOpen = false;
		}
		else
		{
			MessageBox::Show("Cant refresh port DO0 & DO1!", "Error");
		}
	}
}
