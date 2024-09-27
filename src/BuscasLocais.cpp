#include "BuscasLocais.h"

float inversaoBit(Instancia& inst, Solucao& sol, int i) {
    // Se o bit eh 1, irei inverter para 0 e, portanto, subtrair o valor
    // Se o bit eh 0, irei inverter para 1 e, portanto, somar o valor
    if (sol.at(i)==1) {
        sol.at(i) = 0;
        return -inst.itens.at(i).valor;
    }
    else {
        sol.at(i) = 1;
        return inst.itens.at(i).valor;
    }
}

DescidaInversaoDe2BitsFI::DescidaInversaoDe2BitsFI() {}

DescidaInversaoDe2BitsFI::~DescidaInversaoDe2BitsFI() {}

Solucao DescidaInversaoDe2BitsFI::aprimorarSolucao(Instancia& inst, Solucao& sol) {
    Solucao melhorSol = sol;
    float melhorFO = avaliaFO(inst, sol);

    Solucao solAtual = melhorSol;
    float atualFO = melhorFO;
    float antigaFO;
    
    bool melhorou;
    
    do {
        melhorou = false;
        for (int i=0; i<inst.nItens; i++) {
            for (int j=i+1; j<inst.nItens; j++) {

                // Calcula o delta de valores (valor, peso)
                float delta1 = inversaoBit(inst, solAtual, i);
                float delta2 = inversaoBit(inst, solAtual, j);

                // Calcula nova FO
                antigaFO = atualFO;
                atualFO = atualFO + delta1 + delta2 + (avaliaValidade(inst, solAtual) ? 0 : -inst.somaDosValores);

                // Se melhorou, reinicia-se a busca com a nova solução
                if (atualFO > melhorFO) {
                    cout << "Melhora encontrada na DescidaInversaoDe2BitsFI. FO: " << melhorFO << " -> " << atualFO << endl;
                    melhorFO = atualFO;
                    melhorSol = solAtual;
                    melhorou = true;
                    break;
                }
                else {
                    // Desfaz mudança
                    inversaoBit(inst, solAtual, i);
                    inversaoBit(inst, solAtual, j);
                    atualFO = antigaFO;
                }
            }
            if (melhorou)
                break;
        }
    } while (melhorou);

    return melhorSol;
}

DescidaInversaoDe2BitsBI::DescidaInversaoDe2BitsBI() {}

DescidaInversaoDe2BitsBI::~DescidaInversaoDe2BitsBI() {}

Solucao DescidaInversaoDe2BitsBI::aprimorarSolucao(Instancia& inst, Solucao& sol) {
    Solucao melhorSol = sol;
    float melhorFO = avaliaFO(inst, sol);

    Solucao solAtual = melhorSol;
    float atualFO = melhorFO;
    float antigaFO;
    
    bool melhorou;
    
    do {
        melhorou = false;
        for (int i=0; i<inst.nItens; i++) {
            for (int j=i+1; j<inst.nItens; j++) {

                // Calcula o delta de valores (valor, peso)
                float delta1 = inversaoBit(inst, solAtual, i);
                float delta2 = inversaoBit(inst, solAtual, j);

                // Calcula nova FO
                antigaFO = atualFO;
                atualFO = atualFO + delta1 + delta2 + (avaliaValidade(inst, solAtual) ? 0 : -inst.somaDosValores);

                // Se melhorou, reinicia-se a busca com a nova solução
                if (atualFO > melhorFO) {
                    cout << "Melhora encontrada na DescidaInversaoDe2BitsBI. FO: " << melhorFO << " -> " << atualFO << endl;
                    melhorFO = atualFO;
                    melhorSol = solAtual;
                    melhorou = true;
                }
                else {
                    // Desfaz mudança
                    inversaoBit(inst, solAtual, i);
                    inversaoBit(inst, solAtual, j);
                    atualFO = antigaFO;
                }
            }
        }
    } while (melhorou);

    return melhorSol;
}

DescidaInversaoDe2aNBitsFI::DescidaInversaoDe2aNBitsFI() {
    this->nBits = 10;
    this->iterMax = 200000;
}

DescidaInversaoDe2aNBitsFI::DescidaInversaoDe2aNBitsFI(int n, int iterMax) {
    this->nBits = n;
    this->iterMax = iterMax;
}

DescidaInversaoDe2aNBitsFI::~DescidaInversaoDe2aNBitsFI() {}

