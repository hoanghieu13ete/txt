#pragma once
#ifndef DAILYDATAMANAGER_HEADER
#define DAILYDATAMANAGER_HEADER

#ifndef _DEBUG
#include <vector>
#include <fstream>
#include <iostream>
#include "DataBaseProcessing.h"


enum CheckInState {
	NOT_EXIST,
	CHECKED_IN,
	CHECKED_OUT,
	DETECT_ERROR
};
struct EntryData
{
	CheckInState state;
	int id;
};

class DailyDataManager
{
public:
	DailyDataManager();
	~DailyDataManager();
	void getDataInforbyDate(const std::string & date);/*function is to get Plate data base by date*/
	void DisplayDataInfor(System::Windows::Forms::DataGridView^ dataGridView);/*function is to display Database information by date to DataGridView*/
	EntryData CheckPlateState(DataPlate* dataPlate);/*function is to check Plate number state*/
	void UpdateCheckOutState(DataPlate* dataPlate, System::Windows::Forms::DataGridView^ dataGridView);/*function is to update check out state when a motorbike check out*/
	void ConvertDataFromResultSet(sql::ResultSet* result);
	void UpdateCheckInState(DataPlate* dataPlate);
	int AddFMD(const std::string& FingerprintTemplate);
	std::string getFMDdata(const int & fmd_id);
private:
	DataBaseProcessing* DBProcess;
	std::vector<DataPlate*> dataPlates;
};
#endif//_DEBUG
#endif//DAILYDATAMANAGER_HEADER

