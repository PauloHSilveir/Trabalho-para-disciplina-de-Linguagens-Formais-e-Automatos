//    Universidade Federal de Lavras
//    Trabalho Prático para disciplina Linguagens Formais e Automatos
//    Curso: Ciência da Computação.
//    Turma: 10 A.
//    Aluno: Paulo Henrique dos Anjos Silveira.
// -------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <cctype>

using namespace std;

template <typename T>
bool contains(const vector<T>& vec, const T& value) {
    return find(vec.begin(), vec.end(), value) != vec.end();
}

class Gramatica {
    private:
        string simboloInicial;

        vector<char> alfabeto;

        vector<string> variaveis;

        map<string, vector<string>> producoes;

    public:
        //Construtor
        Gramatica (string sInicial, vector<char> alfabeto, vector<string> variaveis,
                   map<string, vector<string>> gramatica) {

            this->simboloInicial = sInicial;
            this->alfabeto = alfabeto;
            this->variaveis = variaveis;
            producoes = gramatica;
        }

        vector<string>& regras(pair<const string, vector<string>>& producao) {
            return producao.second;
        }
        const string variavel(const pair<const string, vector<string>>& producao) {
            return producao.first;
        }
        void removeRegra(vector<string>& regras, const string& regraParaRemover) {
            regras.erase(remove(regras.begin(), regras.end(), regraParaRemover), regras.end());
        }
        void removeProducao(const string& variavel) {
                producoes.erase(variavel);
        }

        //Procura em toda as regras por um simbolo inicial recursivo
        //Se o simbolo inicial for recursivo, define um novo inicial
        void remRecSimboloInicial() {

            for(auto& producao : producoes) {

                for(auto& regra : regras(producao)) {

                    for(auto& simbolo : regra) {

                        if(simboloInicial == string(1, simbolo)) {

                            string novoInicial = simboloInicial + "'";

                            producoes[novoInicial].push_back(simboloInicial);

                            variaveis.push_back(novoInicial);
                            simboloInicial = novoInicial;
                            
                            break;
                        }
                    }
                }
            }
        }
        //Procura por variaveis que geram lambda diretamente ou indiretamente
        vector<string> variaveisAnulaveis() {

            vector<string> anulaveis, anterior;

            //Procura em cada regra se existe um váriavel composta somente por lambda
            //Se encontrar armazena em um vetor de anulaveis
            for(auto& producao : producoes) {

                for(auto& regra : regras(producao)) {

                    if(regra == ".") {

                        anulaveis.push_back(variavel(producao));

                        break;
                    }
                }
            }
            //Procura por variaveis que geram lambda indiretamente
            //Se alguma variavel ou regra ja estiver em anulaveis passa para a proxima iteração
            do {
                anterior = anulaveis;

                for(auto& producao : producoes) {
                    
                    if (contains(anulaveis, variavel(producao))) {
                        continue;
                    }
                    
                    for(auto& regra : regras(producao)) {

                        if(regra == ".") {
                            continue;
                        }

                        bool ehAnulavel = true;
                        for(auto& simbolo : regra) {

                            if (!contains(anulaveis, string(1, simbolo))) {

                                ehAnulavel = false;

                                break;
                            }
                        }

                        if(ehAnulavel) {

                            anulaveis.push_back(variavel(producao));

                            break;
                        }
                    }
                }
            } while (anulaveis != anterior);

            // cout << "Variáveis anulaveis:" << endl;
            // for (const auto& var : anulaveis) {
            //     cout << var << endl;
            // }
            return anulaveis;
        }

        void novasProducoes(vector<string>& anulaveis) {

            //O loop for que começa com 'mask' igual a zero continua ate que seja menor que 2^n. 
            // O que significa que 'mask' percorre todos os inteiros de 0 ate 2^n-1
            for (auto& producao : producoes) {

                vector<string> novasProducoes;

                for (auto& regra : regras(producao)) {

                    int n = regra.size();

                    // Itera sobre todos os subconjuntos de símbolos na produção
                    for (int mask = 0; mask < (1 << n); ++mask) {

                        string novaProducao = "";

                        for (int i = 0; i < n; ++i) {

                            string simbolo = string(1, regra[i]);

                            // Se o bit estiver definido ou se o simbolo não for anulável,
                            // Ele é incluido em novaProducao
                            if(!contains(anulaveis, simbolo) || (mask & (1 << i))) {
                            
                                novaProducao += simbolo;
                            }
                        }
                        if (novaProducao.empty() && variavel(producao) == simboloInicial) {

                            novaProducao = ".";
                        }
                        
                        if (!novaProducao.empty() && novaProducao != regra && 
                            !contains(novasProducoes, novaProducao)) {

                            novasProducoes.push_back(novaProducao);
                        }
                    }
                }
                // Adiciona as novas produções em regras
                regras(producao).insert(regras(producao).end(),
                     novasProducoes.begin(), novasProducoes.end());
            }
        }


