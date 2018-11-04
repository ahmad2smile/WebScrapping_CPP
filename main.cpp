#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <libxml/HTMLparser.h>

#ifdef _MSC_VER
#define COMPARE(a, b) (!_stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

struct Context
{
	Context() : addTitle(false) {}

	bool addTitle;
	std::string title;
};

static void StartElement(void *voidContext,
						 const xmlChar *name,
						 const xmlChar **attributes)
{
	Context *context = (Context *)voidContext;

	if (COMPARE((char *)name, "TITLE"))
	{
		context->title = "";
		context->addTitle = true;
	}
	(void)attributes;
}

//
//  libxml end element callback function
//

static void EndElement(void *voidContext,
					   const xmlChar *name)
{
	Context *context = (Context *)voidContext;

	if (COMPARE((char *)name, "TITLE"))
		context->addTitle = false;
}

//
//  Text handling helper function
//

static void handleCharacters(Context *context,
							 const xmlChar *chars,
							 int length)
{
	if (context->addTitle)
		context->title.append((char *)chars, length);
}

//
//  libxml PCDATA callback function
//

static void Characters(void *voidContext,
					   const xmlChar *chars,
					   int length)
{
	Context *context = (Context *)voidContext;

	handleCharacters(context, chars, length);
}

//
//  libxml CDATA callback function
//

static void cdata(void *voidContext,
				  const xmlChar *chars,
				  int length)
{
	Context *context = (Context *)voidContext;

	handleCharacters(context, chars, length);
}

static int writer(char *data, size_t size, size_t nmemb,
				  std::string *writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size * nmemb);

	return size * nmemb;
}

static htmlSAXHandler saxHandler =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		StartElement,
		EndElement,
		NULL,
		Characters,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		cdata,
		NULL};

static void parseHtml(const std::string &html,
					  std::string &title)
{
	htmlParserCtxtPtr ctxt;
	Context context;

	ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",
									XML_CHAR_ENCODING_NONE);

	htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
	htmlParseChunk(ctxt, "", 0, 1);

	htmlFreeParserCtxt(ctxt);

	title = context.title;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <url>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	std::string title;

	CURL *curl = curl_easy_init();
	std::string page;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &page);

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	parseHtml(page, title);

	printf("Title: %s", title.c_str());
	return 0;
}