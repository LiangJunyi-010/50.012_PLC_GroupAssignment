#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

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

    JSONValue(const string &value) : type(JSONValueType::String), stringValue(value) {}

    JSONValue(const vector<JSONValue> &value) : type(JSONValueType::Array), arrayValue(value) {}

    JSONValue(const unordered_map<string, JSONValue> &value) : type(JSONValueType::Object), objectValue(value) {}

    bool booleanValue;
    double numberValue;
    string stringValue;
    vector<JSONValue> arrayValue;
    unordered_map<string, JSONValue> objectValue;
};

class JSONParser {
public:
    JSONParser(const string &jsonString) : jsonString(jsonString), index(0) {}

    JSONValue parse() {
        skipWhiteSpace();

        char firstChar = getCurrentChar();

        if (firstChar == 'n') {
            expect("null");
            return JSONValue();
        } else if (firstChar == 't') {
            expect("true");
            return JSONValue(true);
        } else if (firstChar == 'f') {
            expect("false");
            return JSONValue(false);
        } else if (firstChar == '\"') {
            return parseString();
        } else if (isdigit(firstChar) || firstChar == '-') {
            return parseNumber();
        } else if (firstChar == '[') {
            return parseArray();
        } else if (firstChar == '{') {
            return parseObject();
        }

        throw invalid_argument("Invalid JSON string");
    }

private:
    string jsonString;
    size_t index;

    void skipWhiteSpace() {
        while (index < jsonString.size() && isspace(jsonString[index])) {
            ++index;
        }
    }

    char getCurrentChar() {
        return jsonString[index];
    }

    void expect(const string &expected) {
        size_t length = expected.length();

        if (jsonString.substr(index, length) == expected) {
            index += length;
            skipWhiteSpace();
        } else {
            throw invalid_argument("Invalid JSON string");
        }
    }

    string parseString() {
        expect("\"");

        string result;
        char currentChar = getCurrentChar();

        while (currentChar != '\"' && index < jsonString.size()) {
            if (currentChar == '\\') {
                ++index;
                currentChar = getCurrentChar();

                switch (currentChar) {
                    case '\"':
                        result += '\"';
                        break;
                    case '\\':
                        result += '\\';
                        break;
                    case '/':
                        result += '/';
                        break;
                    case 'b':
                        result += '\b';
                        break;
                    case 'f':
                        result += '\f';
                        break;
                    case 'n':
                        result += '\n';
                        break;
                    case 'r':
                        result += '\r';
                        break;
                    case 't':
                        result += '\t';
                        break;
                    case 'u': {
                        string hexString = jsonString.substr(index + 1, 4);
                        int codePoint = stoi(hexString, nullptr, 16);

                        if (codePoint < 0xD800 || codePoint > 0xDFFF) {
                            result += static_cast<char>(codePoint);
                            index += 5;
                        } else {
                            throw invalid_argument("Invalid JSON string");
                        }
                        break;
                    }
                    default:
                        throw invalid_argument("Invalid JSON string");
                        break;
                }
            } else {
                result += currentChar;
            }

            ++index;
            currentChar = getCurrentChar();
        }

        if (currentChar == '\"') {
            ++index;
            skipWhiteSpace();
            return result;
        } else {
            throw invalid_argument("Invalid JSON string");
        }
    }

    double parseNumber() {
        size_t startIndex = index;

        if (getCurrentChar() == '-') {
            ++index;
        }

        while (index < jsonString.size() && isdigit(getCurrentChar())) {
            ++index;
        }

        if (getCurrentChar() == '.') {
            ++index;

            while (index < jsonString.size() && isdigit(getCurrentChar())) {
                ++index;
            }
        }

        if (getCurrentChar() == 'e' || getCurrentChar() == 'E') {
            ++index;

            if (getCurrentChar() == '-' || getCurrentChar() == '+') {
                ++index;
            }

            while (index < jsonString.size() && isdigit(getCurrentChar())) {
                ++index;
            }
        }

        string numberString = jsonString.substr(startIndex, index - startIndex);
        skipWhiteSpace();
        return stod(numberString);
    }

    vector<JSONValue> parseArray() {
        vector<JSONValue> result;

        expect("[");

        while (getCurrentChar() != ']') {
            result.push_back(parse());

            if (getCurrentChar() == ',') {
                ++index;
                skipWhiteSpace();
            } else if (getCurrentChar() != ']') {
                throw invalid_argument("Invalid JSON string");
            }
        }

        ++index;
        skipWhiteSpace();
        return result;
    }

    unordered_map<string, JSONValue> parseObject() {
        unordered_map<string, JSONValue> result;

        expect("{");

        while (getCurrentChar() != '}') {
            string key = parseString();
            expect(":");
            JSONValue value = parse();

            result.emplace(move(key), move(value));

            if (getCurrentChar() == ',') {
                ++index;
                skipWhiteSpace();
            } else if (getCurrentChar() != '}') {
                throw invalid_argument("Invalid JSON string");
            }
        }

        ++index;
        skipWhiteSpace();
        return result;
    }
};

int main() {
    ifstream inFile("student.json");
    string jsonString((istreambuf_iterator<char>(inFile)),
                      istreambuf_iterator<char>());

//    string jsonString = "{"name":"John","age":30,"city":"New York","married":false,"hobbies":["reading","coding","running"],"address":{"street":"123 Main St","city":"New York","zip":"10001"}}";
    try {
        JSONParser parser(jsonString);
        JSONValue value = parser.parse();
        if (value.type == JSONValueType::Object)     {
            cout << "here" << "\n";
        }
        if (value.objectValue["Value2"].type == JSONValueType::Number) {
            cout << "is number" << "\n";
        }
        cout << int(value.objectValue["Value2"].numberValue) << endl;
//        for (auto const& i : value.objectValue) {
//            cout << "key: " + i.first << " ";
//            cout << "stringValue: " + i.second.stringValue << " or ";
//            cout << "numberValue: " << i.second.numberValue << " or ";
//            cout << "intValue: " << int(i.second.numberValue) << "\n";
//        }
    } catch (const exception &e) {
        cerr << "Failed to parse JSON string: " << e.what() << endl;
    }

    return 0;
}