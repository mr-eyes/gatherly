#include <gatherly.hpp>
#include <iostream>

using namespace std;

int main(){

    auto * Gath = new Gatherly::SplittedIndex("/home/mabuelanin/dib-dev/gatherly/splitted_index/ok");
    cout << Gath->gatherly_test() << endl;    
}