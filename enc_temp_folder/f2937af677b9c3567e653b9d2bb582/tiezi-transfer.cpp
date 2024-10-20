#include <iostream>
#include "lang.h"
#include "app.h"
#include <stdlib.h> 
#include "Windows.h"
#include "thread-pool.h"
using namespace std;

string download_path = "";

std::string wstringToString(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
    return strTo;
}

void download(const MediaObject& obj) {
    HttpConnection Conn;
    cout << obj.show_name << " " << obj.URL << endl;
    Conn.DownloadFile(obj.URL, download_path + "\\res\\" + obj.show_name);
}

int main()
{
    std::locale::global(std::locale("zh_CN.UTF-8"));
    TieBaAPI c;
    Tiezi_Info t;
    wcout << L"指定下载目录" << endl;
    cin >> download_path;
    wcout << L"请输入要保存贴子的ID" << endl;
    string b;
    cin >> b;
    string page1 = wstringToString(utf8_to_w(c.Post(b, 1)));
    download_path += "\\";
    download_path += b;
    download_path += "\\";
    cout << download_path << endl;
    t = GetTieZiInfo(page1);
    char ok_to_save;
    wcout << L"你指的是：【";
    cout << t.title;
    wcout << L"】吗？如果是请输入Y或y。" << endl;
    cin >> ok_to_save;
    if (ok_to_save != 'Y' && ok_to_save != 'y') {
        wcout << L"以退出。" << endl;
        return 0;
    }
    string page;
    CreatePath(download_path);
    CreatePath(download_path + "\\res");
    
    wcout << L"注意：先逐个保存每一页内容，之后才会进行资源下载。" << endl;
    vector<MediaObject> res;
    vector<SubPost> sp;
    for (int i = 1; i <= t.page; i++)
    {
        wcout << L"正在进行第" << i << L"页JSON保存并分析资源列表...";
        page = c.Post(b, i);
        WriteFile(download_path + to_string(i) + ".json", page);
        vector<MediaObject> re = GetMediaInfo(page);
        copy(re.begin(), re.end(), back_inserter(res)); //合并大数组
        vector<SubPost> sps = ReplyInfo(b, page);
        copy(sps.begin(), sps.end(), back_inserter(sp)); //合并大数组

        wcout << L" [OK]" << endl;
    }
    
    wcout << L"即将进行资源保存，资源数：" << res.size() << endl << L"以及楼中楼保存，共有：" << sp.size() << endl << L"组" << L"保存请输入Y或y。" << endl;
    cin >> ok_to_save;
    if (ok_to_save != 'Y' && ok_to_save != 'y') {
        wcout << L"以退出。" << endl;
        return 0;
    }
    ThreadPool pool(1);
    HttpConnection x;
    for (int i = 0; i < res.size(); i++)
    {
        //x.DownloadFile(res[i].URL, download_path + "\\res\\" + res[i].show_name);
        pool.submit(std::bind(download, res[i]));
    }
    while (!pool.has_tasks()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    wcout << L"已完成。" << endl;
        // 循环等待任务是否完成
    
    return 0;
}
