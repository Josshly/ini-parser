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
        FILE *f = fopen("results/file.txt", "w");
        ofstream ofile("results/ofile.txt");
        ostringstream str_stream;
        ini.parse("INI/main.ini");
        ini.print(cout);
        ini.print(ofile);
        ini.print(str_stream);
        ini.print(f);
        cout << str_stream.str();
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}
