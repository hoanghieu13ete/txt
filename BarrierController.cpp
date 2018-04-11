#include <stdlib.h>
#include "BarrierController.h"
#include "../config_anprs.h"
#include "../config.h"
#include "Log.h"
#include <msclr\marshal_cppstd.h>
#include <fstream>
#include <iostream>


using namespace System::IO::Ports;
using namespace std;


BarrierController::BarrierController()
{
#ifdef USING_ADAM
	m_adamController = gcnew AdamController();
	serialPortMananger = gcnew SerialPortManager();

	m_adamController->connectToAdam();
	m_adamController->refreshAdam();
#endif
	setBarrierOpening(false);
	//m_isWaiting = false;
}
BarrierController::~BarrierController()
{
	//SAFE_DELETE(m_timer);
}
/*
This function is to call open barrier
Param:
Output:
*/
void BarrierController::openBarrier()
{
	setBarrierOpening(true);

#ifdef USING_ADAM
	m_adamController->openBarrier();
#else
	//open barrier signal
	serialPortMananger->writeSignal(OPEN_SIGNAL);

#endif
}
/*
This function is to call close barrier
Param:
Output:
*/
void BarrierController::closeBarrier()
{
	
#ifdef USING_ADAM
	m_adamController->closeBarrier();
#else
	serialPortMananger->writeSignal(CLOSE_SIGNAL);
#endif
}
void BarrierController::closeBarrierAfterSecond()
{
	_sleep(9000);
	closeBarrier();
	//wait 5s for barrier has closed
	_sleep(5000);
	//reset barrier open status
	setBarrierOpening(false);
}
/*
This function is to return state of barrier
Param:
Output: return true if barrier is opening, return false if barrier has closed
*/
bool BarrierController::barrierIsOpening()
{
	return m_isBarrierOpening;
}
void BarrierController::setBarrierOpening(bool status)
{
	m_isBarrierOpening = status;
}
/*
This function is to return waiting state of threading read signal from Arduino
Param:
Output: return true if is waiting signal, otherwise return false
*/
bool BarrierController::isWaitingForSignal()
{
	return /*m_isWaiting*/ true;
}
/*
This function is to read signal from Arduino
Param:
Output:
*/
void BarrierController::readSignal()
{

	if (!barrierIsOpening())
	{
		return;
	}
	//m_isWaiting = true;

	string dataReadFromArduino = serialPortMananger->read();

	LOG("Barrier has " + dataReadFromArduino);
	if(!strcmp(dataReadFromArduino.c_str(),""))
	{ 
		if (strcmp(dataReadFromArduino.c_str(), "Closed"))
		{
			setBarrierOpening(false);
		}
		//m_isWaiting = false;
	}

}

void BarrierController::writeFingerprintID(const int & id, const string & mode)
{
	System::IO::Ports::SerialPort^ serialPort;
	if (mode == "Eroll fingerprint") {
		serialPortMananger->write("E");
		string dataReadFromArduino = serialPortMananger->read();
		if (strcmp(dataReadFromArduino.c_str(), "Enable Eroll mode"))
		{
			cout << dataReadFromArduino << endl;
			serialPortMananger->write(to_string(id));
		}
		serialPort = serialPortMananger->getSerialPort();		
		serialPort->Close();
	}
}

void BarrierController::readFingerprintTemplate()
{
	System::IO::Ports::SerialPort^ serialPort;
	ofstream FingerprintTemplateFile(FINGERPRINT_PATH);
	while (true) {
		string dataReadFromArduino = serialPortMananger->read();
		if (dataReadFromArduino.find("0x") != std::string::npos) {
			FingerprintTemplateFile << dataReadFromArduino << endl;
		}
		else
		{
			cout << dataReadFromArduino << endl;
		}
		if (dataReadFromArduino == "") {
			break;
		}
	}
	FingerprintTemplateFile.close();
	serialPort = serialPortMananger->getSerialPort();
	serialPort->Close();
}

//void BarrierController::downloadTemplateFromServer()
//{
//	System::IO::Ports::SerialPort^ serialPort;
//	ifstream FingerprintTemplateFile;
//	string data, FingerTemplate;
//	FingerprintTemplateFile.open(FINGERPRINT_PATH);
//	if (FingerprintTemplateFile.is_open()) {
//		while (!FingerprintTemplateFile.eof()) {
//			FingerprintTemplateFile >> data;
//			FingerTemplate += data;
//		}
//	}
//	FingerprintTemplateFile.close();
//	serialPortMananger->write("C");
//	string dataReadFromArduino = serialPortMananger->read();
//	if (strcmp(dataReadFromArduino.c_str(), "Verify Fingerprint mode"))
//	{
//		cout << dataReadFromArduino << endl;
//	}
//	serialPort = serialPortMananger->getSerialPort();
//	serialPort->Close();
//}

