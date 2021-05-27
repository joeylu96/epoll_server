#ifndef _FILE_TOOL_
#define _FILE_TOOL_

#include <fstream>
#include <string>
#include <json/json.h>

using namespace std;

class CFileTool
{
  public:
    static void genFileFromJson(string file_name, Json::Value& jsnText);
    static int readJsonFile(string file_name, Json::Value& tmp);
};

#endif