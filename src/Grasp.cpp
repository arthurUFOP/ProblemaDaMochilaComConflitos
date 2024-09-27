#include "Grasp.h"

Grasp::Grasp() {
    this->alpha = 0.2;
    this->graspMax = 15;
    this->bl = new DescidaDestroyAndRepair();
}

Grasp::Grasp(float alpha, int graspMax) {
    this->alpha = alpha;
    this->graspMax = graspMax;
    this->bl = new DescidaDestroyAndRepair();
}

Grasp::~Grasp() {
    delete this->bl;
}

// Retorna a solucao e seu CI
pair<Solucao, Solucao> Grasp::construcao(Instancia& inst) {
    MetodoExato* metodoExato = new CPLEXMochilaBasico(inst.nItens, 0, false);

    // Gerando o CI
    Solucao CI = geraMochilaVazia(inst.nItens);

    // Candidatos ordenados por seu valor
    vector<Item> listaCandidatos = inst.itens;

    sort(listaCandidatos.begin(), listaCandidatos.end(), CompRest);

    while (!listaCandidatos.empty()) {

        // gMin, gMax & g(t)
        int gMin = listaCandidatos.back().nRest;
        int gMax = listaCandidatos.front().nRest;
        float gValue = gMin + (this->alpha*(gMax-gMin));

        // calculando a quantidade de valores k que podem ser sorteados
        int k = 0;
        while (k < listaCandidatos.size() && listaCandidatos.at(listaCandidatos.size()-1-k).nRest <= gValue) 
            k++;

        int indexSorteado = listaCandidatos.size() - 1 - rand()%k;

        int indexSelecionado = listaCandidatos.at(indexSorteado).index; // Candidato de maior metrica
        CI.at(indexSelecionado) = 1; // Adiciona o candidato de maior metrica
        listaCandidatos.erase(listaCandidatos.begin()+indexSorteado); // Remove ele da lista de candidatos

        // Verifica quais candidatos sao incompativeis com o recentemente adicionado
        vector<int> indexesRemover = vector<int>();
        for (int i=0; i<listaCandidatos.size(); i++) 
            if (inst.restricoes[indexSelecionado][listaCandidatos.at(i).index]) 
                indexesRemover.push_back(i);
        
        // Remove tais candidatos
        for (auto it=indexesRemover.rbegin(); it<indexesRemover.rend(); it++) // Remover de tras pra frente para nao baguncar os indices
            listaCandidatos.erase(listaCandidatos.begin()+(*it));

    }

    // Preparando a solucao otima deste CI gerado

    int nItensSubproblema = 0;
    for (auto v : CI)
        nItensSubproblema += v;
        
    // Itens do subproblema
    vector<Item> itensSubproblema;
    for (int i=0; i<CI.size(); i++)
        if (CI.at(i))
            itensSubproblema.push_back(inst.itens.at(i));
    
    Instancia instSubproblema = Instancia(nItensSubproblema, inst.pesoMax, itensSubproblema); // Construtor para subproblema CPLEX
    
    //Metodo Exato para geracao em cima do CI
    Solucao sol = metodoExato->gerarSolucaoOtima(instSubproblema);

    return pair<Solucao, Solucao>(sol, CI);
}

Solucao Grasp::gerarSolucao(Instancia& inst) {
    Solucao melhorSol, sol, CI;
    float foMax = -999999999999;
    float fo;

    for (int i=0; i<this->graspMax; i++) {
        cout << "Iteracao GRASP = " << i+1 << endl;

        // Construcao + BuscaLocal
        auto data = this->construcao(inst);
        sol = data.first;
        CI = data.second;
        sol = this->bl->aprimorarSolucao(inst, sol, CI);
        fo = avaliaFO(inst, sol);

        // Encontrou melhora?
        if (fo > foMax) {
            cout << "Melhoria Encontrada! " << foMax << " --> " << fo << endl;
            melhorSol = sol;
            foMax = fo;
        }
    }

    return melhorSol;
}