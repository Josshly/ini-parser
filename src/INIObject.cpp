#include "../inc/INIObject.hpp"

namespace INI {

char *myStrdup(const char *src) {
    std::size_t len = strlen(src);
    char *s = new char[len + 1];
    memmove(s, src, len + 1);
    return s;
}

void strStrip(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void toLower(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void INIObject::add(const char *key, const char *val) {
    if (key == nullptr) {
        throw std::logic_error("INIObject::add() invalid argument: key cannot be nullptr.");
    }
    for (std::size_t i = 0; i < this->key.size(); i++) {
        if (!strcmp(this->key[i], key)) {
            throw std::runtime_error("The keys or sections cannot be repeated.");
        }
    }
    this->key.emplace_back(myStrdup(key));
    this->val.emplace_back(val ? myStrdup(val) : nullptr);
}

int INIObject::readLine(const std::string &line, const char *f, std::string &key,
                        std::string &val) {
    char *key_cpy = new char[line.size() + 1];
    char *val_cpy = new char[line.size() + 1];

    int res = sscanf(line.c_str(), f, key_cpy, val_cpy);
    key = key_cpy;
    val = val_cpy;

    delete[] key_cpy;
    delete[] val_cpy;
    return res;
}

int INIObject::readLine(const std::string &line, const char *f, std::string &key) {
    char *key_cpy = new char[line.size() + 1];

    int res = sscanf(line.c_str(), f, key_cpy);
    key = key_cpy;

    delete[] key_cpy;
    return res;
}

INIObject::line_status INIObject::parseLine(std::string &input_line, std::string &section,
                                            std::string &key, std::string &val) {
    line_status status;
    strStrip(input_line);
    std::size_t len = input_line.size();

    if (len < 1) {
        // Empty line.
        status = LINE_EMPTY;
    } else if (input_line[0] == '#' || input_line[0] == ';') {
        // Comment line.
        status = LINE_COMMENT;
    } else if (input_line[0] == '[' && input_line[len - 1] == ']') {
        // Section line.
        readLine(input_line, "[%[^]]", section);
        toLower(section);
        status = LINE_SECTION;
    } else if (readLine(input_line, "%[^=] = \"%[^\"]\"", key, val) == 2 ||
               readLine(input_line, "%[^=] = '%[^\']'", key, val) == 2) {
        // key=val line with quotes.
        strStrip(key);
        toLower(key);
        status = LINE_VALUE;
    } else if (readLine(input_line, "%[^=] = %[^;#]", key, val) == 2) {
        // key=val line without quotes.
        strStrip(key);
        toLower(key);
        strStrip(val);
        // store '' or "" as empty values.
        if (!strcmp(val.c_str(), "\"\"") || !strcmp(val.c_str(), "''")) {
            val.clear();
        }
        status = LINE_VALUE;
    } else if (readLine(input_line, "%[^=] = %[;#]", key, val) == 2 ||
               readLine(input_line, "%[^=] %[=]", key, val) == 2) {
        /**
         * Special cases:
         * key=
         * key=#
         * key=;
         */
        strStrip(key);
        toLower(key);
        val.clear();
        status = LINE_VALUE;
    } else {
        status = LINE_ERROR;
    }
    return status;
}

void INIObject::parse(const char *filename) {
    std::ifstream input(filename);
    std::string line;
    std::string section;
    std::string key;
    std::string value;
    std::string tmp;
    std::ptrdiff_t len;
    bool is_multi_line = false;

    if (!input.is_open()) {
        throw std::runtime_error("File cannot be opened.");
    }

    while (std::getline(input, line)) {
        len = line.size() - 1;

        // Remove spaces an the end of the line.
        while (isspace(line[len])) {
            line.pop_back();
            len--;
        }

        // The line entirely consist of spaces.
        if (len < 0) {
            len = 0;
        }

        // Detect multi-line.
        if (line[len] == '\\') {
            line[len] = ' ';
            tmp += line;
            is_multi_line = true;
            continue;
        }

        if (is_multi_line) {
            tmp += line;
            line = tmp;
            tmp.clear();
            is_multi_line = false;
        }

        switch (parseLine(line, section, key, value)) {
        case LINE_EMPTY:
        case LINE_COMMENT:
            break;
        case LINE_SECTION:
            add(section.c_str(), nullptr);
            break;
        case LINE_VALUE:
            key = section + ":" + key;
            add(key.c_str(), value.c_str());
            break;
        case LINE_ERROR:
            input.close();
            throw std::runtime_error("Syntax error.");
        default:
            break;
        }
    }
    input.close();
}

void INIObject::print(FILE *f) {
    for (std::size_t i = 0; i < key.size(); i++) {
        if (val[i] == nullptr) {
            fprintf(f, "[%s]:\n", key[i]);
        } else {
            fprintf(f, "    [%s] = %s\n", key[i], val[i]);
        }
    }
}

} // namespace INI