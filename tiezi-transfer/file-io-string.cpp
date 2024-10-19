#include <fstream> 
#include "app.h"
string ReadFile(const string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw 1; // ERROR 1 �޷����ļ�
    }

    string line,text;
    while (std::getline(inFile, line)) {
        text += line;
    }

    inFile.close();

    return text;
}
bool WriteFile(const string& filename, const string& write_text) {
    std::ofstream fileStream(filename);
    if (!fileStream.is_open()) {
        return false;
    }

    fileStream << write_text;
    //fileStream << "����ʹ��C++д����ı���\n";

    // �ر��ļ���
    fileStream.close();
    return true;

}