#include <string>

#include "Buffer.h"
#include "Menu.h"


using namespace std;


int main() {
    limparConsole();

    Buffer buffer(
        "booksDataset.csv",
        "booksDataset.dat",
        "indicesBooksDataset.dat",
        "indicesInvertidosBooksDataset.dat",
        "stopwords_en.txt",
        "pontuacoes.txt"
    );

    carregarIndices(buffer);

    exibirMenuPrincipal(buffer);

    return 0;
}

