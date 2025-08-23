#include <iostream>
#include <utility>
#include <set>
#include <vector>
#include <string.h>
#include <sstream> 
#include <fstream>



using namespace std;

class IndiceInvertido {

public:

    string palavra;
    set<long long> ids;

    IndiceInvertido();
    IndiceInvertido(string palavra, set<long long> ids);

    vector<string> parserLinha(const string& linha);

    void unpackIndiceInvertido(const string& buffer);

    string packIndiceInvertido() const;


};
