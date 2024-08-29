# Trabalho-para-disciplina-de-Linguagens-Formais-e-Automatos
Programa que coloca uma GLC na FNC.

#Para compilar:
> g++ -Wall fnc.cpp -o fnc

#Chamada deve ser por linha de comando:
> ./fnc glc1Entrada.txt glc1Saida.txt

#As produções devem considerar os seguintes padrões:
<p>– Variáveis: [A-Z]</p> 
<p>– Terminais: [a-z]</p>
<p>– Operador de definição: -></p>
<p>– Separador de regras: |</p>
<p>– Lambda: .</p>

#Teste como entrada as seguintes gramáticas:
<p>S -> ABC</p>
<p>A -> aA | .</p>
<p>B -> bB | .</p>
<p>C -> cC | .</p>
<br>

> S -> aS | bS | C | D
> C -> c | .
> D -> abc
> D -> .

> S -> aAa
> A -> ABC
> B -> cC | Cc | CC
> C -> aba | .

> S -> Aa | bB | AAA | C    
> A -> BD | cC | CB
> B -> BbB | aB | bBD | CB
> C -> aA | b | .
> D -> AcC | cA | .
