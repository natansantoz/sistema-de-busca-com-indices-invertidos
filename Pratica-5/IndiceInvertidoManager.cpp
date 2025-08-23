#include <iostream>

#include <string.h>
#include <cctype>
#include <algorithm>
#include<sstream>


#include "IndiceInvertidoManager.h"
#include "IndiceInvertido.h"
#include "Indice.h"

using namespace std;


IndiceInvertidoManager::IndiceInvertidoManager(){}


IndiceInvertidoManager::IndiceInvertidoManager(string nomeArquivoStopWords, string nomeArquivoPontuacoes) {
    this->indicesInvertidos = unordered_map<string, set<long long>>();
    this->stopWords = unordered_set<string>();
    this->pontuacoes = unordered_set<char>();

    CarregarStopWords(nomeArquivoStopWords);
    CarregarPontuacoes(nomeArquivoPontuacoes);
}


void IndiceInvertidoManager::adicionarIndiceInvertido(Registro reg) {

    // remover stopwords
    string tituloTratado = reg.titulo;

    transform(
        tituloTratado.begin(),
        tituloTratado.end(),
        tituloTratado.begin(),
        [](unsigned char c) {
            return tolower(c);
        }
    );

    tituloTratado = removerPontuacoes(tituloTratado);
    tituloTratado = removerStopwords(tituloTratado);

    istringstream stream(tituloTratado);
    string palavra;

    // Extraindo as palavras do titulo e vinculando o id do registro atual (criando/populando os indices invertidos)
    while (stream >> palavra) {
        indicesInvertidos[palavra].insert(reg.id);
    }
}

void IndiceInvertidoManager::escreverIndicesInvertidos(string nomeDoArquivoIndicesInvertidos) {

    ofstream arquivoBinarioIndicesInvertidos(nomeDoArquivoIndicesInvertidos, ios::app | ios::binary);

    for (unordered_map<string, set<long long>>::iterator it = this->indicesInvertidos.begin(); it != this->indicesInvertidos.end(); ++it) {

        const pair<const string, set<long long>>& par = *it;

        IndiceInvertido indiceInvertidoAtual(par.first, par.second);

        string buffer = indiceInvertidoAtual.packIndiceInvertido();

        arquivoBinarioIndicesInvertidos.write(reinterpret_cast<const char*>(buffer.c_str()), buffer.size());
    }
}


void IndiceInvertidoManager::CarregarPontuacoes(const string& arq) {
    set<char> pontuacoes;
    ifstream arquivo(arq);
    string linha;

    if (!arquivo.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo de pontuações: " + arq);
    }

    while (getline(arquivo, linha)) {

        linha.erase(0, linha.find_first_not_of(" \t\n\r\f\v"));
        linha.erase(linha.find_last_not_of(" \t\n\r\f\v") + 1);

        for (char c : linha) {
            if (!isspace(static_cast<unsigned char>(c))) {
                this->pontuacoes.insert(c);
            }
        }
    }

    arquivo.close();
}


string IndiceInvertidoManager::removerPontuacoes(const string& texto) {
    ostringstream oss;

    for (size_t i = 0; i < texto.length(); ++i) {
      	if(
      	    (i == 0 && (this->pontuacoes.find(texto[i]) != this->pontuacoes.end()))
      	        ||
      	    (i == 0 && texto[i] == ' ')
      	)
      	    continue;


        if(
            (texto[i] == ' ' && (texto[i - 1] == ' '))
                ||
            (
                this->pontuacoes.find(texto[i]) != this->pontuacoes.end()
                &&
                this->pontuacoes.find(texto[i - 1]) != this->pontuacoes.end()
            )
        )
            continue;


        if ((this->pontuacoes.find(texto[i]) != this->pontuacoes.end())) {
        	if((this->pontuacoes.find(texto[i - 1]) == this->pontuacoes.end() && texto[i - 1] != ' ')) {

        	    if (texto[i] == ',' && isdigit(texto[i - 1]) && isdigit(texto[i + 1])) continue;

        	    if (i != (texto.size() - 1)) {
        	        oss << ' ';
        	    }

        		continue;
            }

            continue;
        }

        if (oss.str().size() >= 2) {
            if (texto[i] == ' ' && (oss.str()[oss.str().size() - 1] == ' ')) continue;
        }

        oss << texto[i];
    }

    return oss.str();
}


void IndiceInvertidoManager::CarregarStopWords(const string &arq){
    ifstream arquivo(arq);
    string palavra;

    if (!arquivo.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo de stopwords: " + arq);
    }

    while (getline(arquivo, palavra)) {

        palavra.erase(0, palavra.find_first_not_of(" \t\n\r\f\v"));
        palavra.erase(palavra.find_last_not_of(" \t\n\r\f\v") + 1);

        if (palavra.size () > 0) {
            this->stopWords.insert(palavra);
        }
    }

    arquivo.close();
}

string IndiceInvertidoManager::removerStopwords(const string &texto) {
    istringstream iss(texto);
    ostringstream oss;
    string palavra;

    while (iss >> palavra) {

        if (this->stopWords.find(palavra) == this->stopWords.end()) { // Se a palavra não for uma stopword
            oss << palavra << " ";
        }
    }
    return oss.str();
}

void IndiceInvertidoManager::carregarIndicesInvertidos(string nomeArquivoIndicesInvertidos) {
    ifstream in(nomeArquivoIndicesInvertidos, ios::binary);

    if (!in.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo em carregarIndicesInvertidos.");
    }

    while (in.peek() != EOF) {
        int tamanhoRegistro;
        
        in.read(reinterpret_cast<char*>(&tamanhoRegistro), sizeof(int));
        
        char* buffer = new char[tamanhoRegistro];
        in.read(buffer, tamanhoRegistro);

        string registroStr(buffer, tamanhoRegistro);
        delete[] buffer;

        IndiceInvertido IndiceInvertido;
        IndiceInvertido.unpackIndiceInvertido(registroStr); 

        this->indicesInvertidos[IndiceInvertido.palavra] = IndiceInvertido.ids;
    }

    in.close();
}

string IndiceInvertidoManager::tratarTitulo(string titulo) {
    string tituloTratado = titulo;

    transform(
        tituloTratado.begin(),
        tituloTratado.end(),
        tituloTratado.begin(),
        [](unsigned char c) {
            return tolower(c);
        }
    );

    tituloTratado = removerPontuacoes(tituloTratado);
    tituloTratado = removerStopwords(tituloTratado);

    return tituloTratado;
}

set<long long> IndiceInvertidoManager::obterIntersecaoEntreSets(vector<set<long long>> sets) {

    set<long long> intersecao = sets[0];

    for (size_t i = 1; i < sets.size(); ++i) {
        set<long long> aux;

        set_intersection(
            intersecao.begin(), intersecao.end(),
            sets[i].begin(), sets[i].end(),
            inserter(aux, aux.begin())
        );
        intersecao = aux;
    }

    return intersecao;
}

set<long long> IndiceInvertidoManager::buscarIdsViaIndicesInvertidos(string titulo) {

    string tituloTratado = tratarTitulo(titulo);

    vector<set<long long>> setsCorrespondentes;

    istringstream stream(tituloTratado);
    string palavra;

    while (stream >> palavra) {
        
        try {

            setsCorrespondentes.push_back(indicesInvertidos[palavra]);


        } catch (exception& e) {
            return set<long long>();
        }
    }

    if (setsCorrespondentes.size() == 0) return set<long long>();

    set<long long> intersecao = obterIntersecaoEntreSets(setsCorrespondentes);

    return intersecao;
}
