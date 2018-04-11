#pragma once
#ifndef  HTTPDOWNLOADER_HEADER
#define HTTPDOWNLOADER_HEADER

#include <string>


class HTTPDownloader
{
public:
	HTTPDownloader();
	~HTTPDownloader();

	bool downLoad(const std::string& url, const std::string& savePath = "");

private:
	//static CURLcode resultCode;
};


#endif //HTTPDOWNLOADER_HEADER