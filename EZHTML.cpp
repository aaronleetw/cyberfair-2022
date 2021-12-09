/*
    EZHTML : An easy static website generator, written entirely in C++
    Copyright (C) 2021 Aaron Lee

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* 
    This program has been edited from the original distrubution to accommodate
    for usage requirements
*/

#include <fstream>
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif
#include <vector>
#include <iostream>
using namespace std;
string CSS[100], JS[100], LOCATION = "docs";
vector<pair<string, string>> enPAGE, zhPAGE, PAGE;
int CSScount = 0, JScount = 0, enPAGEcount, zhPAGEcount, PAGEcount;

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

std::string reduce(const std::string& str,
                   const std::string& fill = " ",
                   const std::string& whitespace = " \t")
{
    auto result = trim(str, whitespace);
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;
        result.replace(beginSpace, range, fill);
        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }
    return result;
}

void generateFinalFile(string fileName, string titleName, bool lang)
{
    cout << "Opening: ./" + LOCATION + "/" + fileName + ".html\n";
    string fWithoutLang = (fileName[2] == '/') ? fileName.substr(3) : fileName;
    if (fWithoutLang.find('/') != string::npos)
    {
        string dir = fWithoutLang.substr(0, fWithoutLang.find('/'));
        if (!fs::exists(LOCATION + "/" + (lang ? "zh" : "en") + "/" + dir))
        {
            fs::create_directory(LOCATION + "/" + (lang ? "zh" : "en") + "/" + dir);
        }
    }
    std::ofstream EachFile("./" + LOCATION + "/" + fileName + ".html");
    EachFile << "<!-- Website generated using a modified version of EZHTML -->";
    EachFile << "<!-- View the original project on Github: https://github.com/aaronleetw/EZHTML -->";
    EachFile << "<!DOCTYPE html>";
    EachFile << "<html lang=\"" << (lang ? "zh" : "en") << "\">";
    EachFile << "<head><title>" << titleName << "</title>";
    for (int i = 0; i < CSScount; i++)
        EachFile << "<link rel=\"stylesheet\" href=\"" << CSS[i] << "\">";
    EachFile << "<meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta content=\"utf-8\" http-equiv=\"encoding\">";
    EachFile << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    string tmp = fWithoutLang;
    if (tmp.find('/') != string::npos)
        tmp = tmp.substr(0, tmp.find('/')) + tmp.substr(tmp.find('/') + 1);
    EachFile << "</head><body class=\"" + tmp + "\">";
    ifstream NAVread;
    if (lang)
        NAVread.open("NAVzh.html");
    else
        NAVread.open("NAVen.html");
    string gotText;
    while (getline(NAVread, gotText)) {
        // remove whitespaces
        EachFile << reduce(gotText);
    }
    NAVread.close();

    ifstream PAGEread(fileName + ".html");
    while (getline(PAGEread, gotText)) {
        EachFile << reduce(gotText);
    }
    PAGEread.close();

    ifstream FOOTERread("FOOTER.html");
    while (getline(FOOTERread, gotText)) {
        EachFile << reduce(gotText);
    }
    FOOTERread.close();

    for (int i = 0; i < JScount; i++)
        EachFile << "<script src=\"" << JS[i] << "\"></script>";

    EachFile << "</body></html>";
    EachFile.close();
}

int getAllTheNumbers(string str)
{
    int N = 0;
    for (int i = 0; i < str.size(); i++)
        N = (N * 10) + ((int)(str[i]) - '0');
    return N;
}

void resetDir()
{
    fs::remove_all("./" + LOCATION + "/");
    fs::create_directories("./" + LOCATION + "/");
    fs::create_directories("./" + LOCATION + "/en/");
    fs::create_directories("./" + LOCATION + "/zh/");
}

