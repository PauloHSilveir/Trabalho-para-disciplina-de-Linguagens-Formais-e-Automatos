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
<p>S -> aS | bS | C | D</p>
<p>C -> c | .</p>
<p>D -> abc</p>
<p>D -> .</p>
<br>
<p>S -> aAa</p>
<p>A -> ABC</p>
<p>B -> cC | Cc | CC</p>
<p>C -> aba | .</p>
<br>
<p>S -> Aa | bB | AAA | C</p>
<p>A -> BD | cC | CB</p>
<p>B -> BbB | aB | bBD | CB</p>
<p>C -> aA | b | .</p>
<p>D -> AcC | cA | .</p>
<br>
