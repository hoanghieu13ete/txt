#pragma once
#ifndef  FPTUPLOADER_HEADER
#define FPTUPLOADER_HEADER
#include <string>

class FTPUploader
{
public:
	FTPUploader();
	~FTPUploader();
	bool uploadFile(const std::string& fileName);
};
#endif // FPTUPLOADER_HEADER

