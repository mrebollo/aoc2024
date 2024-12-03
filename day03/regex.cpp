#include <iostream>
#include <regex>
using namespace std;

int main(){
    string s = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    regex r("mul\\([0-9]+,[0-9]+\\)");
    smatch m; 
    while(regex_search(s, m, r)){
        for (auto x : m)
            cout << x << " ";
        cout << endl;
        //remove the matched part
        s = m.suffix().str();
    }
    return 0;
}