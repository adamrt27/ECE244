#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main(){
    string a = "hi 1.2 ";
    stringstream test(a);
    string t;
    int t1, t2;
    test >> t;
    test >> t1;
    test >> t2;
    cout << t << " " << t1 << " " << t2 << endl;
    cout << test.fail() << endl;
    cout << test.eof() << endl;
    // cout << (1.0 == 1) << endl;
    // cout << (1.2 == 1) << endl;
}