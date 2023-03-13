

#ifndef PART2_H
#define PART2_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
enum class JSONValueType {
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object
};

class JSONValue {
public:
    JSONValueType type;

    JSONValue() : type(JSONValueType::Null) {}

    JSONValue(bool value) : type(JSONValueType::Boolean), booleanValue(value) {}

    JSONValue(double value) : type(JSONValueType::Number), numberValue(value) {}

    JSONValue(std::string value) : type(JSONValueType::String), stringValue(std::move(value)) {}

    JSONValue(const std::vector <JSONValue> &value) : type(JSONValueType::Array), arrayValue(value) {}

    JSONValue(const std::unordered_map <std::string, JSONValue> &value) : type(JSONValueType::Object),
                                                                          objectValue(value) {}

    bool booleanValue{};
    double numberValue{};
    std::string stringValue;
    std::vector <JSONValue> arrayValue;
    std::unordered_map <std::string, JSONValue> objectValue;
};

class JSONParser {
public:
    explicit JSONParser(std::string jsonString) : jsonString(std::move(jsonString)), index(0) {}

    JSONValue parse();

private:
    std::string jsonString;
    size_t index;

    void skipWhiteSpace();

    char getCurrentChar();

    void expect(const std::string &expected);

    std::string parseString();

    double parseNumber();

    std::vector <JSONValue> parseArray();

    std::unordered_map <std::string, JSONValue> parseObject();
};

string generate_impl_file(JSONValue inJsonValue, ofstream &header_file, ofstream &cpp_file);

void generate_file(const JSONValue &inJsonValue, ofstream &header_file, ofstream &cpp_file, const string &fileName);

#endif


