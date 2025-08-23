#include <iostream>
#include <string.h>

#include "Indice.h"
#include "IndiceInvertido.h"


using namespace std;


IndiceInvertido::IndiceInvertido() {} 
IndiceInvertido::IndiceInvertido(string palavra, set<long long> ids) : palavra(palavra), ids(ids) {} 

vector<string> IndiceInvertido::parserLinha(const string& linha) {

    vector<string> campos;
    size_t inicio = 0;
    size_t fim = linha.find("||");

    while (fim != string::npos) {
    
        campos.push_back(linha.substr(inicio, fim - inicio));
        inicio = fim + 2;  
        fim = linha.find("||", inicio);
    }

    campos.push_back(linha.substr(inicio));

    return campos;
}

void IndiceInvertido::unpackIndiceInvertido(const string& buffer) {

    vector<string> campos = parserLinha(buffer);

    string palavra = campos[0];
    set<long long> ids;

    for(size_t i = 1; i < campos.size(); i++) {
        ids.insert(stoll(campos[i]));
    }
            
    this->palavra = palavra;
    this->ids = ids;
}

string IndiceInvertido::packIndiceInvertido() const{

    ostringstream oss;

    for (auto it = this->ids.begin(); it != this->ids.end(); ++it) {
        if (it != this->ids.begin()) {
            oss << "||";
        }
        oss << *it; 
    }

    string registro = this->palavra + "||" + oss.str();
    
    int size = registro.size();
    
    char* buffer = new char[size + sizeof(int)];
    
    memcpy(buffer, &size, sizeof(int));
    
    memcpy(buffer + sizeof(int), registro.c_str(), size);
    
    string packed(buffer, size + sizeof(int));
    
    delete[] buffer;
    
    return packed;
}