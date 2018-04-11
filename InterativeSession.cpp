#include "ThreadProcessing.h"
#include "InterativeSession.h"
#include <stdlib.h>
#include "DataPlate.h"
#include "Log.h"
#include "FingerScanner.h"

using namespace System::Threading;

InterativeSession::InterativeSession()
{
	
}
InterativeSession::~InterativeSession()
{
	throw gcnew System::NotImplementedException();
}
/*
This function is to checking whenever motobike plate has detected
Params:
Output:
*/
#ifndef _DEBUG
bool InterativeSession::checkingMotobiker(DailyDataManager * dailyDataManager, BarrierController^ barrier, const std::string plate, const std::string pictureName, float probability)
{
	if (m_threadBarrier)
	{
		m_threadBarrier->Join();//stop
	}
	DataPlate *plateData = DBG_NEW DataPlate();
	plateData->setPlate_number(plate);
	plateData->setCheckinState("yes");
	plateData->setDate_in(Log::getCurrentTimeAsString());
	plateData->setPic_inPath(pictureName);
	plateData->setConfidence_in(probability);
	//checking if motobike has check out
	EntryData result = dailyDataManager->CheckPlateState(plateData);


	switch (result.state)
	{
		case CHECKED_OUT: //case 3: if motobiker existed and last check out
						 //same as case not exist
		case NOT_EXIST: //case 1: if motobiker not exist on database

			//capture fingerprint
			if (FingerScanner::OpenReader())
			{
				FingerScanner finger;
				finger.CaptureFinger();
				finger.CaculateFMDFromFID();

				//store data in DB
				string data = finger.FMDToString();

				//get fingerprint_id
				int FMDid = dailyDataManager->AddFMD(data);


				//update to checkin state
				plateData->setFMDid(FMDid);

				//open barrier
				//barrier->openBarrier();
				//m_threadBarrier = gcnew Thread(gcnew ThreadStart(barrier, &BarrierController::closeBarrierAfterSecond));
				//m_threadBarrier->Start();//run thread

				//insert plate check in
				dailyDataManager->UpdateCheckInState(plateData);

			}
			else//error when open fingerscanner
			{

			}

			break;
		
		case CHECKED_IN: //case 2: if motobiker existed and last check in
			//checking if plate check out and finderprint are mapping
			
			//open barrier if correct


			//else show error warning

			//update database
			break;

		case DETECT_ERROR:
			//show error infor
			break;
		default:
			break;
	}	

	SAFE_DELETE(plateData);
	//if check in parking system
	return true;
}
#endif