        void removeProducaoLambda(vector<string> anulaveis) {

            vector<string> producoesARemover;
            
            for(auto& producao : producoes) {

                if(variavel(producao) != simboloInicial) {

                    removeRegra(regras(producao), ".");

                    removeRegra(regras(producao), variavel(producao));
                }

                // Coletar as produções a serem removidas se estiver vazia
                if (regras(producao).empty()) {

                    producoesARemover.push_back(variavel(producao));
                }
            }
            for (const auto& var : producoesARemover) {

                removeProducao(var);

                removeRegra(variaveis, var); // Remova da lista de variáveis
            }
        }


        vector<string> construirChain(const string& variavel) {

            vector<string> chain, anterior;

            chain.push_back(variavel);

            do {
                vector<string> novoChain = chain;
                
                // Novo chain passa a ser chain - anterior
                for (auto& elem : anterior) {

                    for (auto& elem2 : novoChain) {

                        if (elem == elem2) {

                            removeRegra(novoChain, elem);

                            break;
                        }
                    }
                }
                anterior = chain;

                // A partir do novo chain procura em regras outra variavel que gera chain
                for (auto& elem : novoChain) {

                    for (auto& regra : producoes.at(elem)) {

                        // Se a regra é uma única variável, adiciona ao conjunto se ainda não estiver nele
                        if (contains(variaveis, regra) && !contains(chain, regra)) {

                            chain.push_back(regra);
                        }
                    }
                    
                    
                }
                

            } while (chain != anterior);

            // cout << "chain de cada variavel\n";
            // for (int i = 0; i < chain.size(); i++)
            // {
            //     cout << chain[i] << " ";
            // }
            // cout << endl;
            return chain;
        }

        void substituirCadeias() {

            vector<string> chainVec;
            // Substituir as cadeias pelas produções equivalentes
            for (auto& producao : producoes) {

                chainVec = construirChain(variavel(producao));

                vector<string> novasProducoes = regras(producao);

                // Adiciona as produções equivalentes das cadeias
                for (auto& var : chainVec) {

                    if (var != variavel(producao)) {

                        for (auto& regra : producoes[var]) {

                            if (!contains(novasProducoes, regra)) {

                                novasProducoes.push_back(regra);
                            }
                        }
                    }
                }

                producoes[variavel(producao)] = novasProducoes;
            }
            // Remove todas as cadeias das producoes
            for(auto& producao : producoes) {

                vector<string> regrasParaRemover;
                // Remove todas as regras das producoes que são variaveis de tamanho 1 e maiuscula
                for(auto& regra : regras(producao)) {

                    if(contains(variaveis, regra)) {

                        regrasParaRemover.push_back(regra);
                    }
                }  
                for(auto& regra : regrasParaRemover) {

                    removeRegra(regras(producao), regra);
                } 
            }
        }


        vector<string> construirTerm() {

            vector<string> terminais;

            // Passo inicial: adiciona variáveis que geram diretamente terminais
            for (auto& producao : producoes) {

                for (auto& regra : regras(producao)) {

                    bool geraTerminal = true;
                    for (auto& simbolo : regra) {

                        if (!contains(alfabeto, simbolo)) {

                            geraTerminal = false;

                            break;
                        }
                    }

                    if (geraTerminal) {

                        terminais.push_back(variavel(producao));

                        break;
                    }
                }
            }

            if(terminais.size() != variaveis.size()) {
                vector<string> anterior;
                
                do {
                    anterior = terminais;

                    for (auto& producao : producoes) {

                        for (auto& regra : regras(producao)) {

                            bool pertenceAoTerm = true;
                            for (auto& simbolo : regra) {

                                if(!contains(anterior, string(1, simbolo)) && !contains(alfabeto, simbolo)) {

                                    pertenceAoTerm = false;

                                    break;
                                }
                            }

                            if (pertenceAoTerm) {

                                if (!contains(terminais, variavel(producao))) {

                                    terminais.push_back(variavel(producao));
                                }

                                break;
                            }
                        }
                    }

                } while (anterior != terminais);
            }
            
            // cout << "Esse é o term \n ";
            // for(auto& var:terminais) {
            //     cout << var << " ";

            // }
            // cout << endl;
            return terminais;
        }

