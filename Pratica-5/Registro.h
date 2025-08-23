#ifndef REGISTRO_H
#define REGISTRO_H

#include <iostream>
#include <vector>


using namespace std;

class Registro {

public:
    
    long long id;
    string titulo;
    string autores;
    int ano;
    string categoria;

    /**
    * @brief Construtor padrão da classe Registro.
    */
    Registro();

    /** 
     * @brief Construtor da classe Registro.
     * @param id ID do livro.
     * @param titulo Título do livro.
     * @param autores Autores do livro.
     * @param ano Ano de publicação do livro.
     * @param categoria Categoria do livro.
     * 
    */
    Registro(long long id, string titulo, string autores, int ano, string categoria);

    /**
    * @brief Serializa o registro em uma string de tamanho fixo.
    * @details Converte o objeto em uma representação binária de tamanho fixo, onde o nome é armazenado nos primeiros 50 bytes e a idade nos 4 bytes finais.
    * @return Uma string representando o registro serializado em um formato de tamanho fixo.
    */
    string packFixed() const;

    /**
    * @brief Deserializa um registro a partir de uma string de tamanho fixo.
    * @details Converte uma string binária de tamanho fixo em um objeto Registro, onde os primeiros 50 bytes representam o nome e os 4 bytes finais representam a idade.
    * @param packedData A string contendo o registro serializado em um formato de tamanho fixo.
    */
    void unpackFixed(const string& packedData);

    /** Serializa o registro em uma string com campos separados por delimitador.
     * @details Converte o objeto em uma representação binária com campos separados por '|', onde os campos são Nome, Sobrenome, Telefone e Data de Nascimento.
     * @return Uma string representando o registro serializado em um formato com campos delimitados.
     */
    string packDelimitedWithDescriptor() const;

    /**
     * @brief Deserializa um registro a partir de uma string com campos separados por delimitador.
     * @details Converte uma string binária com campos separados por '|' em um objeto Registro, onde os campos são Nome, Sobrenome, Telefone e Data de Nascimento.
     * @param packedData A string contendo o registro serializado em um formato com campos delimitados.
     */
    void unpackDelimitedWithDescriptor(const string& packedData);

    /**
     * @brief Exibe o registro em uma visualização como uma tabela vertical.
     */
    void exibirRegistro();

    /**
     * @brief Exibe um campo específico do registro de uma forma formatada.
     * @param campo Qual é o campo a ser exibido.
     * @param valor O valor do campo a ser exibido.
     */
    void exibirCampo(string campo, string valor);

    /**
     * @brief Realiza o parser da linha oriunda do arquivo binário de registros, extraindo cada campo circundado por ||.
     * @param linha A linha a ser parseada.
     * @return Um vetor contendo os campos extraídos da linha.
     */
    vector<string> parserLinha(const string& linha);

};

#endif
