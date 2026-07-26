/*
 * Passeio do Cavalo - Sebatian Spessatto
 * Disciplina: Complexidade de Algoritmos
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int N = 8; 

//Movimentos cav
const int movX[8] = { 2,  1, -1, -2, -2, -1,  1,  2 };
const int movY[8] = { 1,  2,  2,  1, -1, -2, -2, -1 };

//Posicao 
bool posicaoValida(int x, int y, const vector<vector<int>>& tabuleiro) {
    return (x >= 0 && x < N && y >= 0 && y < N && tabuleiro[x][y] == -1);
}

//Ordem de visita de cada casa
void imprimirTabuleiro(const vector<vector<int>>& tabuleiro) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(3) << tabuleiro[i][j] + 1 << " ";
        }
        cout << "\n";
    }
}

// Tour aberto ou fechado
bool tourFechado(int ultX, int ultY, int inicioX, int inicioY) {
    for (int i = 0; i < 8; i++) {
        int nx = ultX + movX[i];
        int ny = ultY + movY[i];
        if (nx == inicioX && ny == inicioY) return true;
    }
    return false;
}


long long passosTestados = 0; // conta quantas tentativas de movimento foram feitas

const long long LIMITE_TENTATIVAS = 50000000000LL; // 50 bilhoes 
bool limiteEstourado = false;

bool backtrackingCavalo(int x, int y, int passoAtual, vector<vector<int>>& tabuleiro) {
    if (passoAtual == N * N) return true; // todas as casas visitadas
    if (limiteEstourado) return false;

    for (int i = 0; i < 8; i++) {
        passosTestados++;
        if (passosTestados > LIMITE_TENTATIVAS) { limiteEstourado = true; return false; }
        int proxX = x + movX[i];
        int proxY = y + movY[i];

        if (posicaoValida(proxX, proxY, tabuleiro)) {
            tabuleiro[proxX][proxY] = passoAtual;

            if (backtrackingCavalo(proxX, proxY, passoAtual + 1, tabuleiro))
                return true;

            // backtrack
            tabuleiro[proxX][proxY] = -1;
        }
    }
    return false;
}

// Segundo Algoritmo 
int contarMovimentosPossiveis(int x, int y, const vector<vector<int>>& tabuleiro) {
    int contador = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + movX[i];
        int ny = y + movY[i];
        if (posicaoValida(nx, ny, tabuleiro)) contador++;
    }
    return contador;
}

bool warnsdorffCavalo(int x, int y, vector<vector<int>>& tabuleiro) {
    tabuleiro[x][y] = 0;

    for (int passo = 1; passo < N * N; passo++) {
        int melhorX = -1, melhorY = -1;
        int menorGrau = 9; // maior que o máximo possível (8)

        // avalia as 8 direções e escolhe a de menor grau (
        for (int i = 0; i < 8; i++) {
            int nx = x + movX[i];
            int ny = y + movY[i];
            if (posicaoValida(nx, ny, tabuleiro)) {
                int grau = contarMovimentosPossiveis(nx, ny, tabuleiro);
                if (grau < menorGrau) {
                    menorGrau = grau;
                    melhorX = nx;
                    melhorY = ny;
                }
            }
        }

        if (melhorX == -1) return false; // ficou sem movimento -> tour incompleto

        tabuleiro[melhorX][melhorY] = passo;
        x = melhorX;
        y = melhorY;
    }
    return true;
}


int main(int argc, char* argv[]) {
    int inicioX, inicioY;

    cout << "=== Passeio do Cavalo - Complexidade de Algoritmos ===\n\n";
    cout << "Tabuleiro " << N << "x" << N << " (linhas e colunas de 0 a " << N - 1 << ")\n";

    if (argc >= 3) {
        inicioX = atoi(argv[1]);
        inicioY = atoi(argv[2]);
        cout << "Posicao inicial (via argumento): " << inicioX << " " << inicioY << "\n";
    } else {
        cout << "Digite a posicao inicial do cavalo (linha coluna), ex: 0 0: ";
        cin >> inicioX >> inicioY;
    }

    if (inicioX < 0 || inicioX >= N || inicioY < 0 || inicioY >= N) {
        cout << "Posicao invalida!\n";
        return 1;
    }

    // -------- Algoritmo 1: Tentativa e erro --------
    {
        vector<vector<int>> tabuleiro(N, vector<int>(N, -1));
        tabuleiro[inicioX][inicioY] = 0;
        passosTestados = 0;

        auto inicio = high_resolution_clock::now();
        bool sucesso = backtrackingCavalo(inicioX, inicioY, 1, tabuleiro);
        auto fim = high_resolution_clock::now();
        double tempoMs = duration_cast<duration<double, milli>>(fim - inicio).count();

        cout << "\n--- Algoritmo 1: Tentativa e Erro (Backtracking) ---\n";
        if (sucesso) {
            imprimirTabuleiro(tabuleiro);
            cout << "Passos (casas visitadas): " << N * N << "\n";
            cout << "Tentativas de movimento testadas: " << passosTestados << "\n";
            cout << "Tempo de execucao: " << tempoMs << " ms\n";

            // procura a posicao final (valor N*N - 1) para checar se o tour e fechado
            int ultX = -1, ultY = -1;
            for (int i = 0; i < N && ultX == -1; i++)
                for (int j = 0; j < N; j++)
                    if (tabuleiro[i][j] == N * N - 1) { ultX = i; ultY = j; break; }

            bool fechado = tourFechado(ultX, ultY, inicioX, inicioY);
            cout << "Tipo de solucao: " << (fechado ? "FECHADA" : "ABERTA") << "\n";
        } else {
            if (limiteEstourado) {
                cout << "Limite de " << LIMITE_TENTATIVAS << " tentativas atingido sem terminar.\n";
                cout << "Isso mostra na pratica a explosao combinatoria do backtracking puro\n";
                cout << "para certas posicoes iniciais (ex.: proximas ao centro do tabuleiro).\n";
            } else {
                cout << "Nao foi possivel encontrar uma solucao a partir dessa posicao.\n";
            }
            cout << "Tentativas de movimento testadas: " << passosTestados << "\n";
            cout << "Tempo de execucao: " << tempoMs << " ms\n";
        }
    }

    // -------- Algoritmo 2: Heuristica de Warnsdorff --------
    {
        vector<vector<int>> tabuleiro(N, vector<int>(N, -1));

        auto inicio = high_resolution_clock::now();
        bool sucesso = warnsdorffCavalo(inicioX, inicioY, tabuleiro);
        auto fim = high_resolution_clock::now();
        double tempoMs = duration_cast<duration<double, milli>>(fim - inicio).count();

        cout << "\n--- Algoritmo 2: Heuristica de Warnsdorff ---\n";
        if (sucesso) {
            imprimirTabuleiro(tabuleiro);
            cout << "Passos (casas visitadas): " << N * N << "\n";
            cout << "Tempo de execucao: " << tempoMs << " ms\n";

            int ultX = -1, ultY = -1;
            for (int i = 0; i < N && ultX == -1; i++)
                for (int j = 0; j < N; j++)
                    if (tabuleiro[i][j] == N * N - 1) { ultX = i; ultY = j; break; }

            bool fechado = tourFechado(ultX, ultY, inicioX, inicioY);
            cout << "Tipo de solucao: " << (fechado ? "FECHADA" : "ABERTA") << "\n";
        } else {
            cout << "A heuristica ficou presa antes de completar o tabuleiro.\n";
            cout << "Tempo de execucao: " << tempoMs << " ms\n";
        }
    }

    return 0;
}