        void removeNaoTerminais(vector<string> terminais) {

            vector<string> naoTerminais;

            // Encontra as variaveis não terminais
            for(auto& var : variaveis) {

                if(!contains(terminais, var)) {

                    naoTerminais.push_back(var);
                }
            }

            // Remove produções associadas às variáveis não terminais
            for (auto& producao : producoes) {
                
                vector<string> regrasAtualizadas;
                // Verifica cada regra da produção para remover regras contendo não terminais
                for(auto& regra : regras(producao)) {

                    bool temNaoTerminal = false;

                    for (auto& simbolo : regra) {
                        // Verifica se o caractere pertence ao conjunto de não terminais
                        if(contains(naoTerminais, string(1, simbolo))) {

                            temNaoTerminal = true;

                            break;
                        }
                    }
                    if(temNaoTerminal) {
                        regrasAtualizadas.push_back(regra);
                    }
                }
                for(auto& regra : regrasAtualizadas) {

                     removeRegra(regras(producao), regra);
                }
                // Se não existe regras em uma producao, remove toda a producao
                if (regras(producao).empty()) {

                    removeRegra(variaveis, variavel(producao));
                    
                    removeProducao(variavel(producao));
                }

            }
        }

        vector<string> construirReach() {

            vector<string> alcancaveis, anterior;
            
            // Alcancaveis a partir do inicial
            alcancaveis.push_back(simboloInicial);

            do {
                vector<string> novoAlcancavel = alcancaveis;

                // Novo alcancavel passa a ser alcancaveis - anterior
                for (auto& elem : anterior) {

                    for (auto& elem2 : novoAlcancavel) {

                        if (elem == elem2) {

                            removeRegra(novoAlcancavel, elem);

                            break;
                        }
                    }
                }
                
                anterior = alcancaveis;
                
                // Para o elemento em novo alcancavel, adicione novo alcancavel em alcancaveis
                for (auto& elem : novoAlcancavel) {

                    for (auto& regra : producoes[elem]) {

                        for(auto& simbolo : regra) {

                            string var = string(1, simbolo);

                            if (contains(variaveis, var)) {

                                if (!contains(alcancaveis, var)) {

                                    alcancaveis.push_back(var);
                                }
                            }
                        }
                    }
                    
                }

            } while (alcancaveis != anterior);
            // cout << "Variáveis alcançáveis: ";
            // for (const auto& var : alcancaveis) {
            //     cout << var << " ";
            // }
            // cout << endl;
            return alcancaveis;
        }

        void removeNaoAlcancaveis(vector<string>& alcancaveis) {

            vector<string> naoAlcancaveis;

            // Encontra as variáveis não alcançáveis
            for (auto& var : variaveis) {

                if (!contains(alcancaveis, var)) {

                    naoAlcancaveis.push_back(var);
                }
            }
            // Coletar as produções a serem removidas
            vector<string> producoesARemover;
            
            for (auto& producao : producoes) {

                if (contains(naoAlcancaveis, variavel(producao))) {

                    producoesARemover.push_back(variavel(producao));
                }
            }
            // Remover as produções associadas às variáveis não alcançáveis
            for (auto& var : producoesARemover) {

                removeProducao(var);

                removeRegra(variaveis, var); // Remova da lista de variáveis
            }
        }

        string gerarNovaVariavel(string simbolo) {

            simbolo[0] = toupper(simbolo[0]);
            
            return simbolo + "'";
        }

        string gerarNovaVariavelT() {

            static int contador = 1;
            string novo = "T" + to_string(contador);

            contador++;
            
            return novo;
        }

        int contarSimbolos(string& regra) {

            int count = 0;
            for(auto& simbolo : regra) {

                if(simbolo == '\'') {
                    continue;
                }
                count++;
            }
            return count;
        }

        string verificaSeRegraTExiste(string regraBuscada, vector<pair<string, string>> novasProducoes) {

            for(auto& producao : novasProducoes) {
                
                string var = producao.first;

                if(var[0] == 'T') {

                    if(regraBuscada == producao.second) {

                        return var;
                    }
                }
            }

            return "";
        }

        string verificaRegraLinha(string regra) {
            
            if (regra.size() > 1 && regra[1] == '\'') {

                return regra.substr(0, 2);
            } 
            else {

                return string(1, regra[0]); 
            }
        }

        void substituirTerminais() {
            // Percorre todas as produções da gramática
            for (auto& producao : producoes) {
                
                for (auto& regra : regras(producao)) {

                    if (regra.size() > 1) {
                        
                        for (int i = 0; i < regra.size(); ++i) {

                            if(contains(alfabeto, regra[i])) {

                                string novaV = gerarNovaVariavel(string(1, regra[i]));

                                if(!contains(variaveis, novaV)) {

                                    producoes[novaV].push_back(string(1, regra[i]));
                                    variaveis.push_back(novaV);
                                }

                                regra.replace(i, 1, novaV);
                            }
                        }
                    }
                }
            }
        }

