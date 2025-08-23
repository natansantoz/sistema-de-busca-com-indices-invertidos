#ifndef ARVORE_H
#define ARVORE_H

#include <sstream>
#include <string>
#include <cstring>
#include "Pilha.h"
#include "Fila.h"

#include "Indice.h"


using namespace std;


template <class TIPO>
class ArvoreBinaria;

template <class TIPO>
class Nodo {
	private:
		Nodo<TIPO> *esq, *dir;
		TIPO item;
        
    public:
		// construtor
		Nodo(TIPO t){
			item = t;
			esq = dir = NULL;
		}
		Nodo(TIPO t, Nodo<TIPO>* e, Nodo<TIPO>* d)
		{
		    item = t;
			esq = e;
			dir = d;
		}
    friend class ArvoreBinaria<TIPO>;
};


template <class TIPO>
class ArvoreBinaria
{
    private:
        Nodo<TIPO>* raiz;

        Nodo<TIPO>* Inserir(TIPO valor, Nodo<TIPO>* n);
        TIPO Pesquisar(TIPO valor, Nodo<TIPO>* n);

        Nodo<TIPO>* Remover(TIPO valor, Nodo<TIPO>* n);
        Nodo<TIPO>* RemoverNodo(Nodo<TIPO>* n);
        Nodo<TIPO>* RemoverMax(Nodo<TIPO>* n);

        void Travessia_InOrder(Nodo<TIPO>* n, stringstream &buffer);
        void Travessia_PreOrder(Nodo<TIPO>* n, stringstream &buffer);
        void Travessia_PosOrder(Nodo<TIPO>* n, stringstream &buffer);

        void Destruir(Nodo<TIPO>* n);
        void Print(Nodo<TIPO>* n);

    public:
    int comparacoes = 0;
        ArvoreBinaria(){ raiz = NULL;   }
        void Inserir(TIPO valor);
        TIPO Pesquisar(TIPO valor, bool exibirComparacoes);

        void Remover(TIPO valor);
        ~ArvoreBinaria(){Destruir(raiz);}

        void Print();

        /// algoritmos de travessia
        string Travessia_InOrder();
        string Travessia_PreOrder();
        string Travessia_PosOrder();
        string Travessia_Profundidade();
        string Travessia_Largura();
        string SalvarIndicesTravessiaLargura(const string& caminhoArquivo);
        void carregarIndices(const string& caminhoArquivo);
};





template <class TIPO>
void ArvoreBinaria<TIPO>::Print()
{
    cout<<"{";
    Print(raiz);
    cout<<"}";
}


template <class TIPO>
void ArvoreBinaria<TIPO>::Print(Nodo<TIPO> *n)
{
    if ( n!= NULL )
    {
        Print(n->esq);
        cout<<n->item<<" ";
        Print(n->dir);
    }
}



template <class TIPO>
void ArvoreBinaria<TIPO>::Inserir(TIPO valor)
{
    raiz = Inserir(valor, raiz);
}

template <class TIPO>
Nodo<TIPO>* ArvoreBinaria<TIPO>::Inserir(TIPO valor, Nodo<TIPO>* n)
{
    if ( n == NULL )
        return new Nodo<TIPO>(valor);
    else {
        if ( valor > n->item )
            n->dir = Inserir(valor, n->dir);
        else
            n->esq = Inserir(valor, n->esq);
        return n;
    }
}


template <class TIPO>
void ArvoreBinaria<TIPO>::Remover(TIPO valor)
{
    raiz = Remover(valor, raiz);
}
template <class TIPO>
Nodo<TIPO>* ArvoreBinaria<TIPO>::Remover(TIPO valor, Nodo<TIPO>* n)
{
    Nodo<TIPO>* tmp = NULL;
    if ( n != NULL )
    {
        if ( valor == n->item )
        {
            tmp = n; /// guardar o endere�o do n� a ser removido
            n = RemoverNodo(n);
        }
        else if ( valor > n->item )
            n->dir = Remover(valor, n->dir);
        else
            n->esq = Remover(valor, n->esq);
    }
    if (tmp) delete tmp;
    return n;
}

