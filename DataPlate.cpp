#include "DataPlate.h"

using namespace std;

DataPlate::DataPlate()
{
	this->plate_number = "";
	this->checkin = "";
	this->checkout = "";
	this->fingerprint_in = "";
	this->fingerprint_out = "";
	this->date_in = "";
	this->date_out = "";
	this->pic_in = "";
	this->pic_out = "";
	this->confidence_in = 0.0;
	this->confidence_out = 0.0;
}

DataPlate::DataPlate(int id)
{
}

int DataPlate::getId()
{
	return this->id;
}

void DataPlate::setId(int id)
{
	this->id = id;
}

string DataPlate::getPlate_number()
{
	return this->plate_number;
}

void DataPlate::setPlate_number(string plate_number)
{
	this->plate_number = plate_number;
}

string DataPlate::getCheckinState()
{
	return this->checkin;
}

void DataPlate::setCheckinState(string checkin)
{
	this->checkin = checkin;
}

string DataPlate::getCheckoutState()
{
	return this->checkout;
}

void DataPlate::setCheckoutState(string checkout)
{
	this->checkout = checkout;
}

string DataPlate::getFingerprint_inPath()
{
	return this->fingerprint_in;
}

void DataPlate::setFingerprint_inPath(string fingerprint_in)
{
	this->fingerprint_in = fingerprint_in;
}

string DataPlate::getFingerprint_outPath()
{
	return this->fingerprint_out;
}

void DataPlate::setFingerprint_outPath(string fingerprint_out)
{
	this->fingerprint_out = fingerprint_out;
}

string DataPlate::getDate_in()
{
	return this->date_in;
}

void DataPlate::setDate_in(string date_in)
{
	this->date_in = date_in;
}

string DataPlate::getDate_out()
{
	return this->date_out;
}

void DataPlate::setDate_out(string date_out)
{
	this->date_out = date_out;
}

string DataPlate::getPic_inPath()
{
	return this->pic_in;
}

void DataPlate::setPic_inPath(string pic_in)
{
	this->pic_in = pic_in;
}

string DataPlate::getPic_outPath()
{
	return this->pic_out;
}

void DataPlate::setPic_outPath(string pic_out)
{
	this->pic_out = pic_out;
}

float DataPlate::getConfidence_in()
{
	return this->confidence_in;
}

void DataPlate::setConfidence_in(float confidence_in)
{
	this->confidence_in = confidence_in;
}

float DataPlate::getConfidence_out()
{
	return this->confidence_out;
}

void DataPlate::setConfidence_out(float confidence_out)
{
	this->confidence_out = confidence_out;
}

void DataPlate::setFMDid(int id)
{
	fingerprint_id = id;
}

int DataPlate::getFMDid()
{
	return fingerprint_id;
}
