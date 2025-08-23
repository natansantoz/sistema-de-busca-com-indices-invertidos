/**
 * Arquivo com funções auxiliares para a exibição de menus e interação com o usuário.
*/


#ifndef MENU_H
#define MENU_H

#define _NO_BYTES

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>  
#include <cctype>
#include <map>
#include <chrono> 
#include <cstdio>
#include <thread>
#include <iomanip>
#include <cstddef> // Para std::byte (se necessário)


#include "Buffer.h"

#ifdef _WIN32
    void limparConsole() {
        system("cls");
    }
#else
    #include <unistd.h>
    void limparConsole() {
        system("clear");
    }
#endif


using namespace std;


string obterTituloParaPesquisa() {

    limparConsole();

    Registro novoRegistro;

    cout << "\n===================================================" << endl;
    cout << "                 Buscar via Titulo                   " << endl;
    cout << "===================================================" << endl;

    string titulo;

    while (true) {
        cout << "Digite o titulo do livro: ";

        getline(cin, titulo);

        if (titulo.find('\n') != string::npos) {
            titulo.erase(titulo.find('\n'), 1); 
        }

        if ((titulo.find_first_not_of(" \t\n\r\f\v") == string::npos) || titulo.size() == 0) {
            cout << "\nTitulo invalido. Tente novamente.\n" << endl;
            continue;
        }

        break;
    }

    return titulo;
}

string validarRespostaSimNao(string mensagem) {
    string resposta;
    bool respostaValida = false;
    
    while(!respostaValida) {
        
        cout << mensagem;

        cin >> resposta;

        if (resposta.length() == 1 && (resposta[0] == 's' || resposta[0] == 'n')) {
            respostaValida = true;

        } else {
            cout << "\nResposta invalida. Digite 's' para sim ou 'n' para nao.\n" << endl;
        }
    }

    return resposta;
}

void carregarIndices(Buffer& buffer) {

    ifstream arquivoBinarioRegistros(buffer.nomeDoArquivo, ios::binary);
    
    if (arquivoBinarioRegistros.is_open()) {
        arquivoBinarioRegistros.close();

        cout << "\nArquivo de indices encontrado." << endl;

        string respostaCarregarIndices = validarRespostaSimNao("Deseja carregar os indices existentes? (s/n): ");
        
        buffer.arquivoIndicesEncontrado = true;
        
        if (respostaCarregarIndices[0] == 's') {

            cout << "\nCarregando indice primario..." << endl;

            buffer.arvoreIndices.carregarIndices(buffer.nomeDoArquivoIndices);

            cout << "Carregando indice invertido..." << endl;

            buffer.indiceInvertidoManager.carregarIndicesInvertidos(buffer.nomeDoArquivoIndicesInvertidos);

            cout << "\nIndices carregados com sucesso!\n" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            limparConsole();

            string respostaExibirComparacoes = "n";
            // respostaExibirComparacoes = validarRespostaSimNao("Deseja exibir o numero de nos visitados durante a consulta? (s/n): ");

            if (respostaExibirComparacoes[0] == 's') {
                buffer.exibirComparacoes = true;
            }
            
            return;
        } 
    }

    remove(buffer.nomeDoArquivo.c_str());
    remove(buffer.nomeDoArquivoIndices.c_str());
    remove(buffer.nomeDoArquivoIndicesInvertidos.c_str());

    cout << "\nLendo registros do arquivo csv..." << endl;
    vector<Registro> registros = buffer.lerRegistrosCSVLivros();
    cout << "Registros lidos com sucesso!" << endl;


    buffer.adicionarRegistrosDelimitados(registros);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    string respostaExibirComparacoes = "n";
    // respostaExibirComparacoes = validarRespostaSimNao("Deseja exibir o numero de nos visitados durante a consulta? (s/n): ");

    if (respostaExibirComparacoes[0] == 's') {
        buffer.exibirComparacoes = true;
    }

    limparConsole();
}


int obterInteiroValido(string mensagem) {
    long long idLivro;
    string idLivroString;

    cin.clear();

    while (true) {
        cout << mensagem;

        getline(cin, idLivroString);

        if (idLivroString.empty() || !all_of(idLivroString.begin(), idLivroString.end(), ::isdigit)) {
            cout << "\nDigite um numero inteiro valido.\n" << endl;
            cin.clear();  
            continue;
        } 

        try {
            idLivro = stoll(idLivroString);
        } catch (const out_of_range& e) {
            cout << "\nO valor excede o limite suportado.\n" << endl;
            cin.clear();  
            continue;
        }

        if (idLivro < 0) {
            cout << "\nDigite um numero inteiro positivo.\n" << endl;
            cin.clear();  
            continue;
        }

        if (mensagem.find("ano") != string::npos) {

            auto lt = chrono::system_clock::to_time_t(chrono::system_clock::now());
            struct tm *time = localtime(&(lt));
            int anoAtual = time->tm_year + 1900;   

            if (idLivro > anoAtual) {
                cout << "\nDigite um ano menor ou igual ao ano atual.\n" << endl;
                cin.clear();  
                continue;
            }
        }

        break;
    }

    return idLivro;
}


