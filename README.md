<h1 align="center">Sistema de Busca com Índices Invertidos</h1>


<p align="center">
  <a href=""><img alt="C++" src="https://img.shields.io/badge/Linguagem-C%2B%2B-blue.svg" height="20"/></a>
  <a href=""><img alt="Build System" src="https://img.shields.io/badge/Build-Make-red.svg" height="20"/></a>
  <a href=""><img alt="Data Format" src="https://img.shields.io/badge/Dados-CSV-green.svg" height="20"/></a>
</p>

<p align="center">
  <a target="_blank" rel="noopener noreferrer" href="https://github.com/natansantoz/Atividade-Pratica-5-OSA/blob/main/gif-p5.gif">
      <img src="gif-p5.gif" width="900" style="max-width: 100%;">
  </a>
</p>



Trabalho prático da Prática 5 da disciplina Organização e Sistemas de Arquivos, com o objetivo de implementar um sistema eficiente de busca por palavras-chave em títulos de livros armazenados em arquivos binários, utilizando índices invertidos.

[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)](#table-of-contents)

### Performance

O sistema foi testado com diferentes tamanhos de arquivo para validar sua eficiência:

| Nº Registros | Leitura do CSV e Criação de Índices | Tempo Busca por Palavra | Tempo Busca por Registro Específico |
|-----------|-------------------|-------------------|------------------|
| 1.000     | 16ms              |  1ms              |   1ms            |
| 10.000    | 163ms             |  12ms             |   ~0ms            |
| 50.000    | 869ms             |  47ms             |   ~0ms            |
| 103.064   | 1921ms            |  104ms            |   ~0ms            |


[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)](#table-of-contents)

### Fluxo de Processamento e Busca

  #### 1. Processamento dos Dados
  Leitura sequencial do CSV com conversão simultânea para registros binários de tamanho variável, utilizando descritores de tamanho e delimitadores de campo.

  #### 2. Construção dos Índices
  Durante a escrita dos registros, dois índices são construídos em memória: índice primário (árvore binária mapeando ID → offset) e índice invertido (mapa não ordenado mapeando palavra → conjunto de IDs), com remoção automática de stop words e pontuações.

  #### 3. Persistência dos Índices
  Após o processamento completo do CSV, ambos os índices são persistidos em arquivos: o índice primário é salvo através de percurso em largura da árvore binária, enquanto o índice invertido é serializado com formato `[tamanho]||[palavra]||[ID1,ID2,...]`, utilizando registros de tamanho variável com descritores e delimitadores "||".

  #### 4. Busca por Palavras-chave
  O sistema realiza um pré-processamento na entrada do usuário, consulta o índice invertido para cada palavra (O(1) amortizado), calcula a interseção dos conjuntos de IDs resultantes e recupera os registros através do índice primário.

[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)](#table-of-contents)

#### Requisitos
- Compilador C++ 
- Make

<br>

**Ubuntu/Debian:**
  ```bash
  sudo apt install build-essential
  ```


[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)](#table-of-contents)

#### Instalação

1. **Clone o repositório:**
    ```bash
    git clone https://github.com/natansantoz/Atividade-Pratica-5-OSA.git
    ```

2. **Navegue até o diretório do projeto:**
    ```sh
    cd Atividade-Pratica-5-OSA/Pratica-5/
    ```

### Execução

1. **Compile e execute o projeto:**
   ```sh
   make run
   ```


2. **Limpeza dos arquivos gerados:**
   ```sh
   make clean
   ```



[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)](#table-of-contents)