Solucao DescidaInversaoDe2aNBitsFI::aprimorarSolucao(Instancia& inst, Solucao& sol) {
    Solucao melhorSol = sol;
    float melhorFO = avaliaFO(inst, sol);

    Solucao solAtual = melhorSol;
    float atualFO = melhorFO;
    float antigaFO;
    
    int iter;
    bool melhorou;
    
    do {
        iter = 0;
        melhorou = false;
        while (iter < iterMax) {
            iter++;

            int nIteracao = 2+rand()%(this->nBits-1); // Valor de n nesta iteracao
            vector<pair<int, float>> delta(nIteracao); // Formato: [(posicao da inversao, deltaDaInversao)]

            // Realiza as i \in [2, n] inversoes
            for (int z=0; z<nIteracao; z++) {
                int i = rand() % inst.nItens;
                delta.at(z).first = i;
                delta.at(z).second = inversaoBit(inst, solAtual, i);
            }

            // Calcula nova FO
            antigaFO = atualFO;
            for (int i=0; i<delta.size(); i++) 
                atualFO+=delta.at(i).second;
            atualFO += avaliaValidade(inst, solAtual) ? 0 : -inst.somaDosValores;

            // Se melhorou, reinicia-se a busca com a nova solução
            if (atualFO > melhorFO) {
                cout << "Melhora encontrada na DescidaInversaoDe2aNBitsFI. FO: " << melhorFO << " -> " << atualFO << endl;
                melhorFO = atualFO;
                melhorSol = solAtual;
                melhorou = true;
                break;
            }
            // Se não melhorou, Desfaz mudança
            else {
                for (int z=0; z<nIteracao; z++)
                    inversaoBit(inst, solAtual, delta.at(z).first);
                atualFO = antigaFO;
            }
        }
    } while (melhorou);

    return melhorSol;
}


/*
pair<Solucao, float> movimentoInsercaoAleatoria(Instancia& inst, Solucao& solBase, float fo) {
    Solucao sol = solBase;

    cout << "FO INICIAL = " << fo;

    // Sorteando um item fora do CI para ser adicionado
    int indexItemSorteado;
    bool invalido;
    do {
        invalido = false;
        indexItemSorteado = rand()%sol.size();
        if (sol.at(indexItemSorteado))   // O item nao pode ser um item ja adicionado
            invalido = true;
    } while (invalido);

    // Adicionando o item a solucao
    sol.at(indexItemSorteado) = 1;
    fo+=inst.itens.at(indexItemSorteado).valor;

    // Removendo da solucao os itens que apresentam conflitos
    int counter = 0, counter2 = 0;
    for (int i=0; i<sol.size(); i++) {
        if (sol.at(i))
            counter2++;
        if (sol.at(i) && inst.restricoes[indexItemSorteado][i]) {
            sol.at(i) = 0;
            fo -= inst.itens.at(i).valor;
            counter++; 
        }
    }
    
    cout << " Tinha " << counter2 << " items!";
    cout << " Retirou " << counter << " items!";
    
            
    // Removendo o item com menor valor/peso ate que a capacidade maxima seja respeitada
    float peso = avaliaPeso(inst, sol);
    vector<Item> itensNaSol;
    if (peso > inst.pesoMax) {  // Caso nao for entrar no while, nao tem porque criar este vetor
        for (int i=0; i<sol.size(); i++)
            if (sol.at(i))
                itensNaSol.push_back(inst.itens.at(i));

        sort(itensNaSol.begin(), itensNaSol.end(), CompValorPorPeso);
    }

    cout << "Tem " << itensNaSol.size() << " itens na solucao" << endl;

    while (peso > inst.pesoMax) {
        Item itemARetirar = itensNaSol.back();
        itensNaSol.pop_back();
        
        sol.at(itemARetirar.index) = 0;
        fo -= inst.itens.at(itemARetirar.index).valor;
        peso -= itemARetirar.peso;
    }
    cout << " -- FO final = " << fo << endl;
    return pair<Solucao, float>(sol, fo);
}
*/

DescidaDestroyAndRepair::DescidaDestroyAndRepair() {
    this->maxIter = 200;
    this->percBase = 0.2;
    this->percDestruido = this->percBase;
    this->alpha = 0.3;
}

DescidaDestroyAndRepair::DescidaDestroyAndRepair(int maxIter, float percBase, float alpha) {
    this->maxIter = maxIter;
    this->percBase = percBase;
    this->percDestruido = this->percBase;
    this->alpha = alpha;
}

