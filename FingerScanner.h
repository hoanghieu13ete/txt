#pragma once
#include<iostream>
#include<dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>
#include <conio.h>
#include <fstream>
#include <string>

using namespace std;

class FingerScanner
{
public:
	FingerScanner();
	~FingerScanner();
	static bool OpenReader();
	static bool CloseReader();
	void CaptureFinger();
	void CaculateFMDFromFID();
	bool CompareFMD(unsigned char* fmd, unsigned int size);
	unsigned char* GetFMD();
	unsigned int GetSizeFMD();
	string FMDToString();
	bool CompareStringFMD(string data);
private:
	static DPFPDD_DEV dev;
	unsigned int nFeaturesSize = MAX_FMD_SIZE;
	unsigned char* pFeatures = new unsigned char[nFeaturesSize];
	DPFPDD_CAPTURE_CALLBACK_DATA_0 data = { 0 };
};

void StringToFMD(string data, unsigned char* fmd, unsigned int &size);