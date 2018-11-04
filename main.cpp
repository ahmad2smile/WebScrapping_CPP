#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

CURLcode getPage(const char *url)
{
	CURL *curl = curl_easy_init();
	CURLcode res;
	curl_easy_setopt(curl, CURLOPT_URL, url);
	CURLcode page = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return page;
}

int main()
{
	CURLcode page = getPage("https://duckduckgo.com");
	std::cout << page;
	return 0;
}