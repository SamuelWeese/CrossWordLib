#include <iostream>
#include "crossword.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    vector<string> feedMe = {"Chimpanze", "Munchies", "sIP"};
    crossword aCrossword(feedMe);
    aCrossword.print();
    return 0;
}