Registro obterIdEBuscarLivro(Buffer& arquivo) {
    long long idLivro = obterInteiroValido("Digite o ID do livro: ");

    Registro registro = arquivo.buscarRegistroPorId(idLivro);

    if (registro.id == -1) {
        cout << "\nNenhum livro com ID " << idLivro << " foi encontrado." << endl;
    }

    return registro;
}

void adicionarRegistro(Buffer& arquivo) {

    Registro novoRegistro;

    cout << "\n===================================================" << endl;
    cout << "                  Adicionar Registro             " << endl;
    cout << "===================================================" << endl;

    while (true) {
        novoRegistro.id = obterInteiroValido("Digite o ID do livro: ");

        if(arquivo.arvoreIndices.Pesquisar(Indice(novoRegistro.id, 0), false).id != -1) {
            cout << "\nJa existe um livro com o ID " << novoRegistro.id << ", escolha outro ID.\n" << endl;
            continue;
        }

        break;
    }
    
    cout << "Digite o titulo do livro: ";

    getline(cin, novoRegistro.titulo);

    cout << "Digite o(s) autor(es) do livro: ";

    getline(cin, novoRegistro.autores);

    novoRegistro.ano = obterInteiroValido("Digite o ano de publicacao do livro: ");

    cout << "Digite a categoria do livro: ";
    getline(cin, novoRegistro.categoria);

    arquivo.adicionarNovoRegistroDelimitado(novoRegistro);

    cout << "\nRegistro adicionado com sucesso!\n" << endl;
}

long long validarOpcaoNumericaDigitada(string opcao) {
    long long opcaoNum = -1;

    if (!(opcao.empty()) && all_of(opcao.begin(), opcao.end(), ::isdigit)) {
        opcaoNum = stoll(opcao);

        if (opcaoNum < 0) return -1;

        return opcaoNum;
    }

    cout << "\nOpcao invalida, tente novamente." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return -1;
}

tuple<int, int, int, int, int> calcularTamanhosFormatacaoTabelaPaginada(int paginaAtual, vector<Registro> registrosEncontrados, int registrosPorPagina) {
    int inicio = (paginaAtual - 1) * registrosPorPagina;
    int fim = min(inicio + registrosPorPagina, static_cast<int>(registrosEncontrados.size()));

    int tamIdMaisLongo = 0;
    int tamTituloMaisLongo = 0;
    int idDoRegistroMaisLongo = 0;

    for (int i = inicio; i < fim; i++) {
        tamIdMaisLongo = max(static_cast<size_t>(tamIdMaisLongo), to_string(registrosEncontrados[i].id).length());

        if (registrosEncontrados[i].titulo.length() > static_cast<string::size_type>(tamTituloMaisLongo)) {
            idDoRegistroMaisLongo = registrosEncontrados[i].id;
        }

        tamTituloMaisLongo = 150;
    }

    int tamLinha = ("|   ID    " + string("   | Titulo   ") + string(145, ' ')).size() + 1;

    return make_tuple(tamLinha, tamTituloMaisLongo, idDoRegistroMaisLongo, inicio, fim);

}

void exibirTabelaRegistrosEncontrada(int tamLinha, int tamTituloMaisLongo, vector<Registro> registrosEncontrados, int idDoRegistroMaisLongo, int paginaAtual, int totalPaginas, int inicio, int fim) {
    cout << "\n" << string(tamLinha, '=') << endl;
    cout << string((tamLinha / 2) - 10, ' ') << "Registros Encontrados             " << endl;
    cout << "" << string(tamLinha, '=') << endl;

    cout << "|     ID     |" << string((tamTituloMaisLongo - 2)/2, ' ') << "    Titulo" << string((tamTituloMaisLongo - 2) / 2 - 3, ' ') <<"|" << endl;

    for (int i = inicio; i < fim; i++) {

        size_t espacosDireita = max(0, static_cast<int>((tamTituloMaisLongo ) - registrosEncontrados[i].titulo.size()));
        int espacosEsquerda = 12 - 2 - to_string(registrosEncontrados[i].id).size();

        if (registrosEncontrados[i].id == idDoRegistroMaisLongo && registrosEncontrados[i].titulo.size() > 150) {
            cout << "| " << registrosEncontrados[i].id << string(espacosEsquerda, ' ') <<" | " << registrosEncontrados[i].titulo.substr(0, 150) << "..." << string(espacosDireita, ' ') << " | \n";

        } else {
            cout << "| " << registrosEncontrados[i].id << string(espacosEsquerda, ' ') <<" | " << registrosEncontrados[i].titulo << string(espacosDireita, ' ') << "    | \n";
        }
    }

    cout << "\nPagina " << paginaAtual << " de " << totalPaginas << endl;
}


