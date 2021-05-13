#include "../inc/INIObject.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace INI;

int main() {
    try {
        INIObject ini;
        ini.parse("INI/twisted.ini");
        for (std::size_t i = 0; i < ini.size(); i++) {
            cout << ini.getKey(i) << " = ";
            if (ini.getValue(i) == nullptr) {
                cout << "empty" << endl;
            } else {
                cout << ini.getValue(i) << endl;
            }
        }
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}