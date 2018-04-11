#pragma once
#ifndef  BARRIERCONTROLLER_HEADER
#define BARRIERCONTROLLER_HEADER
#include "TimeProcessing.h"
#include "AdamController.h"
#include "SerialPortManager.h"


ref class BarrierController
{

public:
	BarrierController();
	~BarrierController();
	void openBarrier();
	void closeBarrier();
	void closeBarrierAfterSecond();
	bool barrierIsOpening();
	void setBarrierOpening(bool status);
	bool isWaitingForSignal();
	void readSignal();
	void writeFingerprintID(const int& id, const std::string & mode);
	void readFingerprintTemplate();
	//void downloadTemplateFromServer();

private:
	TimeProcessing* m_timer;
	long long m_timeCounter;
	bool m_isBarrierOpening;
	//bool m_isWaiting;
	AdamController^ m_adamController;
	SerialPortManager^ serialPortMananger;
};

#endif//BARRIERCONTROLLER_HEADER