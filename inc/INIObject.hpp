#ifndef INC_INIOBJECT_HPP
#define INC_INIOBJECT_HPP
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace INI {
class INIObject {

public:
    /**
     * Parses an INI file.
     *
     * @param filename File to parse.
     * @return void.
     */
    void parse(const char *filename);

    /**
     * Prints data to stream.
     *
     * This method is for streams such as: fstream, ofstream, ostream, ostringstream, stringstream
     *
     * @param stream Stream where to print information.
     * @return void.
     */
    template <typename stream,
              typename = std::enable_if_t<std::is_base_of<std::ostream, stream>::value ||
                                          std::is_same<std::ostream, stream>::value>>
    void print(stream &output_stream) const noexcept {
        for (std::size_t i = 0; i < key.size(); i++) {
            if (val[i] == nullptr) {
                output_stream << "[" << key[i] << "]:\n";
            } else {
                output_stream << "    [" << key[i] << "] = " << val[i] << "\n";
            }
        }
    }

    /**
     * Prints data to c-style file.
     *
     * @param f File where to print information.
     * @return void.
     */
    void print(FILE *f) const noexcept;

    /**
     * Returns the number of keys.
     *
     * @return number of keys.
     */
    std::size_t size() const noexcept;

    /**
     * Returns the specified key.
     *
     * @param i position of key.
     * @return specified key.
     */
    const char *getKey(std::size_t i);

    /**
     * Returns the specified value.
     * The value can be a nullptr.
     *
     * @param i position of value.
     * @return specified value.
     */
    const char *getValue(std::size_t i);

private:
    enum line_status {
        LINE_COMMENT,
        LINE_EMPTY,
        LINE_ERROR,
        LINE_SECTION,
        LINE_VALUE,
    };

    /**
     * Adds elements.
     *
     * @param key Key to add.
     * @param val Value to add.
     * @return void.
     */
    void add(const char *key, const char *val);

    /**
     * sscanf wrapper that supports std::string.
     *
     * @param line Same as buf parameter in sscanf.
     * @param f Same as format parameter in sscanf.
     * @param key Key storage space.
     * @param val Value storage space.
     * @return Returns the number of items successfully filled.
     */
    int readLine(const std::string &line, const char *f, std::string &key, std::string &val);

    /**
     * Same function as above, but only to fill the key.
     */
    int readLine(const std::string &line, const char *f, std::string &key);

    /**
     * Parses a single line in INI file.
     *
     * @param input_line Line to parse
     * @param section Section storage space.
     * @param key Key storage space.
     * @param val Value storage space.
     * @return line_staus.
     */
    line_status parseLine(std::string &input_line, std::string &section, std::string &key,
                          std::string &val);

private:
    std::vector<const char *> key;
    std::vector<const char *> val;
};

/**
 * Removes spaces in the string.
 *
 * @param str String to convert.
 * @return void.
 */
void strStrip(std::string &str);

/**
 * Converts the given string to lowercase.
 *
 * @param str String to convert.
 * @return void.
 */
void toLower(std::string &str);

/**
 * Creates a copy of c-style string.
 *
 * @param src String to copy.
 * @return Pointer to a copy of the string.
 */
char *myStrdup(const char *src) noexcept;

} // namespace INI
#endif