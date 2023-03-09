#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>

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

    bool booleanValue{};
    double numberValue{};
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

/* Generate the header file for a class */
void generate_header_file(JSONValue inJsonValue, ofstream &header_file) {
    header_file << "#ifndef " << inJsonValue.objectValue["Class"].stringValue << "_H\n";
    header_file << "#define " << inJsonValue.objectValue["Class"].stringValue << "_H\n";
    header_file << "#include <string>" << "\n";
    header_file << "using namespace std;\n";
    header_file << "class " << inJsonValue.objectValue["Class"].stringValue << " {\n";
    header_file << "private:\n";
    string upperString;
    string lowerString;
    unordered_map<string, JSONValue> jsonMap = inJsonValue.objectValue;
    int count = 0;
    for (std::pair<std::string, JSONValue> element: jsonMap) {
        if (element.first.find("Field") != string::npos) {
            string index = element.first.substr(5);
            string valueKey = "Value" + index;
            /* check value type */
            JSONValue value = jsonMap[valueKey];

            if (value.type == JSONValueType::String) {
                upperString += ("string " + element.second.stringValue + ";\n");
                lowerString += ("string " + element.second.stringValue);
            } else if (value.type == JSONValueType::Number) {
                if (ceil(value.numberValue) == floor(value.numberValue)) {
                    upperString += ("int " + element.second.stringValue + ";\n");
                    lowerString += ("int " + element.second.stringValue);
                } else {
                    upperString += ("float " + element.second.stringValue + ";\n");
                    lowerString += ("float " + element.second.stringValue);
                }
            }
            count++;
            if (count != (jsonMap.size() - 2) / 2) {
                lowerString += ", ";
            }
        } else if (element.first.find("Value") != string::npos) {
            /* ignore */
        } else {
            cout << "not supported: " + element.first << "\n";
        }
    }
    header_file << upperString;
    header_file << "public:\n";
    header_file << inJsonValue.objectValue["Class"].stringValue << "(";

    header_file << lowerString;
    header_file << ");\n";
    header_file << "};\n";
    header_file << "#endif\n";
}

// Generate the implementation file for a class
//void generate_implementation_file( JSONValue info, ofstream& implementation_file) {
//    implementation_file << "#include <iostream>\n";
//    implementation_file << "#include \"" << class_name << ".h\"\n";
//    implementation_file << "using namespace std;\n";
//    implementation_file << class_name << "::" << class_name << "(";
//    for (size_t i = 0; i < member_names.size(); i++) {
//        if (i > 0) {
//            implementation_file << ", ";
//        }
//        implementation_file << get_type("") << " " << member_names[i];
//    }
//    implementation_file << ")\n";
//    implementation_file << "{\n";
//    for (const auto& member_name : member_names) {
//        implementation_file << member_name << " = " << member_name << ";\n";
//    }
//    implementation_file << "}\n";
//}

int main() {
    ifstream inFile("student.json");
    string jsonString((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());

    try {
        JSONParser parser(jsonString);
        JSONValue parsedJsonValue = parser.parse();

        ofstream header_file("student.h");
        ofstream implementation_file("student.cpp");
        generate_header_file(parsedJsonValue, header_file);
    } catch (const exception &e) {
        cerr << "Failed to parse JSON string: " << e.what() << endl;
    }

    return 0;
}