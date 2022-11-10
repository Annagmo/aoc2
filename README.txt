Bem-vindo à cacheCool:

Para criar o executável da cacheCool digite o seguinte comando:
g++ -g cacheCool.cpp -lm -o cacheCool

E para inicializar o executável digite:
./cacheCool <nsets> <bsize> <assoc> <substituição> <flag_saida> arquivo_de_entrada

Ex.: ./cacheCool 64 8 1 R 0 bin_100.bin

Atualizações no código pós apresentação:
  - trocamos o for do mapeamento direto por contador (antes percorríamos a cache toda, verificando se cache[indice]==0)
  - unimos Tot.Assoc. com Conj.Assoc.
  - separamos o miss global dos for's dos miss específicos (compulsório, conflito e capacidade)
  - trocamos a lista que verificava miss de capacidade por um contador. (antes tinhamos uma lista de tam de assoc com 1's nas posições ocupadas e verificávamos se a lista estava cheia para ser miss de capacidade)