int main()
{
    auto start = std::chrono::system_clock::now();
    int readSmthing = 0, tempCSScnt, tempJScnt;
    string myText, tempTitleName;
    ifstream INFOread("INFO");
    while (getline(INFOread, myText))
    {
        if (readSmthing != 0)
        {
            if (readSmthing == 1) // READ CSS
            {
                CSS[tempCSScnt] = myText;
                tempCSScnt++;
                if (tempCSScnt == CSScount)
                    readSmthing = 0;
            }
            else if (readSmthing == 2) // READ JS
            {
                JS[tempJScnt] = myText;
                tempJScnt++;
                if (tempJScnt == JScount)
                    readSmthing = 0;
            }
            else if (readSmthing == 3) // READ EN PAGE TITLE
            {
                enPAGEcount++;
                myText.insert(0, "en/");
                enPAGE.push_back(make_pair(myText, tempTitleName));
                generateFinalFile(myText, tempTitleName, 0);
                readSmthing = 0;
            }
            else if (readSmthing == 4) // READ ZH PAGE TITLE
            {
                zhPAGEcount++;
                myText.insert(0, "zh/");
                zhPAGE.push_back(make_pair(myText, tempTitleName));
                generateFinalFile(myText, tempTitleName, 1);
                readSmthing = 0;
            }
            else if (readSmthing == 5) // READ REG PAGE TITLE
            {
                PAGEcount++;
                PAGE.push_back(make_pair(myText, tempTitleName));
                generateFinalFile(myText, tempTitleName, 0);
                readSmthing = 0;
            }
            else if (readSmthing == 6) // READ LOCATION
            {
                LOCATION = myText;
                resetDir();
                readSmthing = 0;
            }
        }
        else if (myText[0] == '^')
        {
            if (myText.substr(1, 2) == "JS")
            {
                JScount = getAllTheNumbers(myText.substr(3));
                tempJScnt = 0;
                readSmthing = 2;
            }
            else if (myText.substr(1, 3) == "CSS")
            {
                CSScount = getAllTheNumbers(myText.substr(4));
                tempCSScnt = 0;
                readSmthing = 1;
            }
            else if (myText.substr(1, 8) == "LOCATION")
                readSmthing = 6;
        }
        else if (myText.substr(0, 3) == "en#")
        {
            readSmthing = 3;
            tempTitleName = myText.substr(3);
        }
        else if (myText.substr(0, 3) == "zh#")
        {
            readSmthing = 4;
            tempTitleName = myText.substr(3);
        }
        else if (myText[0] == '#')
        {
            readSmthing = 5;
            tempTitleName = myText.substr(1);
        }
    }
    INFOread.close();
    fs::copy("./static", "./" + LOCATION + "/static", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::ofstream ResultsFile("./" + LOCATION + "/results");
    ResultsFile << "./" + LOCATION + "/ was generated successfully at " << ctime(&end_time);
    ResultsFile << "elapsed time: " << elapsed_seconds.count() << "s" << endl
                << endl;
    ResultsFile << "JScount: " << JScount << "; CSScount: " << CSScount << endl;
    for (int i = 0; i < JScount; i++)
        ResultsFile << "; JS[" << i << "]: " << JS[i] << endl;
    for (int i = 0; i < CSScount; i++)
        ResultsFile << "; CSS[" << i << "]: " << CSS[i] << endl;
    ResultsFile << "\nEN Pages: \n";
    for (int i = 0; i < enPAGEcount; i++)
        ResultsFile << "; enPAGE[" << i << "]: " << enPAGE[i].first << "; TITLE: " << enPAGE[i].second << endl;
    ResultsFile << "\nZH Pages: \n";
    for (int i = 0; i < zhPAGEcount; i++)
        ResultsFile << "; zhPAGE[" << i << "]: " << zhPAGE[i].first << "; TITLE: " << zhPAGE[i].second << endl;
    ResultsFile << "\nREG Pages: \n";
    for (int i = 0; i < PAGEcount; i++)
        ResultsFile << "; PAGE[" << i << "]: " << PAGE[i].first << "; TITLE: " << PAGE[i].second << endl;
    ResultsFile.close();
    return 0;
}