template <class TIPO>
Nodo<TIPO>* ArvoreBinaria<TIPO>::RemoverNodo(Nodo<TIPO>* n)
{
    if ( n->esq == NULL ) /// verifica se o no em quest�o n�o possui sub-�rvore esquerda
        return n->dir;
    else if ( n->dir == NULL ) /// verifica se o no em quest�o n�o possui sub-�rvore direita
        return n->esq;
    else
        return RemoverMax(n);
}

/// remo��o feita promovendo o MAIOR n� da sub�rvore ESQUERDA
template <class TIPO>
Nodo<TIPO>* ArvoreBinaria<TIPO>::RemoverMax(Nodo<TIPO>* n)
{
    if ( n->esq->dir == NULL )
    {
        n->esq->dir = n->dir;
        return n->esq;
    }
    else
    {
        Nodo<TIPO>* aux1 = n->esq;
        Nodo<TIPO>* aux2 = n->esq->dir;
        while( aux2->dir != NULL )
        {
            aux1 = aux2;
            aux2 = aux2->dir;
        }
        aux1->dir = aux2->esq;
        aux2->esq = n->esq;
        aux2->dir = n->dir;
        return aux2;
    }
}


template <class TIPO>
string ArvoreBinaria<TIPO>::Travessia_InOrder()
{
    stringstream buffer;
    Travessia_InOrder(raiz, buffer);
    return buffer.str();
}
template <class TIPO>
void ArvoreBinaria<TIPO>::Travessia_InOrder(Nodo<TIPO>* n, stringstream &buffer)
{
    if ( n != NULL )
    {
        Travessia_InOrder(n->esq, buffer);
        buffer << n->item << " ";
        Travessia_InOrder(n->dir, buffer);
    }
}

template <class TIPO>
string ArvoreBinaria<TIPO>::Travessia_PreOrder()
{
    stringstream buffer;
    Travessia_PreOrder(raiz, buffer);
    return buffer.str();
}
template <class TIPO>
void ArvoreBinaria<TIPO>::Travessia_PreOrder(Nodo<TIPO>* n, stringstream &buffer)
{
    if ( n != NULL )
    {
        buffer << n->item << " ";
        Travessia_PreOrder(n->esq, buffer);
        Travessia_PreOrder(n->dir, buffer);
    }
}


template <class TIPO>
string ArvoreBinaria<TIPO>::Travessia_PosOrder()
{
    stringstream buffer;
    Travessia_PosOrder(raiz, buffer);
    return buffer.str();
}


template <class TIPO>
void ArvoreBinaria<TIPO>::Travessia_PosOrder(Nodo<TIPO>* n, stringstream &buffer)
{
    if ( n != NULL )
    {
        Travessia_PosOrder(n->esq, buffer);
        Travessia_PosOrder(n->dir, buffer);
        buffer << n->item << " ";
    }
}

template <class TIPO>
string ArvoreBinaria<TIPO>::Travessia_Profundidade()
{
    stringstream buffer;
    Pilha<Nodo<TIPO>*> p;
    p.Empilhar(raiz);
    while( !p.Vazia() )
    {
        Nodo<TIPO>* n = p.Desempilhar();
        if ( n )
        {
            buffer<<n->item<<" ";
            p.Empilhar(n->esq);
            p.Empilhar(n->dir);
        }
    }
    return buffer.str();
}




template <class TIPO>
string ArvoreBinaria<TIPO>::Travessia_Largura()
{
    stringstream buffer;
    Fila<Nodo<TIPO>*> f;
    f.Enfileirar(raiz);
    while( !f.Vazia() )
    {
        Nodo<TIPO>* n = f.Desenfileirar();
        if ( n )
        {
            buffer<<n->item<<" \n";
            f.Enfileirar(n->esq);
            f.Enfileirar(n->dir);
        }
    }
    return buffer.str();
}


template<class TIPO>
void ArvoreBinaria<TIPO>::Destruir(Nodo<TIPO>* n)
{
    if ( n )
    {
        Destruir(n->esq);
        Destruir(n->dir);
        delete(n);
    }
}

