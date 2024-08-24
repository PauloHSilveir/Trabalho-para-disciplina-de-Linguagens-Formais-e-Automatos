#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <cmath>
using namespace std;

class FNC {
    private:
        string simboloInicial;
        vector<char> alfabeto;
        list<string> variaveis;
        unordered_map<string, vector<string>> producoes;


    public:
        FNC (string sInicial, vector<char> alfabeto, list<string> variaveis, unordered_map<string, vector<string>> gramatica) {
            this->simboloInicial = sInicial;
            this->alfabeto = alfabeto;
            this->variaveis = variaveis;
            producoes = gramatica;
        }
        /*void addProducao(string variavel, string producao) {
            producoes[variavel].push_back(producao);
        }*/
        void remRecSimboloInicial() {
            auto& producoesVec = producoes[simboloInicial];
            
            for(const string& producao : producoesVec) {

                auto it = find(producao.begin(), producao.end(), simboloInicial[0]);
                if(it != producao.end()) {
                    string novoInicial = simboloInicial + "'";
                    variaveis.push_back(novoInicial);
                    producoes[novoInicial].push_back(simboloInicial);
                    simboloInicial = novoInicial;
                    break;
                }
            }
        }
        vector<string> variaveisAnulaveis() {
            vector<string> anulaveis;
            vector<string> anterior;

            for(const auto& producao : producoes) {
                for(const auto& simbolo : producao.second) {
                    if(simbolo == ".") {
                        anulaveis.push_back(producao.first);
                        break;
                    }
                }
            }
            
            do {
                anterior = anulaveis;

                for(const auto& lProducao : producoes) {
                    
                    auto it = find(anulaveis.begin(), anulaveis.end(), lProducao.first);
                    if (it != anulaveis.end()) {
                        continue;
                    }
                    
                    for(const auto& producao : lProducao.second) {
                        if(producao == ".") {
                            continue;
                        }

                        bool ehAnulavel = true;
                        for(const auto& simbolo : producao) {

                            if (find(anulaveis.begin(), anulaveis.end(), string(1, simbolo)) == anulaveis.end()) {
                                ehAnulavel = false;
                                break;
                            }
                        }

                        if(ehAnulavel) {
                            anulaveis.push_back(lProducao.first);
                            break;
                        }
                    }
                }
            } while (anulaveis.size() != anterior.size());

            cout << "Variáveis anulaveis:" << endl;
            for (const auto& var : anulaveis) {
                cout << var << endl;
            }
            return anulaveis;
        }
        void novasProducoes( const vector<string>& anulaveis) {
            unordered_map<string, vector<string>> novasProducoes = producoes;

            for (const auto& lProducao : producoes) {
                for (const auto& producao : lProducao.second) {
                    int n = producao.size();
                    // Itera sobre todos os subconjuntos de símbolos na produção
                    for (int mask = 0; mask < (1 << n); ++mask) {
                        string novaProducao = "";

                        for (int i = 0; i < n; ++i) {
                            char simbolo = producao[i];
                            string simboloStr(1, simbolo);

                            // Se o símbolo não é anulável ou se o bit correspondente está setado
                            if (find(anulaveis.begin(), anulaveis.end(), simboloStr) == anulaveis.end() || (mask & (1 << i))) {
                                novaProducao += simbolo;
                            }
                        }
                        //cout << "add novaProducao: " << novaProducao << " ";
                        // Adiciona a nova produção se for válida
                        // Se a nova produção é vazia e o símbolo é o inicial, adiciona a produção '.'
                        if (novaProducao.empty() && lProducao.first == simboloInicial) {
                            novaProducao = ".";
                        }
                        if (!novaProducao.empty() && novaProducao != producao) {
                            auto it = find(novasProducoes[lProducao.first].begin(), novasProducoes[lProducao.first].end(), novaProducao);
                            if (it == novasProducoes[lProducao.first].end()) {
                                novasProducoes[lProducao.first].push_back(novaProducao);
                            }
                        }
                    }
                }
            }
            producoes = novasProducoes;
        }

