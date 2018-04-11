#include "HTTPDownloader.h"
#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sstream>
#include <iostream>
#include "Log.h"
#include "FileProcessing.h"
#include "../config_anprs.h"

using namespace std;

size_t AppendDataToStringCurlCallback(void *ptr, size_t size, size_t nmemb, void *vstring)
{
	std::string * pstring = (std::string*)vstring;
	pstring->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

HTTPDownloader::HTTPDownloader()
{
}

HTTPDownloader::~HTTPDownloader()
{
}
/*
This function is download a file from server http url and save local
Param:
	const string & url: source file need download
	const string & savePath: destination location
*/
bool HTTPDownloader::downLoad(const string & url, const string & savePath)
{
	string content;
	CURL *curl_handle;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, AppendDataToStringCurlCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);
	CURLcode errorCode = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);

	if (savePath.empty())
	{
		return false;
	}
	//string saveFile = savePath + "/" +  FileProcessing::getFileName(url);
	//if (FileProcessing::fileExist(saveFile))
	//{
	//	LOG("Error: Save directory not exist!");
	//	return false;
	//}

	if (errorCode != CURLE_OK)
	{
		LOG("Download Error code: 0x");		
		return false;
	}
	LOG("Downloaded from " + url );
	ofstream out(savePath);
	out << content;
	out.close();
	return true;
}


