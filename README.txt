Bem-vindo à cacheCool:

Para criar o executável da cacheCool digite o seguinte comando:
g++ -g cacheCool.cpp -lm -o cacheCool

E para inicializar o executável digite:
./cacheCool <nsets> <bsize> <assoc> <substituição> <flag_saida> arquivo_de_entrada

Ex.: ./cacheCool 64 8 1 R 0 bin_100.bin

Atualizações no código pós apresentação:
  - trocamos o for do mapeamento direto por contador (antes percorríamos a cache toda, verificando se cache[indice]==0)
  - unimos Totalmente Associativa com Conjunto Associativa
  - Na conj.Assoc: separamos o miss global dos for's dos misses específicos (compulsório, conflito e capacidade)
  - trocamos a lista que verificava miss de capacidade por um contador. (antes tinhamos uma lista de tam de assoc com 1's nas posições ocupadas e verificávamos se a lista estava cheia para ser miss de capacidade)
  - colocamos um break em cada final de for, para que, depois que a cache ache seu hit ou miss, não fique comparando com os restos dos valores dos outros conjuntos. (antes estávamos contando miss de conflito e compulsório tudo na mesma escrita)

Obstáculos persistentes:
- para caches grandes temos uma taxa de hit muito alta. Mais de 99%, o que não é condizente com os resultados. 
Por causa disso, os misses só começam a aparecer lá pela 3⁰-5⁰ casa decimal. 
Assim, trocamos a impressão *flag 0* para 5 casas decimais, pois era livre, mas não trocamos a de *flag 1* pois já era estipulada nos requisitos do trabalho como contendo 2 casas decimais.
- Apesar das mudanças não notamos diferença nos resultados testados. Uma possível razão é a inutilização das variáveis de tamanho de bloco neste código.