        /*vector<vector<string>> conjuntoDasPartes(const vector<string>& anulaveis) {
            vector<vector<string>> conjuntoDasPartes;
            int tamAnulaveis = anulaveis.size();
            int tamConjPartes = pow(2, tamAnulaveis); // 2^n combinações

            // Gera todas as combinações possíveis
            for (int i = 0; i < tamConjPartes; ++i) {
                vector<string> subConjunto;

                // Verifica cada bit do contador
                for (int j = 0; j < tamAnulaveis; ++j) {
                    if (i & (1 << j)) { // Se o j-ésimo bit está definido
                        subConjunto.push_back(anulaveis[j]);
                    }
                }

                conjuntoDasPartes.push_back(subConjunto);
            }

            return conjuntoDasPartes;
        }*/

        void removeProducaoLambda(vector<string> anulaveis) {
            for(auto& lProducao : producoes) {

                if(lProducao.first != simboloInicial) {

                    auto& producoesVec = lProducao.second;
                    producoesVec.erase(
                    remove(producoesVec.begin(), producoesVec.end(), "."),
                    producoesVec.end());
                    producoesVec.erase(
                    remove(producoesVec.begin(), producoesVec.end(), lProducao.first),
                    producoesVec.end());
                    //lProducao.second.remove(".");
                }
            }
        }
        vector<string> construirChain(const string& variavel) {
            vector<string> chainSet;
            vector<string> prevSet;

            chainSet.push_back(variavel);

            do {
                vector<string> newSet = chainSet;
                
                //novo conjunto passa a ser chain(Var) - Prev
                for (const auto& elem2 : prevSet) {
                    for (int i = 0; i < newSet.size(); ++i) {
                        if (newSet[i] == elem2) {
                            newSet.erase(newSet.begin() + i); // Remova o elemento igual de arr3
                            break; // Não precisa continuar o loop
                        }
                    }
                }
                prevSet = chainSet;

                for (const auto& B : newSet) {
                    if (producoes.find(B) != producoes.end()) {
                        for (const auto& producao : producoes.at(B)) {
                            // Se a produção é uma única variável, adiciona ao conjunto se ainda não estiver nele
                            if (producao.size() == 1 && isupper(producao[0]) && 
                                find(chainSet.begin(), chainSet.end(), producao) == chainSet.end()) {
                                chainSet.push_back(producao);
                            }
                        }
                    }
                }
                //chainSet = newSet;

            } while (chainSet != prevSet);
            cout << "chain de cada variavel\n";
            for (int i = 0; i < chainSet.size(); i++)
            {
                cout << chainSet[i] << " ";
            }
            cout << endl;
            return chainSet;
        }

        void substituirCadeias() {
            // Substituir as cadeias pelas produções equivalentes
            for (auto& producao : producoes) {
                vector<string> chainVec = construirChain(producao.first);

                vector<string> novasProducoes;

                // Adiciona as produções equivalentes das cadeias
                for (const auto& regra : chainVec) {
                    for (const auto& prod : producoes[regra]) {
                        if (find(novasProducoes.begin(), novasProducoes.end(), prod) == novasProducoes.end()) {
                            novasProducoes.push_back(prod);
                        }
                    }
                }

                producao.second = novasProducoes;
            }

            // Remove todas as cadeias 
            for(auto& producao : producoes) {
                auto& regras = producao.second;
                for(auto it = regras.begin(); it != regras.end();) {
                    if(it->size() == 1 && isupper((*it)[0])) {
                        it = regras.erase(it); // Remove a regra e avança o iterador
                    } else {
                        ++it; // Avança o iterador normalmente
                    }
                }
            }
        }
        // vector<string> construirTerm() {
        //     vector<string> TERM;

