#include <iostream>
#include "lang.h"
#include "app.h"
#include <stdlib.h> 
#include "Windows.h"
using namespace std;

std::string wstringToString(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
    return strTo;
}



int main()
{
    std::locale::global(std::locale("zh_CN.UTF-8"));
    TieBaAPI c;
    Tiezi_Info t;
    cout << wstringToString(utf8_to_w(c.Post("5240906135", 1))) << endl;
    t = GetTieZiInfo(wstringToString(utf8_to_w(c.Post("5240906135", 1))));
    for (int i = 1; i <= t.page; i++)
    {
        wcout << L"正在进行第" << i << L"页JSON保存" << endl;
    }
    vector<MediaObject> res = GetMediaInfo(wstringToString(utf8_to_w(c.Post("5240906135", 1))));
    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i].URL << " " << res[i].show_name << " " << res[i].show_name.substr(0, 2) << endl;
    }
    cout << t.title << endl << t.page << endl;
    return 0;
}
