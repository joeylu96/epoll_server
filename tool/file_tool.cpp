#include "file_tool.h"

void CFileTool::genFileFromJson(string file_name, Json::Value& jsnText)
{
	ofstream os;
	os.open(file_name, ios::out);
	Json::FastWriter fast_writer;
	fast_writer.omitEndingLineFeed();
	string tmp_str = fast_writer.write(jsnText);
	os<<tmp_str;
	os.close();
}

int CFileTool::readJsonFile(string file_name, Json::Value& tmp)
{
	ifstream is;
	Json::Reader reader;
	is.open(file_name, ios::binary);

	if(reader.parse(is,tmp))
	{
		return 0;
	}
	return -1;
	is.close();
}
