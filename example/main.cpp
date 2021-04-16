#include "../inc/INIObject.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
using namespace INI;

int main() {
    try {
        INIObject ini;
        FILE *f = fopen("file.txt", "w");
        ofstream ofile("ofile.txt");
        ostringstream str_stream;
        ini.parse("main.ini");
        ini.print(cout);
        ini.print(ofile);
        ini.print(str_stream);
        ini.print(f);
        cout << str_stream.str();
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}