void exibirPaginacaoDeRegistrosEncontrados(vector<Registro> registrosEncontrados) {

    int registrosPorPagina = 10;
    int paginaAtual = 1;
    int totalPaginas = registrosEncontrados.size() / registrosPorPagina;

    if (registrosEncontrados.size() % registrosPorPagina != 0) {
        totalPaginas++;
    }

    map<int, string> opcoesDoMenu = {
        {1, "Opcoes de Paginacao"},
        {2, "Opcoes de Ver Detalhes"}
    };

    string opcaoDoMenu = "Opcoes de Paginacao";

    while (true) {
        limparConsole();

        int tamLinha, tamTituloMaisLongo, idDoRegistroMaisLongo, inicio, fim;

        tie(tamLinha, tamTituloMaisLongo, idDoRegistroMaisLongo, inicio, fim) = calcularTamanhosFormatacaoTabelaPaginada(paginaAtual, registrosEncontrados, registrosPorPagina);
        
        exibirTabelaRegistrosEncontrada(tamLinha, tamTituloMaisLongo, registrosEncontrados, idDoRegistroMaisLongo, paginaAtual, totalPaginas, inicio, fim);

        if (opcaoDoMenu == "Opcoes de Paginacao") {
            cout << "\n[1] Pagina Anterior" << endl;
            cout << "[2] Proxima Pagina" << endl;
            cout << "[3] Ver Detalhes de um Registro" << endl;
            cout << "[4] Retornar ao Menu Anterior" << endl;

            cout << "\nDigite a opcao desejada: ";

            string opcao;
            int opcaoInt;

            cin.clear();

            getline(cin, opcao);

            if (!(opcao.empty()) && all_of(opcao.begin(), opcao.end(), ::isdigit)) {
                opcaoInt = stoi(opcao);

                if (opcaoInt == 4) return;

                if (opcaoInt == 3) { opcaoDoMenu = "Opcoes de Ver Detalhes"; continue; }

                if (opcaoInt == 2) { if (paginaAtual < totalPaginas) paginaAtual++; continue; }

                if (opcaoInt == 1) { if (paginaAtual > 1) paginaAtual--; continue; }

                cout << "\nOpcao invalida, tente novamente." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));

            }else {
                cout << "\nOpcao invalida, tente novamente." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }


        if (opcaoDoMenu == "Opcoes de Ver Detalhes") {
            bool voltarAListagem = false;

            while (!voltarAListagem) {

                cout << "\nDigite o ID do registro que deseja visualizar ou (r) para retornar a listagem: ";

                string opcaoSelecionada;
                long long idRegistro;

                cin.clear();
                getline(cin, opcaoSelecionada);


                if (opcaoSelecionada == "r") { opcaoDoMenu = "Opcoes de Paginacao"; break; }

                idRegistro = validarOpcaoNumericaDigitada(opcaoSelecionada);

                if (idRegistro == -1) {
                    limparConsole();
                    exibirTabelaRegistrosEncontrada(
                        tamLinha,
                        tamTituloMaisLongo,
                        registrosEncontrados,
                        idDoRegistroMaisLongo,
                        paginaAtual,
                        totalPaginas,
                        inicio,
                        fim
                    );
                    continue;
                }

                auto it = find_if(
                    registrosEncontrados.begin(),
                    registrosEncontrados.end(),
                    [idRegistro](Registro& reg) {
                        return reg.id == idRegistro;
                    }
                );

                if (it != registrosEncontrados.end()) {

                    while (true) {
                        limparConsole();

                        it->exibirRegistro();

                        cout << "\n[1] Retornar a Lista de Registros: ";

                        string opcao;
                        cin.clear();
                        getline(cin, opcao);

                        long long opcaoDigitada = validarOpcaoNumericaDigitada(opcao);

                        if (opcaoDigitada == -1) continue;

                        if (opcaoDigitada == 1) { voltarAListagem = true; break; }

                        if (opcaoDigitada != 1) {
                            cout << "\nOpcao invalida, tente novamente." << endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            limparConsole();
                            exibirTabelaRegistrosEncontrada(tamLinha, tamTituloMaisLongo, registrosEncontrados, idDoRegistroMaisLongo, paginaAtual, totalPaginas, inicio, fim);
                        }
                    }

                } else {
                    cout << "\nNenhum registro foi encontrado com o ID " << idRegistro << "." << endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    limparConsole();
                    exibirTabelaRegistrosEncontrada(tamLinha, tamTituloMaisLongo, registrosEncontrados, idDoRegistroMaisLongo, paginaAtual, totalPaginas, inicio, fim);
                }
            }
        }

    }
}

