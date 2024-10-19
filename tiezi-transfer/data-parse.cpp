#include "app.h"
#include "YyJson/yyjson.h"
#include<string>
#include "Json.h"
Tiezi_Info GetTieZiInfo(const string json_str)
{
	Tiezi_Info result;
    CJson j2 = json_str;
	result.page = j2["/page/total_page"].GetInt();
	result.title = j2["/thread/title"].GetStr();
	return result;
}
string GetFileName(const string URL)
{

	size_t lastSlashPos = URL.find_last_of('/');
	size_t lastDotPos = URL.find_last_of('.');
	if (lastSlashPos != std::string::npos && lastDotPos != std::string::npos && lastSlashPos < lastDotPos) {
		std::string extractedText = URL.substr(lastSlashPos + 1, lastDotPos);
		return extractedText;
	}
	else {
		return "";
	}
}
vector<MediaObject> GetMediaInfo(const string json_str)
{
	vector<MediaObject> result;
	MediaObject tmp;
	CJson j = json_str;
	result.clear();
	for (int i = 0; i < j["/post_list"].GetLen(); i++)
	{
		for (int m = 0; m < j[("/post_list/" + std::to_string(i) + "/content").c_str()].GetLen(); m++)
		{
			switch (j[("/post_list/" + std::to_string(i) + "/content/" + std::to_string(m) + "/type").c_str()].GetInt())
			{
			case 3:
				tmp.URL = j[("/post_list/" + std::to_string(i) + "/content/" + std::to_string(m) + "/src").c_str()].GetStr();
				tmp.show_name = GetFileName(tmp.URL);
				result.push_back(tmp);
				break;
			case 5:
				tmp.URL = j[("/post_list/" + std::to_string(i) + "/content/" + std::to_string(m) + "/link").c_str()].GetStr();
				tmp.show_name = GetFileName(tmp.URL);
				result.push_back(tmp);
				break;
			default:
				break;
			}
		}
	};

	return result;
}