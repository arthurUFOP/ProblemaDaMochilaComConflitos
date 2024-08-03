#include "HeuristicasConstrutivas.h"

/*
    Artigo referencia: 
        Matheuristicas para o problema da mochila com restricao de conflitos
        Adriano Rodrigues Alves, Edna A. Hoshino
    
    Metodos de decomposicao (3.1.) --> DecomposicaoGulosa
        Primeiro resolve o problema das restrições (em essencia um conjunto independente) através de uma heuristica.
        Depois utiliza o método exato sobre o subproblema gerado. Irei testar com heuristica nesse passo tambem!
*/

// Comparador entre itens (valor)
struct sCompValor {
  bool operator() (Item i,Item j) {return (i.valor<j.valor);}
} CompValor;

// Comparador entre itens (n. Restricoes, DESCENDENTE)
struct sCompRest {
  bool operator() (Item i,Item j) {return (i.nRest>j.nRest);}
} CompRest;

DecomposicaoGulosa::DecomposicaoGulosa() {}
DecomposicaoGulosa::~DecomposicaoGulosa() {}

// Algoritmo 1 do paper
// OBS.: MODULARIZAR DEPOIS PARA SER GULOSO COM CRITERIOS DIFERENTES
// pExtra[0] == 0 -> valor
// Qualquer outra coisa -> n. de restricoes
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
        int indexSelecionado = listaCandidatos.back().index; // Candidato de maior valor
        sol.at(indexSelecionado) = 1; // Adiciona o candidato de maior valor
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
    float melhorFo = 0;
    //melhorSol = backtrack(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0);

    int iter = 0;
    bool continua = true;
    melhorSol = backtrackComIteracaoMaxima(inst, melhorSol, &melhorFo, sol, itensValidos, 0, 0, &iter, 10000, &continua);

    return melhorSol;
}