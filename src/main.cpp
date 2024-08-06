#include <time.h>
#include <stdio.h>
#include "Instancia.h"
#include "Solucao.h"
#include "Avaliador.h"
#include "HeuristicasConstrutivas.h"

using std::cin;

int menuPrincipal()
{
    int op;
    printf("======================================================\n");
    printf("===  BCC466 - Metaheurísticas - Trabalho Pratico   ===\n");
    printf("=== Problema da Mochila com Restricao de Conflitos ===\n");
    printf("======================================================\n");
    printf("Selecione uma opção: \n");
    printf("1 - Heurísticas Construtivas.\n");
    printf("2 - Buscas Locais.\n");
    printf("3 - Metaheurísticas.\n");
    printf("0 - Sair\n");
    do {   
        printf("Digite a opção escolhida: ");
        cin >> op;
    } while( (op < 0) || (op > 3) );
    return op;
}

int menuHeuristicaConstrutiva()
{
    int hc;
    printf("---- Heurísticas Construtivas ---\n");
    printf("Selecione uma opção:\n");
    printf("1 - Decomposicao Gulosa.\n");
    printf("2 - Decomposicao Aleatoria.\n");
    printf("3 - Insercao Gulosa.\n");
    printf("4 - Insercao Aleatoria.\n");
    do
    {
        printf("Digite a opção escolhida: ");
        cin >> hc; 
    } while ( (hc < 1) || (hc > 4) );

    return hc;
    
    
}

int main(int argc, char** argv) {
    srand(712347);
    //srand(time(NULL));

    // Leitura da instancia (ou informada por linha de comando, ou caminho default)
    Instancia* inst;
    if (argc>1) {
        inst = new Instancia(argv[1]);
        cout << "Instancia carregada do arquivo " << argv[1] << endl; 
    }
    else {
        inst = new Instancia();
        cout << "Instancia carregada do arquivo padrao" << endl; 
    }

    HeuristicaConstrutiva* heuristicaConstrutiva;
    Solucao sol;
    ParametrosExtra paramExtra;
    int opcao;
    int hc;
    while (true) {
        opcao = menuPrincipal();
        if (opcao == 0)
            break;
        switch (opcao) {

            // Heuristica construtiva
            case 1: {
                hc = menuHeuristicaConstrutiva();
                switch (hc)
                {
                    // Decomposicao Gulosa
                    case 1:{
                        heuristicaConstrutiva = new DecomposicaoGulosa();
                        paramExtra[0] = 10; // Guloso pelo n. de restricoes
                        sol = heuristicaConstrutiva->gerarSolucao(*inst, paramExtra);
                        break;
                    }

                    // Decomposicao Aleatoria
                    case 2:{
                        heuristicaConstrutiva = new DecomposicaoAleatoria();
                        sol = heuristicaConstrutiva->gerarSolucao(*inst);
                        break;
                    }

                    // Insercao Gulosa
                    case 3:{
                        heuristicaConstrutiva = new InsercaoGulosa();
                        sol = heuristicaConstrutiva->gerarSolucao(*inst); // Implementar depois: guloso por outras metricas
                        break;
                    }

                    // Insercao Aleatoria
                    case 4:{
                        heuristicaConstrutiva = new InsercaoAleatoria();
                        sol = heuristicaConstrutiva->gerarSolucao(*inst);
                        break;
                    }

                    default:{
                        cout << "Erro inesperado!" << endl;
                        exit(1);
                        break;
                    }
                }
                cout << "Solucao obtida: ";
                imprimeSol(sol);
                cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
                break;
            }

            // Busca Local

            // Metaheuristica
            
            default: {
                cout << "Opcao ainda nao implementada!" << endl;
                break;
            }
        }
    }
    return 0;
}
