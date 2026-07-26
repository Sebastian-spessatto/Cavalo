# Passeio do Cavalo (Knight's Tour)

Trabalho da disciplina **Complexidade de Algoritmos** — Curso de Ciência da Computação, Escola do Mar, Ciência e Tecnologia, Universidade do Vale do Itajaí (UNIVALI).

## Descrição do problema

O Passeio do Cavalo é um problema clássico sobre um tabuleiro de xadrez 8x8: partindo de uma posição inicial qualquer, o cavalo deve visitar **todas as 64 casas exatamente uma vez**, movendo-se sempre segundo o movimento em "L" característico da peça.

Uma solução é dita:
- **Aberta**, quando o cavalo visita todas as casas mas não consegue retornar à posição inicial com um último movimento válido;
- **Fechada**, quando, a partir da última casa visitada, existe um movimento de cavalo que leva de volta à posição inicial (formando um ciclo).

## Algoritmos implementados

### 1. Tentativa e erro (backtracking puro)

Tenta os 8 movimentos possíveis sempre na mesma ordem fixa. Quando não há mais movimento válido e o tabuleiro ainda não está completo, desfaz o último passo (*backtrack*) e tenta a próxima opção. É uma solução de força bruta: garante encontrar uma solução se ela existir, mas seu desempenho depende fortemente da posição inicial e pode crescer exponencialmente no pior caso.

Por conta disso, o código tem um **limite de segurança de tentativas** (`LIMITE_TENTATIVAS`, atualmente configurado em 50 bilhões). Quando esse limite é atingido antes de o tabuleiro ser completado, a execução é interrompida e reportada como **TIMEOUT** nos resultados, isso não significa que não exista solução a partir daquela posição, apenas que o backtracking não conseguiu encontrá-la dentro do limite de tentativas configurado.

### 2. Heurística de Warnsdorff (menor possibilidade de movimento)

A cada passo, avalia todas as casas vizinhas alcançáveis e escolhe aquela que possui a **menor quantidade de movimentos futuros possíveis**. A ideia é visitar primeiro as casas mais "restritas" (cantos e bordas), deixando para depois as casas com mais opções em aberto. Essa estratégia gulosa reduz drasticamente o número de tentativas necessárias e, na prática, encontra uma solução completa em tempo praticamente constante, sem precisar de backtracking na maioria dos casos.

## Métricas coletadas

Para cada posição inicial testada, o programa mede:
- **Tempo de execução** (em milissegundos), via `std::chrono`;
- **Quantidade de passos**: 64 quando a solução é encontrada; e, no caso do backtracking, também o número total de tentativas de movimento realizadas (incluindo as desfeitas);
- **Tipo de solução**: aberta ou fechada, verificado ao final checando se o último movimento consegue retornar à casa inicial.

## Como compilar e executar

```bash
g++ -O2 -std=c++17 -o passeio_cavalo passeio_cavalo.cpp
./passeio_cavalo
```

O programa solicita a posição inicial do cavalo (linha e coluna, de 0 a 7) e roda os dois algoritmos em sequência, imprimindo o tabuleiro final, o tempo de execução, a quantidade de passos e o tipo de solução para cada um.

## Observações sobre os resultados

Um ponto importante ao comparar resultados entre equipes: **o número de tentativas do backtracking depende diretamente da ordem em que os 8 movimentos do cavalo são testados no código de cada implementação**. Por isso, é normal que a mesma posição inicial produza tempos e quantidades de tentativas bem diferentes entre implementações distintas — não existe um valor "correto" universal, o que importa é a tendência observada: posições mais centrais tendem a gerar árvores de busca maiores (e por isso mais TIMEOUTs) do que posições em cantos ou bordas, enquanto a heurística de Warnsdorff se mantém rápida (frações de milissegundo) independentemente da posição inicial.

Essa diferença de comportamento entre os dois algoritmos é justamente a conclusão central do trabalho: ilustrar, na prática, por que estratégias heurísticas são necessárias para tornar o problema do Passeio do Cavalo tratável computacionalmente em qualquer posição inicial, enquanto a força bruta pura só é viável em casos favoráveis.

## Equipe

- (adicionar nomes dos integrantes aqui)
