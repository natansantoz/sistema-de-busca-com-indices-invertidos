#ifndef INDICE_H
#define INDICE_H

#include <fstream>  

using namespace std;

class Indice {     

public:
    long long id;                  
    streampos offset;  

    /**
     * @brief Construtor da classe Indice.
     * @param i O ID do registro.
     * @param o O offset do registro.
     */
    Indice(long long i, streampos o);  

    /**
     * @brief Construtor da classe Indice.
     */
    Indice();                         
    
    /**
     * @brief Retorna o ID do registro.
     * @return long long O ID do registro.
     */
    long long getId() const;

    /**
     * @brief Retorna o offset do registro.
     * @return streampos O offset do registro.
     */
    streampos getOffset() const;

    /**
     * @brief Sobrecarga do operador de comparação >
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for maior que o ID do outro índice, false caso contrário.
     */
    bool operator>(const Indice& outro) const;

    /**
     * @brief Sobrecarga do operador de comparação <
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for menor que o ID do outro índice, false caso contrário.
     */
    bool operator<(const Indice& outro) const;

    /**
     * @brief Sobrecarga do operador de comparação ==
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for igual ao ID do outro índice, false caso contrário.
     */
    bool operator==(const Indice& outro) const;

    /**
     * @brief Sobrecarga do operador de comparação !=
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for diferente do ID do outro índice, false caso contrário.
     */
    bool operator!=(const Indice& outro) const;

    /**
     * @brief Sobrecarga do operador de comparação <=
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for menor ou igual ao ID do outro índice, false caso contrário.
     */
    bool operator<=(const Indice& outro) const;

    /**
     * @brief Sobrecarga do operador de comparação >=
     * @param outro O outro índice a ser comparado.
     * @return bool True se o ID do índice for maior ou igual ao ID do outro índice, false caso contrário.
     */
    bool operator>=(const Indice& outro) const;

    /**
     * @brief Define o ID do registro.
     * @param i O ID do registro.
     */
    void setId(long long id);

    /**
     * @brief Define o offset do registro.
     * @param o O offset do registro.
     */
    void setOffset(streampos o);

    /**
     * @brief Exibe o ID e o offset do registro.
     */
    void exibir() const;

    /**
     * @brief Serializa o índice em uma string de tamanho fixo.
     * @details Converte o objeto em uma representação binária de tamanho fixo
     * @return Uma string representando o índice serializado em um formato de tamanho fixo.
     */
    string pack() const;

    /**
     * @brief Deserializa um índice a partir de uma string de tamanho fixo.
     * @details Converte uma string binária de tamanho fixo em um objeto Indice.
     * @param packedData A string contendo o índice serializado em um formato de tamanho fixo.
     */
    void unpack(ifstream& arquivoBinarioRegitros);


    /**
     * @brief Sobrecarga do operador de inserção em stream, permitindo a impressão do objeto Indice de forma formatada.
     * @param os A stream de saída.
     * @param O objeto da classe Indice a ser impresso.
     * @return ostream& A stream de saída modificada.
     */
    friend ostream& operator<<(ostream& os, const Indice& indice);
};

#endif 
