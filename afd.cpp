#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

// Estrutura para representar o autômato finito determinístico (AFD)
struct AFD {
    int numEstados;                        // Número de estados
    set<char> alfabeto;                    // Conjunto de símbolos terminais
    int estadoInicial;                     // Estado inicial
    set<int> estadosAceitacao;             // Conjunto de estados de aceitação
    map<pair<int, char>, int> transicoes;  // Função de transição (δ)

    // Função que processa uma cadeia de entrada
    bool processarCadeia(const string &cadeia) {
        int estadoAtual = estadoInicial;
        for (char simbolo : cadeia) {
            if (alfabeto.find(simbolo) == alfabeto.end()) {
                return false; // Se o símbolo não estiver no alfabeto, rejeitar
            }
            auto transicao = transicoes.find({estadoAtual, simbolo});
            if (transicao == transicoes.end()) {
                return false; 
            }
            estadoAtual = transicao->second; 
        }
        return estadosAceitacao.find(estadoAtual) != estadosAceitacao.end();
    }
};


set<int> lerEstados(ifstream &entrada, int quantidade) {
    set<int> estados;
    for (int i = 0; i < quantidade; i++) {
        int estado;
        entrada >> estado;
        estados.insert(estado);
    }
    return estados;
}


set<char> lerAlfabeto(ifstream &entrada, int quantidade) {
    set<char> alfabeto;
    for (int i = 0; i < quantidade; i++) {
        char simbolo;
        entrada >> simbolo;
        alfabeto.insert(simbolo);
    }
    return alfabeto;
}

int main() {
    AFD automato;

    // Abrir arquivo de entrada e saída
    ifstream arquivoEntrada("./entrada.txt");
    ofstream arquivoSaida("saida.txt");

    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }
    if (!arquivoSaida.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return 1;
    }

    // Ler o número de estados
    arquivoEntrada >> automato.numEstados;
    cout << "Número de estados: " << automato.numEstados << endl;

    // Ler o alfabeto
    int quantidadeSimbolos;
    arquivoEntrada >> quantidadeSimbolos;
    automato.alfabeto = lerAlfabeto(arquivoEntrada, quantidadeSimbolos);
    cout << "Alfabeto lido: ";
    for (char simbolo : automato.alfabeto) {
        cout << simbolo << " ";
    }
    cout << endl;

    // Ler o estado inicial
    arquivoEntrada >> automato.estadoInicial;
    cout << "Estado inicial: " << automato.estadoInicial << endl;

    // Ler os estados de aceitação
    int quantidadeAceitacao;
    arquivoEntrada >> quantidadeAceitacao;
    automato.estadosAceitacao = lerEstados(arquivoEntrada, quantidadeAceitacao);
    cout << "Estados de aceitação: ";
    for (int estado : automato.estadosAceitacao) {
        cout << estado << " ";
    }
    cout << endl;

    // Ler as transições
    int quantidadeTransicoes;
    arquivoEntrada >> quantidadeTransicoes;
    cout << "Número de transições: " << quantidadeTransicoes << endl;
    for (int i = 0; i < quantidadeTransicoes; i++) {
        int estadoAtual, proximoEstado;
        char simbolo;
        arquivoEntrada >> estadoAtual >> simbolo >> proximoEstado;
        automato.transicoes[{estadoAtual, simbolo}] = proximoEstado;
        cout << "Transição lida: " << estadoAtual << " --" << simbolo << "--> " << proximoEstado << endl;
    }

   
    vector<string> cadeias;
    string cadeia;
    while (getline(arquivoEntrada, cadeia)) {
        // Ignorar linhas vazias
        if (!cadeia.empty() && cadeia != "-") {
            cadeias.push_back(cadeia);
        }
    }

    // Processar cada cadeia de entrada e gravar a saída correspondente
    for (size_t i = 0; i < cadeias.size(); i++) {
        cout << "Processando cadeia: " << cadeias[i] << endl;
        if (automato.processarCadeia(cadeias[i])) {
            arquivoSaida << (i + 1) << ". aceita" << endl;
            cout << (i + 1) << ". aceita" << endl;  
        } else {
            arquivoSaida << (i + 1) << ". rejeita" << endl;
            cout << (i + 1) << ". rejeita" << endl;  
        }
    }

    // Permitir ao usuário adicionar novas cadeias ao arquivo de entrada
    cout << "Deseja adicionar novas cadeias ao arquivo de entrada? (s/n): ";
    char resposta;
    cin >> resposta;
    if (resposta == 's' || resposta == 'S') {
        ofstream arquivoEntradaAppend("./entrada.txt", ios::app); // Abrir em modo append
        if (!arquivoEntradaAppend.is_open()) {
            cerr << "Erro ao abrir o arquivo de entrada para adicionar novas cadeias." << endl;
            return 1;
        }

        cout << "Digite as cadeias a serem adicionadas (digite 'fim' para encerrar):" << endl;
        cin.ignore(); 
        while (true) {
            string novaCadeia;
            getline(cin, novaCadeia);
            if (novaCadeia == "fim") {
                break;
            }
            arquivoEntradaAppend << novaCadeia << endl; 
        }

        arquivoEntradaAppend.close();
    }

  
    arquivoEntrada.close();
    arquivoSaida.close();

    return 0;
}
