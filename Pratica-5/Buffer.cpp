#include <fstream>
#include <vector>
#include <string> 
#include <algorithm>
#include <stdexcept>
#include <sstream>


#include "Buffer.h"



Buffer::Buffer() {}

Buffer::Buffer(const string& nomeDoArquivoCSV, string nomeDoArquivoBinario, string nomeDoArquivoIndices, 
                string nomeDoArquivoIndicesInvertidos, string nomeDoArquivoStopWords, string nomeDoArquivoPontuacoes) {

    this->arquivoIndicesEncontrado = false;
    this->exibirComparacoes = false;
    this->nomeDoArquivoCSV = nomeDoArquivoCSV;
    this->nomeDoArquivo = nomeDoArquivoBinario;
    this->nomeDoArquivoIndices = nomeDoArquivoIndices;
    this->nomeDoArquivoIndicesInvertidos = nomeDoArquivoIndicesInvertidos;

    this->indiceInvertidoManager = IndiceInvertidoManager(nomeDoArquivoStopWords, nomeDoArquivoPontuacoes);

}

Buffer::Buffer(const string& nomeDoArquivoCSV, IndiceInvertidoManager& indiceInvertidoManager) {
    this->arquivoIndicesEncontrado = false;
    this->exibirComparacoes = false;
    this->nomeDoArquivoCSV = nomeDoArquivoCSV;
    this->indiceInvertidoManager = indiceInvertidoManager;
}

Buffer::~Buffer() {}



void Buffer::setNomeDoArquivoIndices(const string& nomeDoArquivoIndices) {
    this->nomeDoArquivoIndices = nomeDoArquivoIndices;
}

void Buffer::setNomeDoArquivoCSV(const string& nomeDoArquivoCSV) {
    this->nomeDoArquivoCSV = nomeDoArquivoCSV;
}


void Buffer::setNomeDoArquivoBinario(const string& nomeDoArquivo) {
    this->nomeDoArquivo = nomeDoArquivo;
}





vector<string> Buffer::parserLinhaCSV(const string& linha) {
    vector<string> campos;
    string campo;
    bool dentroAspas = false;

    for(size_t i = 0; i < linha.size(); i++) {
        
        if (linha[i] == '"') { // Se é aspas, então ou entramos ou saímos de um campo com um ; no meio dele
            dentroAspas = !dentroAspas;

        }else if (linha[i] == ';' && !dentroAspas) { // se é ponto e vírgula e não esta dentro de aspas, então é um delimitador de campo
            campos.push_back(campo);
            campo.clear(); 

        }else {
            campo += linha[i]; 
        }

        if(linha[i] == '"' && linha[i + 1] == '"' && linha[i + 2] != ';') {
            campo += '"';
        }
    }

    campos.push_back(campo);

    return campos;
}


vector<Registro> Buffer::lerRegistrosCSVLivros() {
    
    ifstream arquivoCSV(this->nomeDoArquivoCSV);

    if (!arquivoCSV.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo CSV.");
    }

    vector<Registro> registros;

    string linha, id, titulo, autores, anoString, categoria;

    // Ignorando o cabeçalho
    getline(arquivoCSV, linha); 

    while (getline(arquivoCSV, linha)) {

        if (!linha.empty() && linha[linha.size() - 1] == '\r') {
            linha.erase(linha.size() - 1);
        }

        vector<string> campos = parserLinhaCSV(linha); 

        id = campos[0];
        titulo = campos[1];
        autores = campos[2];
        anoString = campos[3];
        categoria = campos[4];

        int ano = stoi(anoString);
        long long idL = stoll(id);
        registros.emplace_back(idL, titulo, autores, ano, categoria);
    }
    
    arquivoCSV.close();
    return registros;
}


void Buffer::adicionarRegistrosDelimitados(vector<Registro>& registros) {
    ofstream arquivoBinarioRegistros(this->nomeDoArquivo, ios::app | ios::binary);

    ofstream arquivoBinarioIndicesInvertidos(this->nomeDoArquivoIndicesInvertidos, ios::app | ios::binary);

    cout << "Criando os indices em memoria..." << endl;

    for (Registro& reg : registros) {
        adicionarNovoRegistroDelimitado(reg, arquivoBinarioRegistros, false);
    }

    cout << "Salvando indice primario..." << endl;

    this->salvarIndices();

    cout << "Salvando indice invertido..." << endl;

    this->indiceInvertidoManager.escreverIndicesInvertidos(this->nomeDoArquivoIndicesInvertidos);

    cout << "Indices criados com sucesso!" << endl;


    arquivoBinarioRegistros.close();
    arquivoBinarioIndicesInvertidos.close();
}