        //     // Passo inicial: adiciona variáveis que geram diretamente terminais
        //     for (const auto& producao : producoes) {
        //         for (const auto& p : producao.second) {
        //             bool geraTerminal = true;
        //             for (char ch : p) {
        //                 if (!islower(ch)) { // Verifica se todos os caracteres são terminais (minúsculos)
        //                     geraTerminal = false;
        //                     break;
        //                 }
        //             }
        //             if (geraTerminal) {
        //                 TERM.push_back(producao.first);
        //                 break;
        //             }
        //         }
        //     }

        //     vector<string> PREV;

        //     do {
        //         PREV = TERM;


        //         for (const auto& producao : producoes) {
        //             const string& A = producao.first;

        //             for (const auto& w : producao.second) {
        //                 bool pertenceAoTERM = true;
        //                 for (char ch : w) {
        //                     string s(1, ch);
        //                     if (find(PREV.begin(), PREV.end(), s) == PREV.end() && 
        //                         find(alfabeto.begin(), alfabeto.end(), s) == alfabeto.end()) {
        //                         pertenceAoTERM = false;
        //                         break;
        //                     }
        //                 }
        //                 if (pertenceAoTERM) {
        //                     if (find(TERM.begin(), TERM.end(), A) == TERM.end()) {
        //                         TERM.push_back(A);
        //                     }
        //                     break;
        //                 }
        //             }
        //         }

        //     } while (PREV != TERM);
        //     cout << "Esse é o term porra \n ";
        //     for(auto& var:TERM) {
        //         cout << var << " ";

        //     }
        //     cout << endl;
        //     return TERM;
        // }
        vector<string> construirTerm() {
            vector<string> TERM;
            // Passo inicial: adiciona variáveis que geram diretamente terminais
            for (const auto& producao : producoes) {
                for (const auto& p : producao.second) {
                    bool geraTerminal = true;
                    for (char ch : p) {
                        if (!islower(ch)) { // Verifica se todos os caracteres são terminais (minúsculos)
                            geraTerminal = false;
                            break;
                        }
                    }
                    if (geraTerminal) {
                        TERM.push_back(producao.first);
                        break;
                    }
                }
            }
            if(TERM.size() == variaveis.size()) {
                cout << endl << "Todas variáveis da gramática geram terminais." << endl;
            } 
            else {
                vector<string> PREV;

                do {
                    PREV = TERM;

                    for (const auto& producao : producoes) {
                        const string& A = producao.first;

                        for (const auto& w : producao.second) {
                            bool pertenceAoTERM = true;
                            for (char ch : w) {
                                string s(1, ch);
                                if (find(PREV.begin(), PREV.end(), s) == PREV.end() && 
                                    find(alfabeto.begin(), alfabeto.end(), ch) == alfabeto.end()) {
                                    pertenceAoTERM = false;
                                    break;
                                }
                            }
                            if (pertenceAoTERM) {
                                if (find(TERM.begin(), TERM.end(), A) == TERM.end()) {
                                    TERM.push_back(A);
                                }
                                break;
                            }
                        }
                    }

                } while (PREV != TERM);
            }
            cout << "Esse é o term porra \n ";
            for(auto& var:TERM) {
                cout << var << " ";

            }
            cout << endl;
            return TERM;
        }
        void removeNaoTerminais(vector<string> terminais) {
            vector<string> naoTerminais;
            //encontra as variaveis não terminais
            for(auto& var : variaveis) {
                if(find(terminais.begin(), terminais.end(), var) == terminais.end()) {
                    naoTerminais.push_back(var);
                }
            }
            // cout << "Esse é os não terminais \n ";
            // for(auto& var:naoTerminais) {
            //     cout << var << " ";

            // }
            // cout << endl;
            
            // Remove produções associadas às variáveis não terminais
            for (auto it = producoes.begin(); it != producoes.end();) {
                
                // Verifica cada regra da produção para remover regras contendo não terminais
                auto& regras = it->second;
                for (auto regraIt = regras.begin(); regraIt != regras.end();) {
                    bool temNaoTerminal = false;
                    for (const auto& ch : *regraIt) {
                        // Verifica se o caractere pertence ao conjunto de não terminais
                        if (find(naoTerminais.begin(), naoTerminais.end(), string(1, ch)) != naoTerminais.end()) {
                            temNaoTerminal = true;
                            break;
                        }
                    }
                    if (temNaoTerminal) {
                        regraIt = regras.erase(regraIt); // Remove a regra e avança o iterador
                    } else {
                        ++regraIt; // Avança o iterador normalmente
                    }
                }
                // Se não houver mais regras para a produção, remove a produção inteira
                if (regras.empty()) {
                    it = producoes.erase(it);
                } else {
                    ++it; // Avança o iterador normalmente
                }
            }
        }

