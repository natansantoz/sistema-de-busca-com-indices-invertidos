#ifndef INDICEINVERTIDOMANAGER_H
#define INDICEINVERTIDOMANAGER_H


#include <fstream>  
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <set>


#include "Registro.h"


using namespace std;


class IndiceInvertidoManager {

public:
    unordered_map<string, set<long long>> indicesInvertidos;
    unordered_set<string> stopWords;
    unordered_set<char> pontuacoes;

    string nomeDoArquivoIndicesInvertidos;
    string nomeDoArquivoStopWords;
    string nomeDoArquivoPontuacoes;

    IndiceInvertidoManager();

    IndiceInvertidoManager(string nomeArquivoStopWords, string nomeArquivoPontuacoes);
    
    void adicionarIndiceInvertido(Registro reg);

    void escreverIndicesInvertidos(string nomeDoArquivoIndicesInvertidos);

    void CarregarPontuacoes(const string& arq);

    string removerPontuacoes(const string& texto);

    void CarregarStopWords(const string &arq);

    string removerStopwords(const string &texto);

    void carregarIndicesInvertidos(string nomeArquivoIndicesInvertidos);

    string tratarTitulo(string titulo);

    set<long long> obterIntersecaoEntreSets(vector<set<long long>> sets);

    set<long long> buscarIdsViaIndicesInvertidos(string titulo);

};

#endif 
