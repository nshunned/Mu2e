#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(){
    
    cout << "Hello world" << endl;
    double x = 2.0/3;
    cout << setprecision(2) << x << endl;
    
    vector<char> array;
    char c = '0';
    while(c != 'x'){
        cin >> c;
        array.push_back(c);
    }
    
    for(int i=0; i < array.size(); i++)
        cout << array[i] << " ";
    cout << endl;
    
    return 0;
}
