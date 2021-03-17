#include "fuckwindows.h"
#include "json11.hpp"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include "httplib.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

std::string foo(std::map<std::string, float> kv)
{
	json11::Json jo(kv);
	std::string temp = jo.dump();

	httplib::Client cli("localhost", 22222, 0);

	cli.Post("/bar", temp, "application/json");
    return temp;
}
