
#include <iostream>
#include <string>
#include <cstring> 
#include <algorithm> 
#include <stdexcept>
#include <sstream>
#include <vector>
#include <iomanip>



#include "Registro.h"


using namespace std;


Registro::Registro() : id(-1), titulo(""), autores(""), ano(0), categoria("") {}


Registro::Registro(long long id, string titulo, string autores, int ano, string categoria) 
: id(id), titulo(titulo), autores(autores), ano(ano), categoria(categoria) {}


vector<string> Registro::parserLinha(const string& linha) {

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


string Registro::packDelimitedWithDescriptor() const{
    
    string registro = (
        to_string(id)
        + "||" 
        + titulo 
        + "||" 
        + autores 
        + "||" 
        + to_string(ano) 
        + "||" 
        + categoria
    );
    
    int size = registro.size();
    
    char* buffer = new char[size + sizeof(int)];
    
    memcpy(buffer, &size, sizeof(int));
    
    memcpy(buffer + sizeof(int), registro.c_str(), size);
    
    string packed(buffer, size + sizeof(int));
    
    delete[] buffer;
    
    return packed;
}


void Registro::unpackDelimitedWithDescriptor(const string& buffer) {
    
    vector<string> campos = parserLinha(buffer);

    long long idInt = stoll(campos[0]);
    int anoInt = stoi(campos[3]);

    this->id = idInt;
    this->titulo = campos[1];
    this->autores = campos[2];
    this->ano = anoInt;
    this->categoria = campos[4];

    if ((this->id == -1) || this->titulo.empty() || this->autores.empty() || this->ano == 0 ) {
        throw runtime_error("Campos ausentes no registro.");
    }
}


void Registro::exibirRegistro() {

    int maxLength = to_string(this->id).length();

    for(const string& s : {this->titulo, this->autores, to_string(this->ano), this->categoria}) {
        maxLength = max(maxLength, static_cast<int>(s.length()));
    }

    string separador(maxLength + 13, '-'); 
    string separadorTitulo(maxLength + 13, '=');

    string textoTitulo = "Registro";

    cout << endl;
    cout << separadorTitulo << endl;
    
    int espacosEsquerda = ((separadorTitulo.length() - textoTitulo.length()) / 2) - 1;

    cout << " " << setw(espacosEsquerda + textoTitulo.length()) << right << textoTitulo << endl;
    
    cout << separadorTitulo << endl;

    cout << "| " << "ID: " << this->id << setw(maxLength + 2) << right << endl;
    cout << separador << endl;
    cout << "| " << setw(8) << left << "TITULO: " << this->titulo << setw(maxLength + 2) << right << endl;
    cout << separador << endl;
    cout << "| " << setw(8) << left << "AUTORES: " << this->autores << setw(maxLength + 2) << right << endl;
    cout << separador << endl;
    cout << "| " << "ANO: " << this->ano << setw(maxLength + 2) << right << endl;
    cout << separador << endl;
    cout << "| " << "CATEGORIA: " << this->categoria << setw(maxLength + 2) << right << endl;
    cout << separador << endl;
    cout << endl;
}



void Registro::exibirCampo(string campo, string valor) {

    int maxLength = valor.length();

    string separador(maxLength + 13, '-'); 
    string separadorTitulo(maxLength + 13, '=');

    cout << endl;
    cout << separadorTitulo << endl;
    
    int espacosEsquerda = ((separadorTitulo.length() - campo.length()) / 2) - 1;

    cout << " " << setw(espacosEsquerda + campo.length()) << right << campo << endl;
    
    cout << separadorTitulo << endl;

    cout << " " << setw(8) << left << valor << setw(maxLength + 2) << right << endl;

    cout << separador << endl;
}