#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <regex>
#include "part3.h"

using namespace std;


vector<string> split(string s, const string& delimiter) {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);
    return tokens;
}

string removeWhitespace(const string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}


vector<string> getClassNames(string className) {
    vector<string> classNames;
    string line;
    ifstream file(className+".h");
    if (file.is_open()) {
        while (getline(file, line)) {
            smatch match;
            regex classRegex("class\\s+(\\w+)\\s*\\{");
            if (regex_search(line, match, classRegex)) {
                classNames.push_back(match[1].str());
            }
        }
        file.close();
    }
    return classNames;
}

void editHeader(string fileName,vector<string> classNames,
                const vector<string> &classStrings,
                const vector<string> &visStrings,
                const vector<string> &returnTypeStrings,
                const vector<string> &mtdNameStrings,
                const vector<string> &mtdImplStrings) {
    
    for (int j=0; j<classNames.size(); j++){
        ifstream inHeaderFile(fileName + ".h");
        string headerString((istreambuf_iterator<char>(inHeaderFile)), istreambuf_iterator<char>());
        string newHeaderString;
        string className = classNames[j];

        /* adding methods to .h */
        string privateMtdStr;
        string publicMtdStr;
        string protectedMtdStr;

        for (int i = 0; i<classStrings.size();i++){
            if(classStrings[i] == className){
                string currVis = visStrings[i];
                string currMtdStr = (returnTypeStrings[i] + " " + mtdNameStrings[i] + ";\n");
                if (currVis == "private") {
                /* add to privateMtdStr, later write to .h file */
                privateMtdStr += currMtdStr;
                } else if (currVis == "public") {
                    publicMtdStr += currMtdStr;
                } else {
                    protectedMtdStr += currMtdStr;
                }
            }
        }

        /* edit .h */
        /* 1. find class */
        unsigned long splitClassNameIdx = headerString.find(className);
        string splitClassNamepart1 = headerString.substr(0,splitClassNameIdx);
        string splitClassNamepart2 = headerString.substr(splitClassNameIdx);
        newHeaderString = splitClassNamepart1;

        /* 2. add private method*/
        unsigned long splitPrivateIdx = splitClassNamepart2 .find("private:");
        string splitPrivatepart1 = splitClassNamepart2.substr(0,splitPrivateIdx+8);
        string splitPrivatepart2 = splitClassNamepart2.substr(splitPrivateIdx+8);
        newHeaderString = newHeaderString+splitPrivatepart1+"\n"+privateMtdStr;

        /* 3. add public method*/
        unsigned long splitPublicIdx = splitPrivatepart2.find("public:");
        string splitPublicpart1 = splitPrivatepart2.substr(0,splitPublicIdx+7);
        string splitPublicpart2 = splitPrivatepart2.substr(splitPublicIdx+7);
        newHeaderString = newHeaderString+splitPublicpart1+"\n"+publicMtdStr;

        /* 4. add protect method*/
        unsigned long splitProtectIdx = splitPublicpart2.find("protected:");
        string splitProtectpart1 = splitPublicpart2.substr(0,splitProtectIdx+10);
        string splitProtectpart2 = splitPublicpart2.substr(splitProtectIdx+10);
        newHeaderString = newHeaderString+splitProtectpart1+"\n"+protectedMtdStr+splitProtectpart2;

        /* 5. clear origin content*/
        ofstream file(fileName + ".h", ios::trunc);

        /* 6. add new string*/
        ofstream header_file(fileName + ".h");
        header_file<< newHeaderString;
        
    }
    

}

void editCpp(string fileName,vector<string> classNames,
                const vector<string> &classStrings,
                const vector<string> &visStrings,
                const vector<string> &returnTypeStrings,
                const vector<string> &mtdNameStrings,
                const vector<string> &mtdImplStrings){

    for (int i=0; i<classNames.size(); i++){
        ifstream inCppFile(fileName + ".cpp");
        string cppString((istreambuf_iterator<char>(inCppFile)), istreambuf_iterator<char>());
        string newcppString;
        string className = classNames[i];

        /* adding methods to .cpp */
        string cppMtdStr;

        
        for (int j = 0; j<classStrings.size();j++){
            if(classStrings[j] == className){
                 /* for cpp */
                cppMtdStr += (returnTypeStrings[j] + " " + className + "::" + mtdNameStrings[j] + mtdImplStrings[j] + "\n");
            }
        }

        /* edit .cpp */
        /* 1. int main */
        unsigned long splitClassNameIdx = cppString.find("int main");
        string splitCpppart1 = cppString.substr(0,splitClassNameIdx);
        string splitCpppart2 = cppString.substr(splitClassNameIdx);
        newcppString = splitCpppart1+"\n"+cppMtdStr+"\n"+splitCpppart2;

        /* 2. clear origin content*/
        ofstream file(fileName + ".cpp", ios::trunc);

        /* 3. add new string*/
        ofstream cpp_file(fileName + ".cpp");
        cpp_file<< newcppString;
    }

}

int main() {
    string fileName;
    cout << "Input file name (without .json): ";
    cin >> fileName;
    ifstream inTxtFile(fileName + ".txt");
    string txtString((istreambuf_iterator<char>(inTxtFile)), istreambuf_iterator<char>());

    vector<string> splitStrings = split(txtString, "######\n");
    splitStrings.erase(splitStrings.begin());
    vector<string> classStrings;
    vector<string> visStrings;
    vector<string> returnTypeStrings;
    vector<string> mtdNameStrings;
    vector<string> mtdImplStrings;
    for (int i = 0; i < splitStrings.size(); i += 2) {
        string classVisString = splitStrings[i];
        vector<string> classVisStrings = split(classVisString, " -");
        string className = removeWhitespace(classVisStrings[0]);
        string visibility = removeWhitespace(classVisStrings[1]);
        classStrings.push_back(className);
        visStrings.push_back(visibility);
    }
    for (int i = 1; i < splitStrings.size(); i += 2) {
        string retTypeMtdNameImplString = splitStrings[i];
        unsigned long splitIdx = retTypeMtdNameImplString.find('{');
        string retTypeMtdNameString = removeWhitespace(retTypeMtdNameImplString.substr(0, splitIdx));
        string mtdImplString = removeWhitespace(retTypeMtdNameImplString.substr(splitIdx));
        /* separate return type and method name */
        unsigned long splitIdx2 = retTypeMtdNameString.find(' ');
        string returnType = retTypeMtdNameString.substr(0, splitIdx2);
        string methodName = removeWhitespace(retTypeMtdNameString.substr(splitIdx2));
        returnTypeStrings.push_back(returnType);
        mtdNameStrings.push_back(methodName);
        mtdImplStrings.push_back(mtdImplString);
    }
    
    vector<string> classNames = getClassNames(fileName);
    editHeader(fileName,classNames,
                classStrings, visStrings, returnTypeStrings, mtdNameStrings, mtdImplStrings);
    editCpp(fileName,classNames,
                classStrings, visStrings, returnTypeStrings, mtdNameStrings, mtdImplStrings);
    return 0;
}