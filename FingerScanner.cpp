#include "FingerScanner.h"

DPFPDD_DEV FingerScanner::dev = NULL;

FingerScanner::FingerScanner()
{
}

FingerScanner::~FingerScanner()
{
	delete pFeatures;
}

unsigned char* FingerScanner::GetFMD()
{
	return pFeatures;
}

unsigned int FingerScanner::GetSizeFMD()
{
	return nFeaturesSize;
}

bool FingerScanner::OpenReader()
{
	unsigned int nReadersCnt = 0;
	DPFPDD_DEV_INFO* pReaders = NULL;

	dpfpdd_init();

	while (DPFPDD_E_MORE_DATA == dpfpdd_query_devices(&nReadersCnt, pReaders)) {
		if (NULL != pReaders)
		{
			delete[] pReaders;
		}
		pReaders = new DPFPDD_DEV_INFO[nReadersCnt];
		if (NULL == pReaders) {
			nReadersCnt = 0;
			break;
		}
		else
			pReaders[0].size = sizeof(DPFPDD_DEV_INFO);
	}
	if (pReaders == NULL)
		return false;

	if (DPFPDD_SUCCESS == dpfpdd_open(pReaders[0].name, &dev)) {
		return true;
	}
	else
	{
		return false;
	}
}

void FingerScanner::CaptureFinger()
{
	data.capture_parm.size = sizeof(data.capture_parm);
	data.capture_parm.image_fmt = DPFPDD_IMG_FMT_ISOIEC19794;
	data.capture_parm.image_res = 500;
	data.capture_parm.image_proc = DPFPDD_IMG_PROC_NONE;
	data.capture_result.info.size = sizeof(data.capture_result.info);
	data.capture_result.size = sizeof(data.capture_result);
	data.image_size = 0;
	data.image_data = new unsigned char[140000];

	while (1) {
		if (dpfpdd_capture(dev, &data.capture_parm, -1, &data.capture_result, &data.image_size, data.image_data) == DPFPDD_SUCCESS) {
			break;
		}
	}
}

void FingerScanner::CaculateFMDFromFID()
{
	dpfj_create_fmd_from_fid(data.capture_parm.image_fmt, data.image_data, data.image_size, DPFJ_FMD_ISO_19794_2_2005, pFeatures, &nFeaturesSize);
}

bool FingerScanner::CompareFMD(unsigned char* fmd, unsigned int size)
{
	unsigned int falsematch_rate = 0;

	if (dpfj_compare(DPFJ_FMD_ISO_19794_2_2005, pFeatures, nFeaturesSize, 0, DPFJ_FMD_ISO_19794_2_2005, fmd, size, 0, &falsematch_rate) == DPFJ_SUCCESS) {

		const unsigned int target_falsematch_rate = DPFJ_PROBABILITY_ONE / 100000;
		if (falsematch_rate < target_falsematch_rate) /*compare condition*/ {
			return true;
		}
		else
			return false;
	}
}

string FingerScanner::FMDToString()
{
	string result = to_string(pFeatures[0]);

	for (int i = 1; i < nFeaturesSize; i++)
	{
		result += ",";
		result += to_string(pFeatures[i]);
	}

	return result;
}

void StringToFMD(string data, unsigned char* fmd, unsigned int &size)
{
	int i = 1;
	while (1)
	{
		string split = data.substr(0, data.find(","));
		fmd[i - 1] = stoi(split);
		if (split.size() == data.size())
			break;
		data = data.substr(data.find(",") + 1, data.size());
		i++;
	}
	size = i;
}

bool FingerScanner::CompareStringFMD(string data)
{
	unsigned int size = MAX_FMD_SIZE;
	unsigned char* fmd = new unsigned char[size];
	StringToFMD(data, fmd, size);

	if (CompareFMD(fmd, size))
	{
		delete fmd;
		return true;
	}
	else
	{
		delete fmd;
		return false;
	}
}
