#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <fstream>
#include <vector>


#include "Registro.h"
#include "Arvore.h"
#include "Indice.h"
#include "IndiceInvertidoManager.h"


using namespace std;


class Buffer {
public:
    bool arquivoIndicesEncontrado;
    bool exibirComparacoes;
    string nomeDoArquivo;
    string nomeDoArquivoCSV;
    string nomeDoArquivoIndices;
    string nomeDoArquivoIndicesInvertidos;

    // string nomeDoArquivoStopWords;
    // string nomeDoArquivoPontuacoes;

    ArvoreBinaria<Indice> arvoreIndices;
    IndiceInvertidoManager indiceInvertidoManager;

    ofstream arquivoBinarioRegistros;     // sem uso
    string dados;                         // sem uso


    Buffer();

    /**
    * @brief Construtor da classe Buffer.
    */
    Buffer(const string& nomeDoArquivo, IndiceInvertidoManager& indiceInvertidoManager);

    Buffer(const string& nomeDoArquivoCSV, string nomeDoArquivoBinario, string nomeDoArquivoIndices, 
                string nomeDoArquivoIndicesInvertidos, string nomeDoArquivoStopWords, string nomeDoArquivoPontuacoes);


    /**
     * @brief Lê registros de um arquivo CSV e os armazena em um vetor de registros.
     * @details Realiza a leitura de cada linha do arquivo CSV e converte os dados em objetos Registro.
     * @return vector<Registro> O vetor contendo os registros lidos do arquivo CSV.
     */
    vector<Registro> lerRegistrosCSVLivros();

    /**
     * @brief Lê registros com delimitadores de um arquivo binário e os armazena em um vetor de registros.
     * @details Lê blocos de dados binários com delimitadores (registross) do arquivo e os converte em objetos Registro.
     * Cada registro lido é composto por campos delimitados por '||'.
     * @return vector<Registro> o vetor contendo os registros lidos do arquivo binário.
     */ 
    vector<Registro> lerRegistrosDelimitados();

    /**
     * @brief Adiciona registros delimitados ao arquivo binário.
     * @details Adiciona registros ao arquivo binário, onde cada registro é composto por campos delimitados por '||'
     * @param registros O vetor contendo os registros a serem adicionados ao arquivo binário.
     */
    void adicionarRegistrosDelimitados(vector<Registro>& registros);

    /**
     * @brief Chama o método de salvar índices da árvore de índices. 
     * @details Percorre a árvore de índices em largura e salva os índices no arquivo de índices.
     */
    void salvarIndices();

    /**
     * @brief Busca um registro no arquivo binário a partir de um ID.
     * @details Busca pelo ID do registro na árvore de índices e, caso encontrado, lê o registro no arquivo binário por meio do offset.
     * @param id O ID do registro a ser buscado.
     * @return O registro encontrado no arquivo binário.
     */
    Registro buscarRegistroPorId(long long &id);

    /**
     * @brief Adiciona um novo registro delimitado ao arquivo binário.
     * @details Adiciona um novo registro ao arquivo binário, onde o registro é composto por campos delimitados por '||'.
     * @param reg O registro a ser adicionado ao arquivo binário.
     */
    void adicionarNovoRegistroDelimitado(Registro& reg, ofstream& arquivoBinarioRegistros, bool salvarIndices);

    /**
     * @brief método com apenas um parâmetro, para simplificar a adição de um novo registro.
     * @param reg O registro a ser adicionado ao arquivo binário.
     */
    void adicionarNovoRegistroDelimitado(Registro& reg);

    /**
     * @brief método setter para o nome do arquivo de índices.
     * @param nomeDoArquivoIndices O nome do arquivo de índices.
     */
    void setNomeDoArquivoIndices(const string& nomeDoArquivoIndices);

    /**
     * @brief método setter para o nome do arquivo CSV.
     * @param nomeDoArquivoCSV O nome do arquivo CSV.
     */
    void setNomeDoArquivoCSV(const string& nomeDoArquivoCSV);

    /**
     * @brief método setter para o nome do arquivo binário.
     * @param nomeDoArquivo O nome do arquivo binário.
     */
    void setNomeDoArquivoBinario(const string& nomeDoArquivo);


    void setNomeDoArquivoIndicesInvertidos(const string& nomeDoArquivoIndicesInvertidos);

    void setNomeDoArquivoStopWords(const string& nomeDoArquivoStopWords);

    void setNomeDoArquivoPontuacoes(const string& nomeDoArquivoIndicesInvertidos);

    vector<Registro> buscarRegistrosPeloTitulo(string titulo);


    /**
     * @brief Realiza o parser da linha oriunda do CSV, extraindo cada campo e tratando ; no meio de aspas nos campos.
     * @param linha A linha do CSV a ser parseada.
     * @return Um vetor contendo os campos extraídos da linha.
     */
    vector<string> parserLinhaCSV(const string& linha);

    ~Buffer();
};


#endif