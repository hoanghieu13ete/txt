#pragma once
#ifndef  INTERATIVESESSION_HEADER
#define INTERATIVESESSION_HEADER
#include <string>
#include "BarrierController.h"
#include "DailyDataManager.h"

ref class InterativeSession
{
public:
	InterativeSession();
	~InterativeSession();
#ifndef _DEBUG
	static bool checkingMotobiker(DailyDataManager * dailyDataManager, BarrierController^ barrier, const std::string plate, const std::string pictureName, float probability);
#endif
private: 
	static System::Threading::Thread ^ m_threadBarrier;
};

#endif//INTERATIVESESSION_HEADER