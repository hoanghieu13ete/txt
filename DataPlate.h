#pragma once
#ifndef DATAPLATE_HEADER
#define DATAPLATE_HEADER
#include <stdlib.h>
#include <string>
#include <iostream>
#include "../config_anprs.h"

class DataPlate
{
public:
	DataPlate();
	DataPlate(int id);
	int getId();
	void setId(int id);
	std::string getPlate_number();
	void setPlate_number(std::string plate_number);
	std::string getCheckinState();
	void setCheckinState(std::string checkin);
	std::string getCheckoutState();
	void setCheckoutState(std::string checkout);
	std::string getFingerprint_inPath();
	void setFingerprint_inPath(std::string fingerprint_in);
	std::string getFingerprint_outPath();
	void setFingerprint_outPath(std::string fingerprint_out);
	std::string getDate_in();
	void setDate_in(std::string date_in);
	std::string getDate_out();
	void setDate_out(std::string date_out);
	std::string getPic_inPath();
	void setPic_inPath(std::string pic_in);
	std::string getPic_outPath();
	void setPic_outPath(std::string pic_out);
	float getConfidence_in();
	void setConfidence_in(float confidence_in);
	float getConfidence_out();
	void setConfidence_out(float confidence_out);
	
	void setFMDid(int id);
	int getFMDid();

private:
	int id;
	std::string plate_number;
	std::string checkin;
	std::string checkout;
	std::string fingerprint_in;
	std::string fingerprint_out;
	std::string date_in;
	std::string date_out;
	std::string pic_in;
	std::string pic_out;
	float confidence_in;
	float confidence_out;
	int fingerprint_id;
};
#endif