DescidaDestroyAndRepair::~DescidaDestroyAndRepair() {}

Solucao DescidaDestroyAndRepair::recomposicaoGulosa(Instancia& inst, Solucao& CI) {
    Solucao sol = geraMochilaVazia(inst.nItens);

    vector<Item> solAux; // Auxiliar para melhorar a complexidade do processo abaixo

    // Quem esta no CI parcial, continuara no final
    for (int i=0; i<CI.size(); i++) 
        if (CI.at(i)) {
            sol.at(i) = 1;
            solAux.push_back(inst.itens.at(i));
        }
    
    // Gerando a LC. 
    vector<Item> listaCandidatos;

    // Adicionarei todos itens MENOS os que ja estao no CI &
    // MENOS os que tem conflitos com quem ja esta no CI.
    for (int i=0; i<inst.itens.size(); i++) {
        int indexAnalisado = inst.itens.at(i).index;
        if (!sol.at(indexAnalisado)) { // Se o item a ser adicionado ainda nao esta no CI...
            
            // Se o item a ser adicionado nao tem nenhuma restricao com os itens que ja estao no CI...
            bool flag = true;
            for (int j=0; j<solAux.size(); j++) {
                if (inst.restricoes[indexAnalisado][solAux.at(j).index]) {
                    flag = false;
                    break;
                }
            }

            // Eu o adiciono a lista de candidatos
            if (flag) 
                listaCandidatos.push_back(inst.itens.at(i));
        }
    }

    // Ordenar de acordo com algum criterio (valor nesse caso)
    sort(listaCandidatos.begin(), listaCandidatos.end(), CompValor);

    while (!listaCandidatos.empty()) {
        int indexSelecionado = listaCandidatos.back().index; // Candidato de maior metrica
        sol.at(indexSelecionado) = 1; // Adiciona o candidato de maior metrica
        listaCandidatos.pop_back(); // Remove ele da lista de candidatos

        // Verifica quais candidatos sao incompativeis com o recentemente adicionado
        vector<int> indexesRemover = vector<int>();
        for (int i=0; i<listaCandidatos.size(); i++) 
            if (inst.restricoes[indexSelecionado][listaCandidatos.at(i).index]) 
                indexesRemover.push_back(i);
        
        // Remove tais candidatos
        for (auto it=indexesRemover.rbegin(); it<indexesRemover.rend(); it++) // Remover de tras pra frente para nao baguncar os indices
            listaCandidatos.erase(listaCandidatos.begin()+(*it));
    }

    return sol;
}

Solucao DescidaDestroyAndRepair::recomposicaoAleatoria(Instancia& inst, Solucao& CI) {
    Solucao sol = geraMochilaVazia(inst.nItens);

    vector<Item> solAux; // Auxiliar para melhorar a complexidade do processo abaixo

    // Quem esta no CI parcial, continuara no final
    for (int i=0; i<CI.size(); i++) 
        if (CI.at(i)) {
            sol.at(i) = 1;
            solAux.push_back(inst.itens.at(i));
        }
    
    // Gerando a LC. 
    vector<Item> listaCandidatos;

    // Adicionarei todos itens MENOS os que ja estao no CI &
    // MENOS os que tem conflitos com quem ja esta no CI.
    for (int i=0; i<inst.itens.size(); i++) {
        int indexAnalisado = inst.itens.at(i).index;
        if (!sol.at(indexAnalisado)) { // Se o item a ser adicionado ainda nao esta no CI...
            
            // Se o item a ser adicionado nao tem nenhuma restricao com os itens que ja estao no CI...
            bool flag = true;
            for (int j=0; j<solAux.size(); j++) {
                if (inst.restricoes[indexAnalisado][solAux.at(j).index]) {
                    flag = false;
                    break;
                }
            }

            // Eu o adiciono a lista de candidatos
            if (flag) 
                listaCandidatos.push_back(inst.itens.at(i));
        }
    }

    // Ordenar de acordo com algum criterio (valor nesse caso)
    sort(listaCandidatos.begin(), listaCandidatos.end(), CompValor);

    while (!listaCandidatos.empty()) {
        int randValor = rand()%listaCandidatos.size();
        int indexSelecionado = listaCandidatos.at(randValor).index; // Candidato escolhido
        sol.at(indexSelecionado) = 1; // Adiciona o candidato a solucao
        listaCandidatos.erase(listaCandidatos.begin()+randValor); // Remove ele da lista de candidatos

        // Verifica quais candidatos sao incompativeis com o recentemente adicionado
        vector<int> indexesRemover = vector<int>();
        for (int i=0; i<listaCandidatos.size(); i++) 
            if (inst.restricoes[indexSelecionado][listaCandidatos.at(i).index]) 
                indexesRemover.push_back(i);
        
        // Remove tais candidatos
        for (auto it=indexesRemover.rbegin(); it<indexesRemover.rend(); it++) // Remover de tras pra frente para nao baguncar os indices
            listaCandidatos.erase(listaCandidatos.begin()+(*it));
    }

    return sol;
}

