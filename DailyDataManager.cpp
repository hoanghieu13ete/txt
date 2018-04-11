#include "DailyDataManager.h"

#ifndef _DEBUG
using namespace std;
using namespace sql;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;


DailyDataManager::DailyDataManager()
{
	DBProcess = DBG_NEW DataBaseProcessing(DBSERVER_HOST, USER, PASS);
}

DailyDataManager::~DailyDataManager()
{
	for (auto datarow : dataPlates)
	{
		delete datarow;
	}
	dataPlates.clear();
	delete DBProcess;
	DBProcess = NULL;
}

/*
This function is to get Plate data base by date
Param:
Connection * con: database connection
const string & date : query date
Return:
ResultSet* res: result of Sql data base query by date
*/
void DailyDataManager::getDataInforbyDate(const string & date)
{

	string table_name = SESSION_TABLE;
	string query = "SELECT DISTINCT * FROM " + table_name + " WHERE " + "(SELECT DATE(date_in) = '" + date
		+ "') OR " + "(SELECT DATE(date_out) = '" + date + "')";
	ResultSet* res;
	res = DBProcess->Update(USER, query);
	ConvertDataFromResultSet(res);
}

/*
This function is to display Database information by date to DataGridView
Param:
DataGridView^ dataGridView: DataGridView to be displayed
ResultSet* res: a return result of Sql data base query by date
Return:
*/
void DailyDataManager::DisplayDataInfor(DataGridView ^ dataGridView)
{
	string query, plate_number, checkin, checkout, datein, dateout, curdate;
	String ^plate_number1, ^checkin1, ^checkout1, ^datein1, ^dateout1;
	int rowIdx = 0;
	dataGridView->Rows->Clear();
	cout << "capacity:" << dataPlates.capacity() << endl;
	rowIdx = dataPlates.size();
	for (auto dataRow : dataPlates) {
		//DataPlate *dataRow = dataPlates.at(i);
		if (dataRow)
		{
			dataGridView->Rows->Insert(0);//add new row to top

			//fill data into view
			dataGridView->Rows[0]->Cells[0]->Value = gcnew String(to_string(rowIdx).c_str());

			dataGridView->Rows[0]->Cells[1]->Value = gcnew String(dataRow->getPlate_number().c_str());
			dataGridView->Rows[0]->Cells[2]->Value = gcnew String(dataRow->getCheckinState().c_str());
			dataGridView->Rows[0]->Cells[3]->Value = gcnew String(dataRow->getCheckoutState().c_str());
			dataGridView->Rows[0]->Cells[4]->Value = gcnew String(dataRow->getDate_in().c_str());
			if (dataRow->getDate_out() != "0000-00-00 00:00:00") {
				dataGridView->Rows[0]->Cells[5]->Value = gcnew String(dataRow->getDate_out().c_str());
			}			
		}
		if (rowIdx > 0) {
			rowIdx--;
		}
	}
	
	dataGridView->Rows[0]->DefaultCellStyle->BackColor = Color::Yellow;
}

/*
This function is to check Plate number state
Param:
Connection* con : database connection
DataPlate* dataPlate : pointer to plate data object
Return:
EntryData (NOT_EXIST : plate number not exist in database
CHECKED_IN : plate number exist in database, checked in but not checked out
CHECKED_OUT : plate number exist in database, checked out
ERROR : error state)

*/
EntryData DailyDataManager::CheckPlateState(DataPlate * dataPlate)
{
	string tablename = SESSION_TABLE;
	string query = "SELECT id,plate_number,checkin,checkout FROM " + tablename;
	ResultSet *res;
	EntryData entrydata;
	entrydata.id = 0;

	res = DBProcess->Update(USER, query);
	while (res->next()) {
		if (dataPlate->getPlate_number() == res->getString("plate_number")) {
			if (res->getString("checkin") == "yes" && res->getString("checkout") == "") {
				entrydata.state = CHECKED_IN;
				entrydata.id = stoi(res->getString("id"));
				return entrydata;
				break;
			}
			else if (res->getString("checkin") == "yes" && res->getString("checkout") == "yes") {
				entrydata.state = CHECKED_OUT;
				return entrydata;
				break;
			}
			else
			{
				entrydata.state = DETECT_ERROR;
				return entrydata;
				break;
			}
		}
	}
	
	entrydata.state = NOT_EXIST;//for 2 case: not exsit and db is empty

	return entrydata;
}

/*
This function is to update check out state when a motorbike check out
Param:
Connection* con : database connection
DataPlate* dataPlate : pointer to plate data object
DataGridView^ dataGridView : Data gridview  to be displayed
Return:
*/
void DailyDataManager::UpdateCheckOutState(DataPlate * dataPlate, DataGridView ^ dataGridView)
{
	string tablename = SESSION_TABLE;
	string 	query = "INSERT INTO " + tablename + " (checkout, date_out)" + " VALUES " + "(" + "'" + dataPlate->getCheckoutState() + "','" + dataPlate->getDate_out() + "')" +
		" WHERE id = " + to_string(dataPlate->getId());
	ResultSet* res;
	res = DBProcess->Update(USER, query);
	DisplayDataInfor(dataGridView);
}

void DailyDataManager::ConvertDataFromResultSet(ResultSet* result)
{
	dataPlates.clear();
	while (result->next())
	{
		DataPlate *data = DBG_NEW DataPlate();
		data->setId(result->getInt("id"));
		data->setPlate_number(result->getString("plate_number"));
		data->setFingerprint_inPath(result->getString("fingerprint_in"));
		data->setFingerprint_outPath(result->getString("fingerprint_out"));
		data->setCheckinState(result->getString("checkin"));
		data->setCheckoutState(result->getString("checkout"));
		data->setDate_in(result->getString("date_in"));
		data->setDate_out(result->getString("date_out"));
		data->setPic_inPath(result->getString("pic_in"));
		data->setPic_outPath(result->getString("pic_out"));
		data->setConfidence_in(result->getInt("confidence_in"));
		data->setConfidence_out(result->getInt("confidence_out"));
		data->setConfidence_out(result->getInt("fingerprint_id"));

		dataPlates.push_back(data);
	}
}
void DailyDataManager::UpdateCheckInState(DataPlate * dataPlate)
{
	DBProcess->Insert(dataPlate);
	dataPlates.push_back(dataPlate);
}
int DailyDataManager::AddFMD(const std::string & FingerprintTemplate)
{
	string tablename, query;
	int fmd_id;
	ResultSet* res;
	tablename = FINGER_TABLE;
	query = "INSERT INTO " + tablename + " (fingerprint_template)" + " VALUES " + "('" + FingerprintTemplate + "')";
	res = DBProcess->Update(USER, query);
	query = "SELECT fingerprint_id FROM " + tablename;
	res = DBProcess->Update(USER, query);
	fmd_id = res->getInt("fingerprint_id");
	return fmd_id;
}
std::string DailyDataManager::getFMDdata(const int & fmd_id)
{
	string tablename, query, fmd_data;
	ResultSet* res;
	tablename = FINGER_TABLE;
	query = "SELECT fingerprint_template FROM " + tablename + " WHERE id = " + to_string(fmd_id);
	res = DBProcess->Update(USER, query);
	fmd_data = res->getString("fingerprint_template");
	return fmd_data;
}
#endif