void exibirSecaoBuscarViaTitulo(Buffer& arquivo) {

    string titulo;

    while (true) {

        limparConsole();

        cout << "\n==================================================================================" << endl;
        cout << "                                 Buscar via Titulo                                  " << endl;
        cout << "==================================================================================" << endl;

        cout << "\nDigite o Titulo do Livro ou (sair) para Retornar ao Menu Principal: ";

        getline(cin, titulo);

        if (titulo == "sair") {
            cin.clear();
            limparConsole();
            return;
        }

        if (titulo.find('\n') != string::npos) {
            titulo.erase(titulo.find('\n'), 1); 
        }

        if ((titulo.find_first_not_of(" \t\n\r\f\v") == string::npos) || titulo.size() == 0) {
            cout << "\nTitulo invalido. Tente novamente.\n" << endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));
            titulo = "";
            continue;
        }

        vector<Registro> registrosEncontrados = arquivo.buscarRegistrosPeloTitulo(titulo);
        
        if(registrosEncontrados.size() == 0) {
            cout << "\nNenhum registro foi encontrado para o titulo '" << titulo << "'." << endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));

            continue;
        }

        exibirPaginacaoDeRegistrosEncontrados(registrosEncontrados);
    }

}

void exibirMenuBuscar(Buffer& arquivo, const string& campo) {

    if(campo == "Adicionar Novo Registro") {
        limparConsole();
        adicionarRegistro(arquivo);

        cout << "\nPressione Enter para continuar...";
        cin.get();
        limparConsole();
        return;
    }

    if(campo == "Buscar Registro via Titulo") {
        exibirSecaoBuscarViaTitulo(arquivo);
        return;
    }

    Registro registro = obterIdEBuscarLivro(arquivo);

    if(registro.id == -1) return;

    map<string, string> campos = {
        {"Titulo", registro.titulo},
        {"Autores", registro.autores},
        {"Ano", to_string(registro.ano)},
        {"Categoria", registro.categoria}
    };

    if(campo == "Registro Completo") {
        registro.exibirRegistro();

        cout << "\nPressione Enter para continuar...";
        cin.get();
        limparConsole();
        return;
    }

    registro.exibirCampo(campo, campos[campo]);

    cout << "\nPressione Enter para continuar...";
    cin.get();
    limparConsole();

}


void exibirMenuPrincipal(Buffer& arquivo) {
    bool primeiraExecucao = true;

    map<int, string> opcoes = {
        {1, "Buscar Registro via Titulo"},
        {2, "Titulo"},
        {3, "Autores"},
        {4, "Ano"},
        {5, "Categoria"},
        {6, "Registro Completo"},
        {7, "Adicionar Novo Registro"},
        {8, "Sair"}
    };


    while (true) {
        cout << "\n====================================" << endl;
        cout << "             Pratica 04               " << endl;
        cout << "====================================" << endl;
        cout << "[1] Consultar Registro via Titulo" << endl;
        cout << "[2] Consultar Titulo via ID" << endl;
        cout << "[3] Consultar Autores via ID" << endl;
        cout << "[4] Consultar Ano via ID" << endl;
        cout << "[5] Consultar Categoria via ID" << endl;
        cout << "[6] Consultar Registro Completo via ID" << endl;
        cout << "[7] Adicionar Novo Registro" << endl;
        cout << "[8] Sair do Sistema" << endl;

        cout << "\nDigite a opcao desejada: ";

        string opcao;
        int opcaoInt;

        if(primeiraExecucao) {
            cin.clear();
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            primeiraExecucao = false;
        } else {
            cin.clear();
            primeiraExecucao = false;
        }
        
        getline(cin, opcao);

        cin.clear();

        if (!(opcao.empty()) && all_of(opcao.begin(), opcao.end(), ::isdigit)) {
            opcaoInt = stoi(opcao);

            if (opcoes.find(opcaoInt) == opcoes.end()) {
                cout << "\nOpcao invalida, tente novamente." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                limparConsole();
                continue;
            }

            if (opcaoInt == 8) {
                return;
            }

            exibirMenuBuscar(arquivo, opcoes[opcaoInt]);
        } else {
            cout << "\nOpcao invalida, tente novamente." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            limparConsole();
        }
    }
}

#endif