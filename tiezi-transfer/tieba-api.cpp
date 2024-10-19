#define _CRT_SECURE_NO_WARNINGS
#include "app.h"
#include<regex>

#include<string>

string TieBaAPI::CalcSign(const string& data) //返回原数据+&sign
{
    URL_Code u;
    string res;
    res = u.decode(data);
    regex pattern("&");
    res = regex_replace(res, pattern, "") + "tiebaclient!!!";
    res = MD5(res);
    return data + "&sign=" + res;
};
HttpConnection::HttpConnection() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_ = curl_easy_init();
};
HttpConnection::~HttpConnection() {
    curl_easy_cleanup(curl_);
    curl_global_cleanup();
};
int HttpConnection::DownloadFile(const std::string& url, const std::string& save_path)
{
    CURLcode res;
    FILE* fp;
    fp = fopen(save_path.c_str(), "wb");
    if (!fp) {
        return 1;
    }
    if (curl_) {
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl_);
    }
    fclose(fp);
    return 0;
}

bool HttpConnection::Post(const std::string& url, const std::string& data, std::string& response) {
    if (!curl_) {
        return false;
    }
    struct curl_slist* header_list = NULL;
    header_list = curl_slist_append(header_list, "User-Agent:Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.87 Safari/537.36");
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, header_list);
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl_);
    return (res == CURLE_OK);
};

bool HttpConnection::Get(const std::string& url, std::string& response) {
    if (!curl_) {
        return false;
    }
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_POST, 0L);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl_);
    return (res == CURLE_OK);
};

size_t HttpConnection::write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

size_t HttpConnection::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), realsize);
    return realsize;
};




string TieBaAPI::Post(const string& tid, const int& page)
{
    HttpConnection Conn;
    string response;
    Conn.Post("http://c.tieba.baidu.com/c/f/pb/page", CalcSign("_client_id=wappc_1001260722739_827&_client_version=9.1.1&kz=" + tid + "&net_type=1&pn=" + to_string(page)), response);
    
    return response;
};
string TieBaAPI::SubPost(const string& tid, const int& post_id)
{
    return "";
};