/** impress�o IN ORDER sem recurs�o
template<class T>
void BST<T>::iterativeInorder() {
  Stack<BSTNode<T>*> travStack;
  BSTNode<T> *p = root;
  while (p != 0) {
     while (p != 0) {      // empilhar filho da direita (se houver)
       if (p->right)       // e o pr�prio n� quando for
           travStack.push(p->right); // para a direita;
       travStack.push(p);
       p = p->left;
      }
     p = travStack.pop();        // pop em n� sem filho esquerdo
     while (!travStack.empty() && p->right == 0) {
                                 // visita-lo e a todos os n�s
         visit(p);               // sem filho direito;
         p = travStack.pop();
       }
       visit(p);                 // visitar tamb�m o primeiro n� com
       if (!travStack.empty())   // um filho direito (se houver);
             p = travStack.pop();
        else p = 0;
    }
}
*/


template <class TIPO>
void ArvoreBinaria<TIPO>::carregarIndices(const string& caminhoArquivo)
{

    static_assert(std::is_same<TIPO, Indice>::value, "O tipo TIPO deve ser Indice.");

    ifstream arquivoBinarioRegistros(caminhoArquivo, ios::binary); 

    if (!arquivoBinarioRegistros.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo em carregarIndices.");
    }
    
    while (arquivoBinarioRegistros.peek() != EOF) {

        Indice indice;

        indice.unpack(arquivoBinarioRegistros);
        
        this->Inserir(indice);
    }

    arquivoBinarioRegistros.close();
}


template <class TIPO>
string ArvoreBinaria<TIPO>::SalvarIndicesTravessiaLargura(const string& caminhoArquivo)
{

    static_assert(std::is_same<TIPO, Indice>::value, "O tipo TIPO deve ser Indice.");

    ofstream arquivo(caminhoArquivo, ios::binary | ios::app); 

    if (!arquivo.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo em SalvarIndicesTravessiaLargura.");
    }
    

    stringstream stringVisualizacaoPercorrimento;
    Fila<Nodo<TIPO>*> f;
    f.Enfileirar(raiz);
    while( !f.Vazia() )
    {
        Nodo<TIPO>* n = f.Desenfileirar();
        if ( n )
        {
            Indice indice = n->item;    

            string packed = indice.pack();

            arquivo.write(reinterpret_cast<const char*>(packed.c_str()), sizeof(long long) + sizeof(streampos));

            stringVisualizacaoPercorrimento<<n->item<<" ";
            f.Enfileirar(n->esq);
            f.Enfileirar(n->dir);
        }
    }

    arquivo.close();
    return stringVisualizacaoPercorrimento.str();
}


template <class TIPO>
TIPO ArvoreBinaria<TIPO>::Pesquisar(TIPO valor, bool exibirComparacoes)
{
    comparacoes = 0;
    TIPO no = Pesquisar(valor, raiz);
    if (exibirComparacoes) {
        cout << "Numero de nos visitados: " << comparacoes << endl;
    }
    return no;
}


template <class TIPO>
TIPO ArvoreBinaria<TIPO>::Pesquisar(TIPO valor, Nodo<TIPO>* n)
{
    comparacoes++;
    if ( n == NULL )
        return TIPO();
    
    else if ( n->item == valor )
        return n->item;
    else if ( valor > n->item )
        return Pesquisar(valor, n->dir);
    else
        return Pesquisar(valor, n->esq);
}



// template <class TIPO>
// bool ArvoreBinaria<TIPO>::Pesquisar(TIPO valor)
// {
//     return Pesquisar(valor, raiz);
// }
// template <class TIPO>
// bool ArvoreBinaria<TIPO>::Pesquisar(TIPO valor, Nodo<TIPO>* n)
// {
//     if ( n == NULL )
//         return false;
//     else if ( n->item == valor )
//         return true;
//     else if ( valor > n->item )
//         return Pesquisar(valor, n->dir);
//     else
//         return Pesquisar(valor, n->esq);
// }


#endif // ARVORE_H
