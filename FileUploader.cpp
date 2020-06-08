#include<iostream>
#include<string>
#include <curl\curl.h>
#include "FileUploader.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

char* stringToCharstar(std::string s) {
	int len = s.length();
	char* res = new char[len + 1];
	for (int i = 0;i < len;i++)
		res[i] = s[i];
	res[len] = '\0';
	return res;
}

char* FileUploader :: uploadFile(char* url,char* filePath) {

	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	std::string readBuffer;

	/* get a curl handle */
	curl = curl_easy_init();

	if (curl) {

		/* First set the URL that is about to receive our POST. This URL can
		   just as well be a https:// URL if that is what should receive the
		   data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);


		/* Now specify the POST data */
		curl_mime *multipart = curl_mime_init(curl);
		curl_mimepart *part = curl_mime_addpart(multipart);
		curl_mime_name(part, "Document");
		curl_mime_filedata(part, filePath);


		/*Store the response from the server in a string*/
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


		/* Set the form info */
		curl_easy_setopt(curl, CURLOPT_MIMEPOST, multipart);


		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);


		/* Check for errors */
		if (res != CURLE_OK) {
			char* error = (char*)curl_easy_strerror(res);
			std::string errorStr(error);
			readBuffer = "curl_easy_perform() failed: " + errorStr;
		}


		/* free the post data again */
		curl_mime_free(multipart);


		/* always cleanup */
		curl_easy_cleanup(curl);

	}
	curl_global_cleanup();

	return stringToCharstar(readBuffer);
}