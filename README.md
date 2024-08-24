# Trabalho-para-disciplina-de-Linguagens-Formais-e-Automatos
Programa que coloca uma GLC na FNC.

Para compilar:
> g++ -Wall nome.cpp -o nome

Chamada deve ser por linha de comando:
> ./glc2fnc glc1.txt glc1 fnc.txt

As produções devem considerar os seguintes padrões:
– Variáveis: [A-Z]
– Terminais: [a-z]
– Operador de definição: ->
– Separador de regras: |
– Lambda: .

Testes de gramática:
S -> ABC
A -> aA | .
B -> bB | .
C -> cC | .

S -> aS | bS | C | D
C -> c | .
D -> abc
D -> .

S -> aAa
A -> ABC
B -> cC | Cc | CC
C -> aba | .

S -> ACA | CA | AA | AC | A | C | .
A -> aAa | aa | B | C
B -> bB | b
C -> cC | c

S -> Aa | bB | AAA | C    
A -> BD | cC | CB
B -> BbB | aB | bBD | CB
C -> aA | b | .
D -> AcC | cA | .
