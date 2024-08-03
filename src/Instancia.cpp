#include "Instancia.h"

// Instanciacao default
Instancia::Instancia() {
    ifstream arq(CAMINHO_PADRAO);
    if(arq.is_open())
    {
        // Numero de itens
        arq >> this->nItens;

        // Numero de restricoes
        arq >> this->nRest;

        // Peso maximo
        arq >> this->pesoMax;
        
        // Criando o vetor de itens
        Item aux;
        aux.nRest = 0;
        for (int i=0; i<this->nItens; i++) {
            aux.index = i;
            this->itens.push_back(aux);
        }
        
        // Lendo os valores
        for (int i=0; i<this->nItens; i++)
            arq >> this->itens.at(i).valor;
        
        // Lendo os pesos
        for (int i=0; i<this->nItens; i++)
            arq >> this->itens.at(i).peso;
        
        // Alocando a matriz de restricoes
        this->restricoes = new int*[this->nRest];
        for (int i=0; i<this->nRest; i++)
            this->restricoes[i] = new int[this->nRest];
        
        // Inicializando com false (isso eh, sem restricoes)
        for (int i=0; i<this->nRest; i++)
            for (int j=0; i<this->nRest; i++)
                this->restricoes[i][j] = 0;

        // Lendo as restricoes
        for(int i=0; i<this->nRest; i++)
        {
            int a, b;
            arq >> a;
            arq >> b;

            // Corrigindo para index
            a--;
            b--;

            this->restricoes[a][b] = 1;
            this->restricoes[b][a] = 1;
            this->itens[a].nRest++;
            this->itens[b].nRest++;
        }
    }
    else {
        cout << "Nao foi possivel abrir o arquivo " << CAMINHO_PADRAO << endl << "Encerrando com erro...";
        exit(1);
    }
}

Instancia::Instancia(string caminhoArquivo) {
    ifstream arq(caminhoArquivo);
    if(arq.is_open())
    {
        // Numero de itens
        arq >> this->nItens;

        // Numero de restricoes
        arq >> this->nRest;

        // Peso maximo
        arq >> this->pesoMax;

        // Criando o vetor de itens
        Item aux;
        aux.nRest = 0;
        for (int i=0; i<this->nItens; i++) {
            aux.index = i;
            this->itens.push_back(aux);
        }
        
        // Lendo os valores
        for (int i=0; i<this->nItens; i++)
            arq >> this->itens.at(i).valor;
        
        // Lendo os pesos
        for (int i=0; i<this->nItens; i++)
            arq >> this->itens.at(i).peso;
        
        // Alocando a matriz de restricoes
        this->restricoes = new int*[this->nItens];
        for (int i=0; i<this->nItens; i++)
            this->restricoes[i] = new int[this->nItens];
        
        // Inicializando com false (isso eh, sem restricoes)
        for (int i=0; i<this->nItens; i++)
            for (int j=0; i<this->nItens; j++)
                this->restricoes[i][j] = 0;

        // Lendo as restricoes
        for(int i=0; i<this->nRest; i++)
        {
            int a, b;
            arq >> a;
            arq >> b;

            // Corrigindo para index
            a--;
            b--;

            this->restricoes[a][b] = 1;
            this->restricoes[b][a] = 1;
            this->itens[a].nRest++;
            this->itens[b].nRest++;
        }
    }
    else {
        cout << "Nao foi possivel abrir o arquivo " << caminhoArquivo << endl << "Encerrando com erro...";
        exit(1);
    }
}

// Desaloca a matriz de restricoes
Instancia::~Instancia() {
    for (int i=0; i<this->nItens; i++)
        delete[] this->restricoes[i];
    delete this->restricoes;
}

void Instancia::printInstancia(bool printarItens) {
    cout << "N. Itens: " << this->nItens << endl << "N. Restricoes: " << this->nRest << endl << "Peso Maximo: " << this->pesoMax << endl << endl << "-------------" << endl << endl;
    cout << "Matriz de Restricoes: " << endl << endl;

    for (int i=0; i<this->nItens; i++) {
        for (int j=0; j<this->nItens; j++) {
            cout << this->restricoes[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << endl << "-------------" << endl;

    if (printarItens) {
        cout << endl << "Todos Itens:" << endl;
        for (int i=0; i<this->nItens; i++)
            cout << this->itens.at(i) << endl;
        cout << endl << "-------------" << endl;
    }
}