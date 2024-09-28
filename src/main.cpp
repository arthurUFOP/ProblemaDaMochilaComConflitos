#include <time.h>
#include <stdio.h>
#include <chrono>
#include "Instancia.h"
#include "Solucao.h"
#include "Avaliador.h"
#include "HeuristicasConstrutivas.h"
#include "BuscasLocais.h"
#include "Metaheuristica.h"
#include "Grasp.h"
#include "SimulatedAnnealing.h"
#include "ILS.h"

using std::cin;
using std::ofstream; 

#define CAMINHO_BASE "./LOGS/"

// Main Com Menu Principal
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

int menuBuscaLocal()
{
    int bl;
    printf("---- Buscas Locais ---\n");
    printf("Selecione uma opção:\n");
    printf("1 - Destroy&Repair\n");
    printf("2 - Descida Inversao de 2 bits FI.\n");
    printf("3 - Descida Inversao de 2 a N bits FI.\n");
    printf("4 - Descida Inversao de 2 bits BI.\n");
    do
    {
        printf("Digite a opção escolhida: ");
        cin >> bl; 
    } while ( (bl < 1) || (bl > 4) );

    return bl;
}

int menuMetaheuristicas()
{
    int bl;
    printf("---- Metaheuristicas ---\n");
    printf("Selecione uma opção:\n");
    printf("1 - GRASP\n");
    printf("2 - Simulated Annealing\n");
    printf("3 - Smart ILS\n");
    do
    {
        printf("Digite a opção escolhida: ");
        cin >> bl; 
    } while ( (bl < 1) || (bl > 3) );

    return bl;
}

