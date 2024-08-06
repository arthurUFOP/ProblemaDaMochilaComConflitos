#include "MetodosExatos.h"
#include <ilcplex/ilocplex.h>
#include <ilcplex/cplex.h>
ILOSTLBEGIN
#include <ilconcert/ilomodel.h>

CPLEXMochilaBasico::CPLEXMochilaBasico() {
    cout << "Erro: CPLEXMochilaBasico deve ser instanciado com parametros!";
    exit(1);
}

CPLEXMochilaBasico::CPLEXMochilaBasico(int verdadeiroTamanho, float tolerancia, bool verbose) {
    this->verdadeiroTamanho = verdadeiroTamanho;
    this->verbose = verbose;
    this->tolerancia = tolerancia;
}
CPLEXMochilaBasico::~CPLEXMochilaBasico() {}

// Gera a solucao otima para o problema da mochila 0-1 convencional.
// inst recebe uma instancia modificada formada pelo conjunto independente gerado na decomposicao
// verdadeiroTamanho eh a quantidade de itens no problema original
Solucao CPLEXMochilaBasico::gerarSolucaoOtima(Instancia& inst) {
    Solucao sol = geraMochilaVazia(this->verdadeiroTamanho);
    IloEnv env;
    try {
        // Criação do modelo CPLEX
        IloModel model(env);

        // Dados do problema
        int numItems = inst.nItens;
        int capacities = inst.pesoMax;
        //int values[] = {60, 100, 120, 80, 90}; // Valores dos itens
        //int weights[] = {10, 20, 30, 15, 25}; // Pesos dos itens

        // Definindo variáveis binárias para cada item
        vector<IloNumVar> x(numItems);
        for (int i = 0; i < numItems; ++i) {
            x[i] = IloNumVar(env, 0, 1, ILOINT);
            model.add(x[i]);
        }

        // Adiciona a função objetivo: maximizar a soma dos valores dos itens
        IloObjective objective = IloMaximize(env);
        IloExpr objExpr(env);
        for (int i = 0; i < numItems; ++i) {
            objExpr += inst.itens.at(i).valor * x[i];
        }
        objective.setExpr(objExpr);
        model.add(objective);

        // Adiciona a restrição de capacidade da mochila
        IloExpr capacityExpr(env);
        for (int i = 0; i < numItems; ++i) {
            capacityExpr += inst.itens.at(i).peso * x[i];
        }
        model.add(capacityExpr <= capacities);

        // Criação do solucionador CPLEX e resolução do modelo
        IloCplex cplex(model);
        cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, this->tolerancia); // Garante com nivel de tolerancia do otimo
        // Ex.: tolerancia=0.2 -> Solução com lacuna de até 20% do ótimo

        // Encontrou solucao
        if (cplex.solve()) {

            // Impressao do encontro da solucao
            if (this->verbose) {
                cout << "Solução encontrada:" << endl;
                cout << "Valor total: " << cplex.getObjValue() << endl;
                cout << "Itens na mochila:" << endl;
            }

            // Salvando a solucao encontrada em sol
            for (int i = 0; i < numItems; ++i) {
                double value = cplex.getValue(x[i]);
                sol.at(inst.itens.at(i).index) = value;

                if (verbose)
                    cout << "x[" << i << "] = " << value << endl;
            }
        } 
        // Nao encontrou solucao
        else 
            if (verbose)
                cout << "Nenhuma solução encontrada. Devolvendo solucao vazia..." << endl;
        
    // Erros inesperados
    } catch (IloException& ex) {
        cout << "Erro: " << ex.getMessage() << endl;
        exit(1);
    } catch (...) {
        cout << "Erro desconhecido no CPLEX." << endl;
        exit(1);
    }

    // Finalização do ambiente CPLEX
    env.end();

    return sol;
}
