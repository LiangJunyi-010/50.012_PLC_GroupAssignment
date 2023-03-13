#ifndef PART3_H
#define PART3_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <regex>

std::vector<std::string> split(std::string s, const std::string& delimiter);
std::string removeWhitespace(const std::string &str);
std::vector<std::string> getClassNames(std::string className);
void editHeader(std::string fileName,std::vector<std::string> classNames,
                const std::vector<std::string> &classStrings,
                const std::vector<std::string> &visStrings,
                const std::vector<std::string> &returnTypeStrings,
                const std::vector<std::string> &mtdNameStrings,
                const std::vector<std::string> &mtdImplStrings);
void editCpp(std::string fileName,std::vector<std::string> classNames,
                const std::vector<std::string> &classStrings,
                const std::vector<std::string> &visStrings,
                const std::vector<std::string> &returnTypeStrings,
                const std::vector<std::string> &mtdNameStrings,
                const std::vector<std::string> &mtdImplStrings);

#endif // PART3_H