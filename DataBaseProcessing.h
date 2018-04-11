#pragma once
#ifndef  DATABASEPROCESSING_HEADER
#define DATABASEPROCESSING_HEADER
#ifndef _DEBUG
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "../config_anprs.h"
#include "DataPlate.h"


class DataBaseProcessing
{
public:
	DataBaseProcessing(const std::string & ServerAddr, const std::string & user = "", const std::string & pass = "");
	~DataBaseProcessing();
	sql::ResultSet* Update(const std::string & database = "", const std::string & query = "");
	void Insert(DataPlate* dataPlate);	
private:
	sql::Connection *con;
};
#endif//_DEBUG
#endif//_DATABASEPROCESSING_HEADER