        void imprime() {
            
            for (const auto& variavel : producoes) {
                cout << "Produções para " 
                     << variavel.first << ": ";
                for (const string& producao : variavel.second) {
                    cout << producao << "  ";
                }
                cout << endl;
            }
        }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]);

    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    string linha;
    /*int contadorDeLinhas = 0;

    while (getline(arquivoEntrada, linha)) {
        contadorDeLinhas++;
    }
    arquivoEntrada.close();*/

    string sInicial;
    vector<char> alfabeto;
    list<string> variaveis;
    unordered_map<string, vector<string>> producoes;

    while (getline(arquivoEntrada, linha)) {
        stringstream s(linha);
        string c;

        s >> c;
        // Lado esquerdo (variável não terminal)
        string variavel = c;

        auto it = find(variaveis.begin(), variaveis.end(), variavel);
        if(it == variaveis.end())
            variaveis.push_back(variavel);
        
        while (s >> c) {
            if (c == " " || c == "|" || c == "-" || c == ">")
                continue;

            string producao = c;

            if(producao == "->")
                producao = "";
            while (s >> c && (c != " " && c != "|")) {
                producao += c;
            }
        
            //fnc.addProducao(variavel, producao);
            producoes[variavel].push_back(producao);
        }

            for(auto& ch : linha) {
                if(ch == 'a' || ch == 'b' || ch == 'c') {
                    auto it = find(alfabeto.begin(), alfabeto.end(), ch);
                    if(it == alfabeto.end())
                        alfabeto.push_back(ch);
                }
            }
        
    }
    arquivoEntrada.close();

    sInicial = variaveis.front();
    cout << sInicial << endl;
    for(char& letra:alfabeto) {
        cout << letra << " ";
    }
    cout << endl;
    for(string& var :variaveis) {
        cout << var << " ";
    }
    cout << endl;

    for (const auto& variavel : producoes) {
        cout << "Produções para " << variavel.first << ": ";
        for (const string& producao : variavel.second) {
            cout << producao << "  ";
        }
        cout << endl;
    }

    FNC fnc(sInicial, alfabeto, variaveis, producoes);
    fnc.remRecSimboloInicial();
    fnc.imprime();
    vector<string> anulaveis = fnc.variaveisAnulaveis();
    fnc.novasProducoes(anulaveis);

    fnc.imprime();

    
    fnc.removeProducaoLambda(anulaveis);
    cout << "Produções após remoção de λ:" << endl;
    cout << "\n///////////////////////////////////////////////////\n";
    fnc.imprime();

    fnc.substituirCadeias();
    cout << "Regras chain" << endl;
    cout << "\n///////////////////////////////////////////////////\n";
    fnc.imprime();

    vector<string> Terminais = fnc.construirTerm();
    cout << "Terminais" << endl;
    cout << "\n///////////////////////////////////////////////////\n";
    fnc.removeNaoTerminais(Terminais);
    fnc.imprime();

    /*vector<vector<string>> power_set = fnc.conjuntoDasPartes(anulaveis);

    cout << "Conjunto das partes: " << endl;
    for (const auto& subset : power_set) {
        std::cout << "{ ";
        for (string elem : subset) {
            cout << elem << " ";
        }
        cout << "}" << std::endl;
    }*/

    return 0;
}