# Trabalho-para-disciplina-de-Linguagens-Formais-e-Automatos
Programa que coloca uma GLC na FNC.

Para compilar:
> g++ -Wall fnc.cpp -o fnc

Chamada deve ser por linha de comando:
> ./fnc glc1Entrada.txt glc1Saida.txt

As produções devem considerar os seguintes padrões:
> – Variáveis: [A-Z]
> – Terminais: [a-z]
> – Operador de definição: ->
> – Separador de regras: |
> – Lambda: .

Teste as seguintes gramáticas:
> S -> ABC
> A -> aA | .
> B -> bB | .
> C -> cC | .

> S -> aS | bS | C | D
> C -> c | .
> D -> abc
> D -> .

> S -> aAa
> A -> ABC
> B -> cC | Cc | CC
> C -> aba | .

> S -> ACA | CA | AA | AC | A | C | .
> A -> aAa | aa | B | C
> B -> bB | b
> C -> cC | c

S -> Aa | bB | AAA | C    
A -> BD | cC | CB
B -> BbB | aB | bBD | CB
C -> aA | b | .
D -> AcC | cA | .
