#include "../inc/INIObject.hpp"
#include <exception>
#include <iostream>
using namespace std;
using namespace INI;

int main() {
    try {
        INIObject ini;
        ini.parse("INI/twisted.ini");
        ini.print(cout);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}