int main(int argc, char** argv) {
    //srand(712347);
    srand(time(NULL));

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
    Solucao CI;
    ParametrosExtra paramExtra;
    BuscaLocal* buscaLocal;
    BuscaLocalDeCI* buscaLocalDeCI;
    Metaheuristica* metaHeuristica;
    

    int opcao;
    int hc, bl;
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
            case 2: {
                bl = menuBuscaLocal();
                switch (bl)
                {
                    case 1: {
                        buscaLocalDeCI = new DescidaDestroyAndRepair();
                        heuristicaConstrutiva = new DecomposicaoGulosa();
                        paramExtra[0] = 10; // Guloso pelo n. de restricoes
                        sol = heuristicaConstrutiva->gerarSolucao(*inst, paramExtra);
                        CI = dynamic_cast<DecomposicaoGulosa*>(heuristicaConstrutiva)->conjuntoIndependente;

                        cout << "Solucao obtida INICIALMENTE: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;

                        sol = buscaLocalDeCI->aprimorarSolucao(*inst, sol, CI);

                        cout << "Solucao obtida APOS BUSCA LOCAL DE CI: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
                        break;
                    }

                    case 2: {
                        buscaLocal = new DescidaInversaoDe2BitsFI();
                        heuristicaConstrutiva = new DecomposicaoGulosa();
                        paramExtra[0] = 10; // Guloso pelo n. de restricoes
                        sol = heuristicaConstrutiva->gerarSolucao(*inst, paramExtra);
                        cout << "Solucao obtida INICIALMENTE: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;

                        sol = buscaLocal->aprimorarSolucao(*inst, sol);
                        cout << "Solucao obtida APOS BUSCA LOCAL DE CI: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
                        break;
                    }

                    case 3: {
                        buscaLocal = new DescidaInversaoDe2aNBitsFI();
                        heuristicaConstrutiva = new DecomposicaoGulosa();
                        paramExtra[0] = 10; // Guloso pelo n. de restricoes
                        sol = heuristicaConstrutiva->gerarSolucao(*inst, paramExtra);
                        cout << "Solucao obtida INICIALMENTE: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;

                        sol = buscaLocal->aprimorarSolucao(*inst, sol);
                        cout << "Solucao obtida APOS BUSCA LOCAL DE CI: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
                        break;
                    }

                    case 4: {
                        buscaLocal = new DescidaInversaoDe2BitsBI();
                        heuristicaConstrutiva = new DecomposicaoGulosa();
                        paramExtra[0] = 10; // Guloso pelo n. de restricoes
                        sol = heuristicaConstrutiva->gerarSolucao(*inst, paramExtra);
                        cout << "Solucao obtida INICIALMENTE: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;

                        sol = buscaLocal->aprimorarSolucao(*inst, sol);
                        cout << "Solucao obtida APOS BUSCA LOCAL DE CI: ";
                        imprimeSol(sol);
                        cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                        << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                        << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                        << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            // Metaheuristica
            case 3: {
                bl = menuMetaheuristicas();
                switch (bl)
                {
                case 1:
                    metaHeuristica = new Grasp();
                    sol = metaHeuristica->gerarSolucao(*inst);
                    break;
                
                case 2:
                    metaHeuristica = new SimulatedAnnealing();
                    sol = metaHeuristica->gerarSolucao(*inst);
                
                case 3:
                    metaHeuristica = new ILS();
                    sol = metaHeuristica->gerarSolucao(*inst);
                
                default:
                    break;
                }

                cout << "Solucao obtida: ";
                imprimeSol(sol);
                cout << endl << "FO: " << avaliaFO(*inst, sol) << endl 
                << "Peso da Solucao: " << avaliaPeso(*inst, sol) << endl
                << "Peso Maximo da Mochila: " << inst->pesoMax << endl
                << "Solucao Valida: " << avaliaValidade(*inst, sol) << endl;
            }
            
            default: {
                break;
            }
        }
    }
    return 0;
}


// Funcao de Log de Experimentos
void logExperimento(string logCaminho, string metodoNome, string instanciaCaminho, int nItens, int nRest, float pesoMax, float duracao_ms, float fo, float pesoNaMochila, bool validade) {
    ofstream arq(logCaminho, std::ios::app);
    if(arq.is_open()) {
        arq << metodoNome << "," << instanciaCaminho << "," << nItens << "," << nRest << "," << pesoMax << "," << duracao_ms << "," << fo << "," << pesoNaMochila << "," << (validade ? 1 : 0) << endl;
    }
    else {
        cout << "Erro durante o LOG do experimento. Nao foi possivel abrir " << logCaminho << endl;
        exit(1);
    }
}

void logExperimentoBL(string logCaminho, string metodoNome, string instanciaCaminho, int nItens, int nRest, float pesoMax, float duracao_ms, float fo, float pesoNaMochila, bool validade, float difFoInicialFinal) {
    ofstream arq(logCaminho, std::ios::app);
    if(arq.is_open()) {
        arq << metodoNome << "," << instanciaCaminho << "," << nItens << "," << nRest << "," << pesoMax << "," << duracao_ms << "," << fo << "," << pesoNaMochila << "," << (validade ? 1 : 0) << "," << difFoInicialFinal << endl;
    }
    else {
        cout << "Erro durante o LOG do experimento. Nao foi possivel abrir " << logCaminho << endl;
        exit(1);
    }

}

/*
// Main para os experimentos com as heuristicas construtivas
int main(int argc, char** argv) {
    srand(time(NULL));

    // Instanciando as quatro heuristicas
    HeuristicaConstrutiva** hcs = new HeuristicaConstrutiva*[4];
    hcs[0] = new DecomposicaoGulosa();
    hcs[1] = new DecomposicaoAleatoria();
    hcs[2] = new InsercaoGulosa();
    hcs[3] = new InsercaoAleatoria();

    // Vetor de nomes
    string hcNomes[] = {"DecomposicaoGulosa", "DecomposicaoAleatoria", "InsercaoGulosa", "InsercaoAleatoria"};

    for (int i=0; i<4; i++) {
        // Determinando a heuristica em especifico
        HeuristicaConstrutiva* hc = hcs[i];
        string hcNome = hcNomes[i];
        string logCaminho = CAMINHO_BASE + hcNome + ".csv";

        // Caminhos das instancias
        string instanciasCaminhos[] = { "./instancias/I1_I10/10I1", "./instancias/I1_I10/10I2", "./instancias/I1_I10/10I3", "./instancias/I1_I10/10I4",
                            "./instancias/I1_I10/10I5", "./instancias/I1_I10/1I1", "./instancias/I1_I10/1I2", "./instancias/I1_I10/1I3", "./instancias/I1_I10/1I4",
                            "./instancias/I1_I10/1I5", "./instancias/I1_I10/2I1", "./instancias/I1_I10/2I2", "./instancias/I1_I10/2I3", "./instancias/I1_I10/2I4",
                            "./instancias/I1_I10/2I5", "./instancias/I1_I10/3I1", "./instancias/I1_I10/3I2", "./instancias/I1_I10/3I3", "./instancias/I1_I10/3I4",
                            "./instancias/I1_I10/3I5", "./instancias/I1_I10/4I1", "./instancias/I1_I10/4I2", "./instancias/I1_I10/4I3", "./instancias/I1_I10/4I4",
                            "./instancias/I1_I10/4I5", "./instancias/I1_I10/5I1", "./instancias/I1_I10/5I2", "./instancias/I1_I10/5I3", "./instancias/I1_I10/5I4",
                            "./instancias/I1_I10/5I5", "./instancias/I1_I10/6I1", "./instancias/I1_I10/6I2", "./instancias/I1_I10/6I3", "./instancias/I1_I10/6I4",
                            "./instancias/I1_I10/6I5", "./instancias/I1_I10/7I1", "./instancias/I1_I10/7I2", "./instancias/I1_I10/7I3", "./instancias/I1_I10/7I4",
                            "./instancias/I1_I10/7I5", "./instancias/I1_I10/8I1", "./instancias/I1_I10/8I2", "./instancias/I1_I10/8I3", "./instancias/I1_I10/8I4",
                            "./instancias/I1_I10/8I5", "./instancias/I1_I10/9I1", "./instancias/I1_I10/9I2", "./instancias/I1_I10/9I3", "./instancias/I1_I10/9I4",
                            "./instancias/I1_I10/9I5", "./instancias/I11-I20/11I1.txt", "./instancias/I11-I20/11I2.txt", "./instancias/I11-I20/11I3.txt", "./instancias/I11-I20/11I4.txt", "./instancias/I11-I20/11I5.txt",
                            "./instancias/I11-I20/12I1.txt", "./instancias/I11-I20/12I2.txt", "./instancias/I11-I20/12I3.txt", "./instancias/I11-I20/12I4.txt", "./instancias/I11-I20/12I5.txt",
                            "./instancias/I11-I20/13I1.txt", "./instancias/I11-I20/13I2.txt", "./instancias/I11-I20/13I3.txt", "./instancias/I11-I20/13I4.txt", "./instancias/I11-I20/13I5.txt",
                            "./instancias/I11-I20/14I1.txt", "./instancias/I11-I20/14I2.txt", "./instancias/I11-I20/14I3.txt", "./instancias/I11-I20/14I4.txt", "./instancias/I11-I20/14I5.txt",
                            "./instancias/I11-I20/15I1.txt", "./instancias/I11-I20/15I2.txt", "./instancias/I11-I20/15I3.txt", "./instancias/I11-I20/15I4.txt", "./instancias/I11-I20/15I5.txt",
                            "./instancias/I11-I20/16I1.txt", "./instancias/I11-I20/16I2.txt", "./instancias/I11-I20/16I3.txt", "./instancias/I11-I20/16I4.txt", "./instancias/I11-I20/16I5.txt",
                            "./instancias/I11-I20/17I1.txt", "./instancias/I11-I20/17I2.txt", "./instancias/I11-I20/17I3.txt", "./instancias/I11-I20/17I4.txt", "./instancias/I11-I20/17I5.txt",
                            "./instancias/I11-I20/18I1.txt", "./instancias/I11-I20/18I2.txt", "./instancias/I11-I20/18I3.txt", "./instancias/I11-I20/18I4.txt", "./instancias/I11-I20/18I5.txt",
                            "./instancias/I11-I20/19I1.txt", "./instancias/I11-I20/19I2.txt", "./instancias/I11-I20/19I3.txt", "./instancias/I11-I20/19I4.txt", "./instancias/I11-I20/19I5.txt",
                            "./instancias/I11-I20/20I1.txt", "./instancias/I11-I20/20I2.txt", "./instancias/I11-I20/20I3.txt", "./instancias/I11-I20/20I4.txt", "./instancias/I11-I20/20I5.txt"};
        
        // Execucao do primeiro grupo
        for (auto instanciaCaminho : instanciasCaminhos) {

            // Realizando o experimento
            Instancia inst = Instancia(instanciaCaminho);
            ParametrosExtra pExtra;
            pExtra[0] = 1; // Ordenar por valor no Decomposicao Gulosa
            auto inicio = std::chrono::high_resolution_clock::now();
            Solucao sol = hc->gerarSolucao(inst, pExtra);
            auto fim = std::chrono::high_resolution_clock::now();

            // Calculando metricas a serem reportadas
            auto duracao_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
            float fo = avaliaFO(inst, sol);
            bool validade = avaliaValidade(inst, sol);
            float peso = avaliaPeso(inst, sol);

            logExperimento(logCaminho, hcNome, instanciaCaminho, inst.nItens, inst.nRest, inst.pesoMax, duracao_ms, fo, peso, validade);
            cout << "Experimento com " << hcNome << " na instancia " << instanciaCaminho << " terminado!" << endl;

            if (hcNome == "DecomposicaoGulosa") {
                pExtra[0] = 0; // Ordenar por quantidade de restricoes (decrescente)
                inicio = std::chrono::high_resolution_clock::now();
                sol = hc->gerarSolucao(inst, pExtra);
                fim = std::chrono::high_resolution_clock::now();

                // Calculando metricas a serem reportadas
                duracao_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
                fo = avaliaFO(inst, sol);
                validade = avaliaValidade(inst, sol);
                peso = avaliaPeso(inst, sol);

                logExperimento(logCaminho+"-gulosoPorRestricoesDecrescente", hcNome, instanciaCaminho, inst.nItens, inst.nRest, inst.pesoMax, duracao_ms, fo, peso, validade);
                cout << "Experimento com " << hcNome << " (ordenado por nRest) na instancia " << instanciaCaminho << " terminado!" << endl;
            }
        }
    }
    
    delete[] hcs;

    return 0;
}
*/

/*
// Main para os experimentos com as buscas locais
int main(int argc, char** argv) {
    srand(time(NULL));

    // Instanciando a heuristica construtiva (a melhor do ultimo experimento)
    HeuristicaConstrutiva* hc = new DecomposicaoGulosa();
    ParametrosExtra pExtra;
    pExtra[0] = 10; // Ordenar por quantidade de restricoes (decrescente)

    // Vetor de nomes
    string blNomesInversaoBit[] = {"DI2BFI", "DI2BBI", "DI2-NBFI"};

    // Caminhos das instancias
    string instanciasCaminhos[] = { "./instancias/I1_I10/10I1", "./instancias/I1_I10/10I2", "./instancias/I1_I10/10I3", "./instancias/I1_I10/10I4",
                            "./instancias/I1_I10/10I5", "./instancias/I1_I10/1I1", "./instancias/I1_I10/1I2", "./instancias/I1_I10/1I3", "./instancias/I1_I10/1I4",
                            "./instancias/I1_I10/1I5", "./instancias/I1_I10/2I1", "./instancias/I1_I10/2I2", "./instancias/I1_I10/2I3", "./instancias/I1_I10/2I4",
                            "./instancias/I1_I10/2I5", "./instancias/I1_I10/3I1", "./instancias/I1_I10/3I2", "./instancias/I1_I10/3I3", "./instancias/I1_I10/3I4",
                            "./instancias/I1_I10/3I5", "./instancias/I1_I10/4I1", "./instancias/I1_I10/4I2", "./instancias/I1_I10/4I3", "./instancias/I1_I10/4I4",
                            "./instancias/I1_I10/4I5", "./instancias/I1_I10/5I1", "./instancias/I1_I10/5I2", "./instancias/I1_I10/5I3", "./instancias/I1_I10/5I4",
                            "./instancias/I1_I10/5I5", "./instancias/I1_I10/6I1", "./instancias/I1_I10/6I2", "./instancias/I1_I10/6I3", "./instancias/I1_I10/6I4",
                            "./instancias/I1_I10/6I5", "./instancias/I1_I10/7I1", "./instancias/I1_I10/7I2", "./instancias/I1_I10/7I3", "./instancias/I1_I10/7I4",
                            "./instancias/I1_I10/7I5", "./instancias/I1_I10/8I1", "./instancias/I1_I10/8I2", "./instancias/I1_I10/8I3", "./instancias/I1_I10/8I4",
                            "./instancias/I1_I10/8I5", "./instancias/I1_I10/9I1", "./instancias/I1_I10/9I2", "./instancias/I1_I10/9I3", "./instancias/I1_I10/9I4",
                            "./instancias/I1_I10/9I5", "./instancias/I11-I20/11I1.txt", "./instancias/I11-I20/11I2.txt", "./instancias/I11-I20/11I3.txt", "./instancias/I11-I20/11I4.txt", "./instancias/I11-I20/11I5.txt",
                            "./instancias/I11-I20/12I1.txt", "./instancias/I11-I20/12I2.txt", "./instancias/I11-I20/12I3.txt", "./instancias/I11-I20/12I4.txt", "./instancias/I11-I20/12I5.txt",
                            "./instancias/I11-I20/13I1.txt", "./instancias/I11-I20/13I2.txt", "./instancias/I11-I20/13I3.txt", "./instancias/I11-I20/13I4.txt", "./instancias/I11-I20/13I5.txt",
                            "./instancias/I11-I20/14I1.txt", "./instancias/I11-I20/14I2.txt", "./instancias/I11-I20/14I3.txt", "./instancias/I11-I20/14I4.txt", "./instancias/I11-I20/14I5.txt",
                            "./instancias/I11-I20/15I1.txt", "./instancias/I11-I20/15I2.txt", "./instancias/I11-I20/15I3.txt", "./instancias/I11-I20/15I4.txt", "./instancias/I11-I20/15I5.txt",
                            "./instancias/I11-I20/16I1.txt", "./instancias/I11-I20/16I2.txt", "./instancias/I11-I20/16I3.txt", "./instancias/I11-I20/16I4.txt", "./instancias/I11-I20/16I5.txt",
                            "./instancias/I11-I20/17I1.txt", "./instancias/I11-I20/17I2.txt", "./instancias/I11-I20/17I3.txt", "./instancias/I11-I20/17I4.txt", "./instancias/I11-I20/17I5.txt",
                            "./instancias/I11-I20/18I1.txt", "./instancias/I11-I20/18I2.txt", "./instancias/I11-I20/18I3.txt", "./instancias/I11-I20/18I4.txt", "./instancias/I11-I20/18I5.txt",
                            "./instancias/I11-I20/19I1.txt", "./instancias/I11-I20/19I2.txt", "./instancias/I11-I20/19I3.txt", "./instancias/I11-I20/19I4.txt", "./instancias/I11-I20/19I5.txt",
                            "./instancias/I11-I20/20I1.txt", "./instancias/I11-I20/20I2.txt", "./instancias/I11-I20/20I3.txt", "./instancias/I11-I20/20I4.txt", "./instancias/I11-I20/20I5.txt"};
    
    BuscaLocal** bls = new BuscaLocal*[3];
    bls[0] = new DescidaInversaoDe2BitsFI();
    bls[1] = new DescidaInversaoDe2BitsBI();
    bls[2] = new DescidaInversaoDe2aNBitsFI();

    // Execucao do primeiro grupo
    for (auto instanciaCaminho : instanciasCaminhos) {
        // Gerando solucao base
        Instancia inst = Instancia(instanciaCaminho);
        Solucao solBase = hc->gerarSolucao(inst, pExtra);
        float foBase = avaliaFO(inst, solBase);

        for (int i=0; i<3; i++) {
            cout << "Testando a " << instanciaCaminho << " com o metodo " << i+1 << endl;

            // Determinando a BL em especifico
            string blNome = blNomesInversaoBit[i];
            string logCaminho = CAMINHO_BASE + blNome + ".csv";
            BuscaLocal* bl = bls[i];

            // Realizando o experimento
            auto inicio = std::chrono::high_resolution_clock::now();
            Solucao sol = bl->aprimorarSolucao(inst, solBase);
            auto fim = std::chrono::high_resolution_clock::now();

            // Calculando metricas a serem reportadas
            auto duracao_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
            float fo = avaliaFO(inst, sol);
            bool validade = avaliaValidade(inst, sol);
            float peso = avaliaPeso(inst, sol);

            logExperimentoBL(logCaminho, blNome, instanciaCaminho, inst.nItens, inst.nRest, inst.pesoMax, duracao_ms, fo, peso, validade, fo-foBase);
            cout << "Experimento com " << blNome << " na instancia " << instanciaCaminho << " terminado!" << endl;
        }
    }
    
    delete[] bls;

    // Execução do destroy and repair
    for (auto instanciaCaminho : instanciasCaminhos) {
        // Gerando solucao base
        Instancia inst = Instancia(instanciaCaminho);
        Solucao solBase = hc->gerarSolucao(inst, pExtra);
        float foBase = avaliaFO(inst, solBase);

        string blNome = "Destroy&Repair";
        string logCaminho = CAMINHO_BASE + blNome + ".csv";
        BuscaLocalDeCI* bl = new DescidaDestroyAndRepair();

        // Realizando o experimento
        auto inicio = std::chrono::high_resolution_clock::now();
        Solucao sol = bl->aprimorarSolucao(inst, solBase, dynamic_cast<DecomposicaoGulosa*>(hc)->conjuntoIndependente);
        auto fim = std::chrono::high_resolution_clock::now();

        // Calculando metricas a serem reportadas
        auto duracao_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
        float fo = avaliaFO(inst, sol);
        bool validade = avaliaValidade(inst, sol);
        float peso = avaliaPeso(inst, sol);
        logExperimentoBL(logCaminho, blNome, instanciaCaminho, inst.nItens, inst.nRest, inst.pesoMax, duracao_ms, fo, peso, validade, fo-foBase);
        cout << "Experimento com " << blNome << " na instancia " << instanciaCaminho << " terminado!" << endl;
    }

    return 0;
}
*/