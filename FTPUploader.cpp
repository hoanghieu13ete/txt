#include "FTPUploader.h"
#include <stdio.h>
#include <curl/curl.h>
#include <io.h> 
#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "Log.h"
#include "../config_anprs.h"



static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{

	size_t retcode = fread(ptr, size, nmemb, stream);
	LOG("*** We read " + to_string(retcode) + " bytes from file");
	return retcode;
}

FTPUploader::FTPUploader()
{
}

FTPUploader::~FTPUploader()
{
}
/*This function to upload a file to server host over ftp transfer
Param: 
	const std::string& fileName
Output: 
	bool: True if upload completed, False if error
*/
bool FTPUploader::uploadFile(const std::string& fileName)
{

	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	double speed_upload, total_time;
	curl_off_t fsize;


	struct curl_slist *headerlist = NULL;
	//static const char buf_1[] = "RNFR " UPLOAD_FILE_AS;
	//static const char buf_2[] = "RNTO " RENAME_FILE_TO;

	/* get the file size of the local file */
	if (stat(fileName.c_str(), &file_info)) {
		LOG("Couldnt open file " + fileName);
		return false;
	}
	fsize = (curl_off_t)file_info.st_size;

	LOG("Local file size: " + to_string(fsize) + "bytes");

	/* get a FILE * of the same file */
	hd_src = fopen(fileName.c_str(), "rb");

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* build a list of commands to pass to libcurl */
		//headerlist = curl_slist_append(headerlist, buf_1);
		//headerlist = curl_slist_append(headerlist, buf_2);

		/* we want to use our own read function */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

		/* enable uploading */
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		string removeUrl = FTP_HOST + fileName;
		/* specify target */
		curl_easy_setopt(curl, CURLOPT_URL, removeUrl.c_str());

		/* pass in that last of FTP commands to run after the transfer */
		curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

		/* now specify which file to upload */
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

		/* specify username and password for ftp */
		curl_easy_setopt(curl, CURLOPT_USERNAME, USER);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, PASS);

		/* Set the size of the file to upload (optional).  If you give a *_LARGE
		option you MUST make sure that the type of the passed-in argument is a
		curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
		make sure that to pass in a type 'long' argument. */
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
			(curl_off_t)fsize);

		/* enable verbose for easier tracing */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		/* Now run off and do what you've been told! */
		res = curl_easy_perform(curl);

		/* now extract transfer info */
		curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

		LOG("Speed:" + to_string(speed_upload) + " bytes/sec during " + to_string(total_time) + " seconds");

		/* clean up the FTP commands list */
		curl_slist_free_all(headerlist);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	fclose(hd_src); /* close the local file */
	curl_global_cleanup();
	if (res != CURLE_OK)
	{
		return false;
	}
	return true;
}
