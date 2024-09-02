#include "HeuristicasConstrutivas.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// MÉTODOS EXATOS //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Solucao backtrack(Instancia& inst, Solucao& melhorSol, float* melhorFo, Solucao& solAtual, vector<Item>& itens, int i, float pesoAtual) {
    // Caso de parada: cheguei ao último item ou excedi o peso máximo
    if (i >= itens.size()) {
        float atualFo = avaliaFO(inst, solAtual);
        if (atualFo > (*melhorFo)) {
            melhorSol = solAtual;
            *melhorFo = atualFo;
        }
        return melhorSol;
    }

    // Incluir o item atual se não exceder o peso máximo
    if (pesoAtual + itens.at(i).peso <= inst.pesoMax) {
        solAtual.at(itens.at(i).index) = 1;
        if (avaliaValidade(inst, solAtual)) {
            melhorSol = backtrack(inst, melhorSol, melhorFo, solAtual, itens, i+1, pesoAtual+itens.at(i).peso);
        }
        solAtual.at(itens.at(i).index) = 0;  // Reverter a adição do item atual
    }

    // Não incluir o item atual
    melhorSol = backtrack(inst, melhorSol, melhorFo, solAtual, itens, i + 1, pesoAtual);

    return melhorSol;
}

/*
Uma modificação do backtrack: se não encontrar nenhuma melhora em iterMax solucoes finais,
o algoritmo encerra sua execucao. Lembre-se: diferentemente do backtrack convencional, este
NÃO GARANTE o ótimo global (da decomposicao gulosa realizada previamente, nao do problema como um todo).
*/
Solucao backtrackComIteracaoMaxima(Instancia& inst, Solucao& melhorSol, float* melhorFo, Solucao& solAtual, vector<Item>& itens, int i, float pesoAtual, int* iter, int iterMax, bool* continua) {
    if (*continua) {
        // Caso de parada: cheguei ao último item ou excedi o peso máximo
        if (i >= itens.size()) {
            float atualFo = avaliaFO(inst, solAtual);
            (*iter)++;
            if (atualFo > (*melhorFo)) {
                melhorSol = solAtual;
                *melhorFo = atualFo;
                *iter = 0;
            }
            if (*iter > iterMax)
                *continua = false;
            return melhorSol;
        }

        // Incluir o item atual se não exceder o peso máximo
        if (pesoAtual + itens.at(i).peso <= inst.pesoMax) {
            solAtual.at(itens.at(i).index) = 1;
            if (avaliaValidade(inst, solAtual)) {
                melhorSol = backtrackComIteracaoMaxima(inst, melhorSol, melhorFo, solAtual, itens, i+1, pesoAtual+itens.at(i).peso, iter, iterMax, continua);
            }
            solAtual.at(itens.at(i).index) = 0;  // Reverter a adição do item atual
        }

        // Não incluir o item atual
        melhorSol = backtrackComIteracaoMaxima(inst, melhorSol, melhorFo, solAtual, itens, i + 1, pesoAtual, iter, iterMax, continua);
    }

    return melhorSol;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Decomposicao Gulosa //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
    Artigo referencia: 
        Matheuristicas para o problema da mochila com restricao de conflitos
        Adriano Rodrigues Alves, Edna A. Hoshino
    
    Metodos de decomposicao (3.1.) --> DecomposicaoGulosa
        Primeiro resolve o problema das restrições (em essencia um conjunto independente) através de uma heuristica gulosa.
        Depois utiliza o método exato sobre o subproblema gerado. Irei testar com heuristica nesse passo tambem!
*/

DecomposicaoGulosa::DecomposicaoGulosa() {}
DecomposicaoGulosa::~DecomposicaoGulosa() {}

// Algoritmo 1 do paper
// OBS.: MODULARIZAR DEPOIS PARA SER GULOSO COM CRITERIOS DIFERENTES
// pExtra[0] != 0 -> valor
// pExtra[0] == 0 -> n. de restricoes
Solucao DecomposicaoGulosa::decomposicaoGulosa(Instancia& inst, ParametrosExtra pExtra) { 
    Solucao sol = geraMochilaVazia(inst.nItens);

    // Candidatos ordenados por seu valor
    vector<Item> listaCandidatos = inst.itens;

    // Ordenar de acordo com o parametro passado
    if (pExtra[0])
        sort(listaCandidatos.begin(), listaCandidatos.end(), CompValor);
    else
        sort(listaCandidatos.begin(), listaCandidatos.end(), CompRest);

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

    for (auto i : sol)
        this->conjuntoIndependente.push_back(i);

    return sol;
}

Solucao DecomposicaoGulosa::gerarSolucao(Instancia& inst, ParametrosExtra pExtra) {
    Solucao solReferencia = this->decomposicaoGulosa(inst, pExtra);

    // Vetor apenas com o subconjunto de itens nao conflitantes entre si
    vector<Item> itensValidos;
    for (int i=0; i<solReferencia.size(); i++)
        if (solReferencia.at(i))
            itensValidos.push_back(inst.itens.at(i));

    // Solucao do subproblema com metodo exato
    Solucao melhorSol = geraMochilaVazia(inst.nItens);
    Solucao sol = geraMochilaVazia(inst.nItens);

    // 3 Opcoes --> backtrack, backtrack com itermax e CPLEX

    // Utilizando backtrack convencional
    //float melhorFo = 0;
    //melhorSol = backtrack(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0);

    // Utilizando backtrack com iterMax
    // float melhorFo = 0;
    //int iter = 0;
    //bool continua = true;
    //melhorSol = backtrackComIteracaoMaxima(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0, &iter, 10000, &continua);

    // Utilizando CPLEX
    CPLEXMochilaBasico metodoCPLEX = CPLEXMochilaBasico(inst.nItens, 0, false);

    // N de itens no subproblema
    int nItensSubproblema = 0;
    for (auto v : solReferencia)
        nItensSubproblema += v;
    
    // Itens do subproblema
    vector<Item> itensSubproblema;
    for (int i=0; i<solReferencia.size(); i++)
        if (solReferencia.at(i))
            itensSubproblema.push_back(inst.itens.at(i));

    Instancia instSubproblema = Instancia(nItensSubproblema, inst.pesoMax, itensSubproblema); // Construtor para subproblema CPLEX

    melhorSol = metodoCPLEX.gerarSolucaoOtima(instSubproblema);

    return melhorSol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Decomposicao Aleatoria////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    Artigo referencia: 
        Matheuristicas para o problema da mochila com restricao de conflitos
        Adriano Rodrigues Alves, Edna A. Hoshino
    
    Metodos de decomposicao (3.1.) --> DecomposicaoAleatoria
        Primeiro resolve o problema das restrições (em essencia um conjunto independente) através de uma heuristica randomica.
        Depois utiliza o método exato sobre o subproblema gerado. Irei testar com heuristica nesse passo tambem!
*/

DecomposicaoAleatoria::DecomposicaoAleatoria() {}
DecomposicaoAleatoria::~DecomposicaoAleatoria() {}

// Algoritmo 2 do paper
Solucao DecomposicaoAleatoria::decomposicaoAleatoria(Instancia& inst) {
    Solucao sol = geraMochilaVazia(inst.nItens);

    // Candidatos
    vector<Item> listaCandidatos = inst.itens;
        
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

    for (auto i : sol)
        this->conjuntoIndependente.push_back(i);
        
    return sol;
}

Solucao DecomposicaoAleatoria::gerarSolucao(Instancia& inst, ParametrosExtra pExtra) { // pExtra nao eh utilizado
    Solucao solReferencia = this->decomposicaoAleatoria(inst);

    // Vetor apenas com o subconjunto de itens nao conflitantes entre si
    vector<Item> itensValidos;
    for (int i=0; i<solReferencia.size(); i++)
        if (solReferencia.at(i))
            itensValidos.push_back(inst.itens.at(i));

    // Solucao do subproblema com metodo exato
    Solucao melhorSol = geraMochilaVazia(inst.nItens);
    Solucao sol = geraMochilaVazia(inst.nItens);

    // 3 Opcoes --> backtrack, backtrack com iterMax e CPLEX

    // Utilizando backtrack convencional
    //float melhorFo = 0;
    //melhorSol = backtrack(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0);

    // Utilizando backtrack com iterMax
    //float melhorFo = 0;
    //int iter = 0;
    //bool continua = true;
    //melhorSol = backtrackComIteracaoMaxima(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0, &iter, 10000, &continua);

    // Utilizando CPLEX
    CPLEXMochilaBasico metodoCPLEX = CPLEXMochilaBasico(inst.nItens, 0, false);

    // N de itens no subproblema
    int nItensSubproblema = 0;
    for (auto v : solReferencia)
        nItensSubproblema += v;
    
    // Itens do subproblema
    vector<Item> itensSubproblema;
    for (int i=0; i<solReferencia.size(); i++)
        if (solReferencia.at(i))
            itensSubproblema.push_back(inst.itens.at(i));

    Instancia instSubproblema = Instancia(nItensSubproblema, inst.pesoMax, itensSubproblema); // Construtor para subproblema CPLEX

    melhorSol = metodoCPLEX.gerarSolucaoOtima(instSubproblema);

    return melhorSol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Insercao Gulosa///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Pseudocódigo:

Enquanto LC != {}
    1. Seleciono item mais valioso da LC e o removo da LC
        1.1. Se couber na mochila, o adiciono, e removo itens conflitantes com o item adicionado da LC
*/


InsercaoGulosa::InsercaoGulosa() {}
InsercaoGulosa::~InsercaoGulosa() {}

Solucao InsercaoGulosa::gerarSolucao(Instancia& inst, ParametrosExtra pExtra) {
    Solucao sol = geraMochilaVazia(inst.nItens);

    // Candidatos ordenados por seu valor
    vector<Item> listaCandidatos = inst.itens;

    // Ordenar pelo valor
    sort(listaCandidatos.begin(), listaCandidatos.end(), CompValor);

    float pesoAtual = 0;

    while (!listaCandidatos.empty()) {
        Item itemSelecionado = listaCandidatos.back(); // Como esta ordenada a LC, este sempre sera o candidato de maior valor
        listaCandidatos.pop_back();

        // Verifica se o item mais valioso cabe na mochila
        if (pesoAtual+itemSelecionado.peso <= inst.pesoMax) {
            pesoAtual+=itemSelecionado.peso;
            sol.at(itemSelecionado.index) = 1; // Se couber, o adiciona

            // Remove os itens conflitantes da LC (OBS.: De tras pra frente pra nao ter problemas com indices)
            for (int i=listaCandidatos.size()-1; i>=0; i--) 
                if (inst.restricoes[itemSelecionado.index][listaCandidatos.at(i).index])
                    listaCandidatos.erase(listaCandidatos.begin()+i);
        }
    }

    return sol;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////Insercao Aleatoria////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Pseudocódigo:

Enquanto LC != {}
    1. Seleciono item aleatorio da LC e o removo da LC
        1.1. Se couber na mochila, o adiciono, e removo itens conflitantes com o item adicionado da LC
*/

InsercaoAleatoria::InsercaoAleatoria() {}
InsercaoAleatoria::~InsercaoAleatoria() {}

Solucao InsercaoAleatoria::gerarSolucao(Instancia& inst, ParametrosExtra pExtra) {
    Solucao sol = geraMochilaVazia(inst.nItens);

    // Candidatos
    vector<Item> listaCandidatos = inst.itens;

    float pesoAtual = 0;
        
    while (!listaCandidatos.empty()) {
        int randValor = rand()%listaCandidatos.size();
        Item itemSelecionado = listaCandidatos.at(randValor); // Candidato escolhido
        listaCandidatos.erase(listaCandidatos.begin()+randValor); // Remove ele da lista de candidatos

        // Verifica se o item mais valioso cabe na mochila
        if (pesoAtual+itemSelecionado.peso <= inst.pesoMax) {
            pesoAtual+=itemSelecionado.peso;
            sol.at(itemSelecionado.index) = 1; // Se couber, o adiciona

            // Remove os itens conflitantes da LC (OBS.: De tras pra frente pra nao ter problemas com indices)
            for (int i=listaCandidatos.size()-1; i>=0; i--) 
                if (inst.restricoes[itemSelecionado.index][listaCandidatos.at(i).index])
                    listaCandidatos.erase(listaCandidatos.begin()+i);
        }
    }
    
    return sol;
}