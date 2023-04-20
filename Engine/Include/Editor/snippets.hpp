#pragma once

std::string cp1251_to_utf8(std::string str) 
{
    int result_w = MultiByteToWideChar(1251, 0, str.data(), static_cast<int>(str.size()), NULL, 0);
    if (result_w == 0)
    {
        return "";
    }
    TCHAR t_filename[_MAX_EXT] = {};
    std::wstring wres(result_w, '\0');
    if (!MultiByteToWideChar(1251, 0, str.data(), static_cast<int>(str.size()), t_filename, result_w))
    {
        return "";
    }
    wres = t_filename;
    int result_c = WideCharToMultiByte(CP_UTF8, 0, wres.data(), static_cast<int>(wres.size()), NULL, 0, NULL, NULL);
    if (result_c == 0)
    {
        return "";
    }
    std::string res(result_c, '\0');
    char	fileName[_MAX_EXT] = {};
    if (!WideCharToMultiByte(CP_UTF8, 0, wres.data(), static_cast<int>(wres.size()), fileName, result_c, 0, 0))
    {
        return "";
    }
    res = fileName;
    return res;
}
