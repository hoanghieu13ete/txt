#include "DataBaseProcessing.h"
#include "config.h"
#ifndef _DEBUG

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace std;
using namespace sql;
using namespace sql::mysql;

DataBaseProcessing::DataBaseProcessing(const string & ServerAddr, const string & user, const string & pass) {

	sql::Driver *driver;
	try {
		driver = get_driver_instance();
		con = driver->connect(ServerAddr, user, pass);
	}
	catch (SQLException &e) {
		MessageBox::Show("Cannot connect to database !", e.getErrorCode().ToString());
	}
}

DataBaseProcessing::~DataBaseProcessing() {
	SAFE_DELETE(con);
}

/*
This function is to implement a query to a database
Param:
Connection* con: database connection
const string & database: name of database
const string & query: query
Return:
ResultSet* : result
*/
ResultSet* DataBaseProcessing::Update(const string & database, const string & query) {
	Statement *stmt;
	ResultSet *res;

	try {
		con->setSchema(database);
		stmt = con->createStatement();
		res = stmt->executeQuery(query);
	}
	catch (SQLException &e) {
#ifndef _DEBUG
		if (e.getErrorCode())
		{
			cout << "Error code " << e.getErrorCode() << ":" << e.what() << endl;
		}
#endif
	}
	return res;
}

/*
This function is to insert a record date to a row of a database
Param:
Connection* con: database connection
DataPlate* dataPlate : pointer to data plate object
*/
void DataBaseProcessing::Insert(DataPlate* dataPlate) {

	string query, statement, data;
	string tablename = SESSION_TABLE;
	statement = "INSERT INTO " + tablename + " (plate_number, checkin, checkout, fingerprint_in, fingerprint_out, date_in, date_out, pic_in, pic_out, confidence_in, confidence_out, fingerprint_id)"
				+ " VALUES ";
	data = " ('" + dataPlate->getPlate_number() + "','" + dataPlate->getCheckinState() + "','" + dataPlate->getCheckoutState() + "','" + dataPlate->getFingerprint_inPath() + "','" + dataPlate->getFingerprint_outPath()
			+ "','" + dataPlate->getDate_in() + "','" + dataPlate->getDate_out() + "','" + dataPlate->getPic_inPath() + "','" + dataPlate->getPic_outPath() + "','" + to_string(dataPlate->getConfidence_in()) + "','"
			+ to_string(dataPlate->getConfidence_out()) + to_string(dataPlate->getFMDid())+ "')";
	query = statement + data;
	Update(USER, query);
}
#endif