void Buffer::adicionarNovoRegistroDelimitado(Registro& reg, ofstream& arquivoBinarioRegistros, bool salvarIndices) {

    streampos offset = arquivoBinarioRegistros.tellp();

    string buffer = reg.packDelimitedWithDescriptor();

    arquivoBinarioRegistros.write(reinterpret_cast<const char*>(buffer.c_str()), buffer.size());
    
    this->arvoreIndices.Inserir(Indice(reg.id, offset));

    this->indiceInvertidoManager.adicionarIndiceInvertido(reg);

    if(salvarIndices) {
        this->salvarIndices();
        this->indiceInvertidoManager.escreverIndicesInvertidos(this->nomeDoArquivoIndicesInvertidos);
    }
}


void Buffer::adicionarNovoRegistroDelimitado(Registro& reg) {

    if(this->arvoreIndices.Pesquisar(Indice(reg.id, 0), false).id != -1) {
        cout << "\nRegistro com ID " << reg.id << " ja existe. \n" << endl;
        return;
    }

    ofstream arquivoBinarioRegistros(this->nomeDoArquivo, ios::app | ios::binary);

    arquivoBinarioRegistros.seekp(0, ios::end);

    adicionarNovoRegistroDelimitado(reg, arquivoBinarioRegistros, true); 

    arquivoBinarioRegistros.close();
}


void Buffer::salvarIndices() {
    arvoreIndices.SalvarIndicesTravessiaLargura(this->nomeDoArquivoIndices);
}


Registro Buffer::buscarRegistroPorId(long long &id) {
    Indice indiceEncontrado = this->arvoreIndices.Pesquisar(Indice(id, 0), this->exibirComparacoes);
    
    if (indiceEncontrado.id == -1) {
        return Registro();
    }
    
    ifstream arquivoBinarioRegitros(this->nomeDoArquivo, ios::binary);

    if (!arquivoBinarioRegitros.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo de registros.");
    }

    arquivoBinarioRegitros.seekg(indiceEncontrado.offset, std::ios::beg);

    int tamanhoRegistro;
    arquivoBinarioRegitros.read(reinterpret_cast<char*>(&tamanhoRegistro), sizeof(int));

    char* buffer = new char[tamanhoRegistro];
    arquivoBinarioRegitros.read(buffer, tamanhoRegistro);

    string registroStr(buffer, tamanhoRegistro);
    delete[] buffer;

    Registro reg;
    
    reg.unpackDelimitedWithDescriptor(registroStr);

    arquivoBinarioRegitros.close();

    return reg;
}

vector<Registro> Buffer::lerRegistrosDelimitados() {
    ifstream in(this->nomeDoArquivo, ios::binary);

    if (!in.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo em lerRegistrosDelimitados.");
    }

    vector<Registro> registros;

    while (in.peek() != EOF) {
        int tamanhoRegistro;
        
        in.read(reinterpret_cast<char*>(&tamanhoRegistro), sizeof(int));
        
        char* buffer = new char[tamanhoRegistro];
        in.read(buffer, tamanhoRegistro);

        string registroStr(buffer, tamanhoRegistro);
        delete[] buffer;

        Registro reg;
        reg.unpackDelimitedWithDescriptor(registroStr); 

        registros.push_back(reg);
    }
    
    in.close();
    return registros;
}


vector<Registro> Buffer::buscarRegistrosPeloTitulo(string titulo) {

    set<long long> idsCorrespondentes = this->indiceInvertidoManager.buscarIdsViaIndicesInvertidos(titulo);

    if(idsCorrespondentes.empty()) return vector<Registro>();
    
    vector<Registro> registrosCorrespondentes;

    for (long long id : idsCorrespondentes) {
        Registro registro = this->buscarRegistroPorId(id);

        if (registro.id == -1) continue;

        registrosCorrespondentes.push_back(registro);
    }

    return registrosCorrespondentes;
}