Solucao DescidaDestroyAndRepair::aprimorarSolucao(Instancia& inst, Solucao& sol, Solucao& CI) {
    // Inicializacoes
    Solucao melhorSol = sol;
    Solucao melhorCI = CI;
    Solucao solAtual = melhorSol;
    Solucao atualCI = melhorCI;
    MetodoExato* metodoCPLEX = new CPLEXMochilaBasico(inst.nItens, 0, false);
    float melhorFo = avaliaFO(inst, sol);
    float foAtual = melhorFo;

    int iter = 0;
    while (iter < this->maxIter) { // Enquanto nao atingir o maximo de iteracoes sem melhora...
        iter++;

        atualCI = melhorCI; // A estrutura de vizinhanca eh gerada sobre o melhor CI

        // Contabilizando quantos itens estao no CI
        int itensNoCI = 0;
        for (int i=0; i<atualCI.size(); i++)
            itensNoCI += atualCI.at(i);
        
        // Gerando o vizinho
        int alteracoes = 0;
        for (int i=0; alteracoes < itensNoCI*this->percDestruido; i++)
            if (atualCI.at(i)==1) { // Posso tentar adicionar um && rand()%2 == 0 para as eliminacoes serem mais aleatorias...
                atualCI.at(i) = 0;
                alteracoes++;
            }
        
        // Sorteando metodo de recomposicao
        if (rand()%2==0)
            atualCI = this->recomposicaoGulosa(inst, atualCI);
        else
            atualCI = this->recomposicaoAleatoria(inst, atualCI);

        // Gera solucao com base no novo CI

        int nItensSubproblema = 0;
        for (auto v : atualCI)
            nItensSubproblema += v;
        
        // Itens do subproblema
        vector<Item> itensSubproblema;
        for (int i=0; i<atualCI.size(); i++)
            if (atualCI.at(i))
                itensSubproblema.push_back(inst.itens.at(i));

        Instancia instSubproblema = Instancia(nItensSubproblema, inst.pesoMax, itensSubproblema); // Construtor para subproblema CPLEX
        solAtual = metodoCPLEX->gerarSolucaoOtima(instSubproblema);
        foAtual = avaliaFO(inst, solAtual);

        //solAtual = geraMochilaVazia(inst.nItens);
        //foAtual = 0;
        //Solucao saux = geraMochilaVazia(inst.nItens);
        //int iterBack = 0;
        //bool continua = true;
        //solAtual = backtrackComIteracaoMaxima(inst, solAtual, &foAtual, saux, itensSubproblema, 0, 0, &iterBack, 10000, &continua);
        //foAtual = avaliaFO(inst, solAtual);

        //cout << "HAHAHAHAHA";
        //Instancia instSubproblema = Instancia(nItensSubproblema, inst.pesoMax, itensSubproblema);
        //Solucao solAux = InsercaoGulosa().gerarSolucao(instSubproblema);
        //cout << "SOL AUX SIZE = " << solAux.size() << endl;
        //solAtual = geraMochilaVazia(inst.nItens);
        //for (int i=0; i<instSubproblema.nItens; i++) 
        //    if (solAux.at(i))
        //        solAtual.at(instSubproblema.itens.at(i).index) = 1;
        //foAtual = avaliaFO(inst, solAtual);

        if (foAtual > melhorFo) {
            //cout << "Melhora DestroyAndRepair! Fo: " << melhorFo << " --> " << foAtual << endl;
            melhorFo = foAtual;
            melhorSol = solAtual;
            iter = 0;
            this->percDestruido = this->percBase;
        }
        else {
            // percentual destruido sobe ate no maximo 0.8
            this->percDestruido = this->percDestruido * (1+this->alpha) > 0.8 ? 0.8 : this->percDestruido * (1+this->alpha);
        }
    }

    return melhorSol;
}