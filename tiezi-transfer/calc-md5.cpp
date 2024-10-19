#include "app.h"
#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>
#include <sstream>
#include <iomanip>
#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)
inline NTSTATUS calc(const void* pData, SIZE_T cbData, void* pResult)
{
    NTSTATUS nts;
    BCRYPT_ALG_HANDLE hAlg;
    DWORD cbHashObject;
    ULONG cbRet;
    BCRYPT_HASH_HANDLE hHash{};
    UCHAR* pHashObject{};
    if (!NT_SUCCESS(nts = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_MD5_ALGORITHM, NULL, 0)))
        return nts;
    if (!NT_SUCCESS(nts = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject,
        sizeof(DWORD), &cbRet, 0)))
        goto TidyUp;
    pHashObject = (UCHAR*)_malloca(cbHashObject);
    if (!NT_SUCCESS(nts = BCryptCreateHash(hAlg, &hHash, pHashObject, cbHashObject,
        NULL, 0, 0)))
        goto TidyUp;
    if (!NT_SUCCESS(nts = BCryptHashData(hHash, (BYTE*)pData, (ULONG)cbData, 0)))
        goto TidyUp;
    if (!NT_SUCCESS(nts = BCryptFinishHash(hHash, (UCHAR*)pResult, 16, 0)))
        goto TidyUp;
TidyUp:
    if (hHash)
        BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlg, 0);
    if (pHashObject)
        _freea(pHashObject);
    return nts;
}
std::string getMD5String(const unsigned char result[16]) {
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill  ('0') << (int)result[i];
    }
    return ss.str();
}
string MD5(string text) {
    // Ҫ����MD5���ַ���
    const char* data = text.c_str();
    // ���ڴ��MD5ɢ�н���Ļ�������MD5ɢ����16�ֽ�
    BYTE result[16];

    // ����MD5��������ɢ��
    NTSTATUS status = calc(data, (SIZE_T)strlen(data), result);

    // ����Ƿ�ɹ�
    if (NT_SUCCESS(status)) {
        // ��ӡMD5ɢ��ֵ
        /*string a = "";
        for (int i = 0; i < 16; ++i)
        {
            printf(result[i]);
            a += result[i];
        }*/
        return getMD5String(result);
        
    } else {
        // ������
        return "";
    }

    return "";
}