        void dividirRegras() {
            vector<pair<string, string>> novasProducoes; // Armazena as novas produções temporariamente

            // Divide as regras em tamanho 2
            for (auto& producao : producoes) {
                //cout << " Entrei aqui" << endl;
                vector<string> regrasAtualizadas;

                for (auto& regra : regras(producao)) {
                    
                    if (contarSimbolos(regra) > 2) {
                        
                        string primeiraParte = verificaRegraLinha(regra); // Verifica regra tipo A ou A'
                        string segundaParte = regra.substr(primeiraParte.size()); // Resto da regra ABC

                        string novaVar = verificaSeRegraTExiste(segundaParte, novasProducoes);

                        if (novaVar.empty()) {
                            
                            novaVar = gerarNovaVariavelT();

                            novasProducoes.push_back({novaVar, segundaParte});
                        }

                        regra = primeiraParte + novaVar;
                    }
                    if(!contains(regrasAtualizadas, regra)) {
                        
                        regrasAtualizadas.push_back(regra);
                    }
                }

                // Atualiza as regras da produção com as regras divididas
                producoes[variavel(producao)] = regrasAtualizadas;
            }

            for (auto& producao : novasProducoes) {

                producoes[producao.first].push_back(producao.second);
            }
        }

        // Função principal para converter a gramática para Forma Normal de Chomsky
        void converterParaFormaNormalDeChomsky() {

            //Remove recursão no simbolo inicial
            remRecSimboloInicial();
            
            //Gera anulaveis, cria novas producoes e remove lambda
            vector<string> anulaveis = variaveisAnulaveis();
            novasProducoes(anulaveis);
            removeProducaoLambda(anulaveis);

            //Regra da cadeia (chain)
            substituirCadeias();

            //Regra Terminais (TERM)
            vector<string> terminais = construirTerm();
            removeNaoTerminais(terminais);

            //Regra Alcançaveis (REACH)
            vector<string> alcancaveis = construirReach();
            removeNaoAlcancaveis(alcancaveis);

            //Chomsky
            substituirTerminais();
            dividirRegras();
        }

        void imprime() {

            for (auto& producao : producoes) {
                    
                cout << variavel(producao) << " -> ";

                auto& regrasV = regras(producao);
                for (int i = 0; i < regrasV.size(); i++) {

                    cout << regrasV[i];

                    if(i < regrasV.size() - 1) cout << " | ";
                }
                cout << endl;
                
            }
        }

        void gravarArquivo(char* nomeArquivo) {

            ofstream saida(nomeArquivo);

            // Primeiro, imprime a produção do símbolo inicial
            saida << simboloInicial << " -> ";
            auto& regra = producoes[simboloInicial];
            for (int i = 0; i < regra.size(); i++) {

                saida << regra[i];

                if(i < regra.size() - 1) saida << " | ";
            }
            saida << endl;

            // Agora imprime as demais produções
            for (auto& producao : producoes) {

                if (producao.first != simboloInicial) {

                    saida << variavel(producao) << " -> ";

                    auto& regrasV = regras(producao);
                    for (int i = 0; i < regrasV.size(); i++) {

                        saida << regrasV[i];

                        if(i < regrasV.size() - 1) saida << " | ";
                    }
                    saida << endl;
                }
            }
            saida.close();
        }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo_entrada> <nome_do_arquivo_saida>" << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]);

    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    string linha;


    string sInicial;
    vector<char> alfabeto;
    vector<string> variaveis;
    map<string, vector<string>> producoes;

    while (getline(arquivoEntrada, linha)) {
        stringstream s(linha);
        string c;

        s >> c;
        // Lado esquerdo (variável não terminal)
        string variavel = c;

        auto it = find(variaveis.begin(), variaveis.end(), variavel);
        if (it == variaveis.end())
            variaveis.push_back(variavel);

        while (s >> c) {
            if (c == " " || c == "|" || c == "-" || c == ">")
                continue;

            string producao = c;

            if (producao == "->")
                producao = "";
            while (s >> c && (c != " " && c != "|")) {
                producao += c;
            }

            producoes[variavel].push_back(producao);
        }

        for (auto& ch : linha) {
            // Verifica se o caractere é uma letra (minúscula ou maiúscula)
            if (isalpha(ch) && islower(ch)) {

                if (!contains(alfabeto, ch)) {

                    alfabeto.push_back(ch);
                }
            }
        }
    }
    arquivoEntrada.close();

    //Crio objeto gramatica passando os parametros necessários
    Gramatica gramatica(variaveis[0], alfabeto, variaveis, producoes);
    
    cout << "Conversão para Forma Normal de Chomsky:" << endl;
    cout << "-------------------------------------------" << endl;

    gramatica.converterParaFormaNormalDeChomsky();
    
    gramatica.imprime();
    gramatica.gravarArquivo(argv[2]);
    
    cout << "Gramática salva no arquivo " << argv[2] << " com sucesso!" << endl;

    return 0;
}