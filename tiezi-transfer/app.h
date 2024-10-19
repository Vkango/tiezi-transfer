#include <string>
#include "curl/curl.h"
#include "Windows.h"
#include<vector>
using namespace std;
struct MediaObject {
    string URL;
    string show_name;
};
class TieBaAPI 
{
public:
    string CalcSign(const string &data);
    string Post(const string& tid, const int& page);
    string SubPost(const string& tid, const int& post_id);
};
class URL_Code
{
public:
    string encode(const string &data);
    string decode(const string &data);
};
std::string MD5(string text);
std::string ReadFile(const string& filename);
bool WriteFile(const string& filename, const string& write_text);
struct Tiezi_Info {
    string title;
    int page;
};
Tiezi_Info GetTieZiInfo(const string json_str);
vector<MediaObject> GetMediaInfo(const string json_str);
std::wstring utf8_to_w(string str);
class HttpConnection {
public:
    HttpConnection();
    ~HttpConnection();
    bool Post(const std::string& url, const std::string& data, std::string& response);
    bool Get(const std::string& url, std::string& response);
private:
    CURL* curl_ = nullptr;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};