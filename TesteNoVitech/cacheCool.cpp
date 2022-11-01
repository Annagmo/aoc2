#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

int substitui(char *sub, int assoc);
bool acha0(int *vetor,int tam);
static void Help(){
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	printf("\t\tBem-vindo à cacheCool:\n");
	printf(" Você digitou um número incorreto de argumentos. Utilize:\n");
	printf("./cacheCool <nsets> <bsize> <assoc> <substituição> <flag_saida> arquivo_de_entrada\n");
	printf("\n  Ex.: ./cacheCool 64 8 1 R 0 bin_100.bin\n");
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	exit(EXIT_FAILURE);
}

int main( int argc, char *argv[ ] )
{
	if (argc != 7){
		Help();
	}
	int nsets = atoi(argv[1]);
	int bsize = atoi(argv[2]);
	int assoc = atoi(argv[3]);
	char *subst = argv[4];
	int flagOut = atoi(argv[5]);
	char *arquivoEntrada = argv[6];

		int miss_compulsorio =0, n_bits_tag =0;
		int endereco = 0, tag =0, indice = 0, miss =0, hit =0, i =0, miss_conflito =0, miss_capacidade=0, qtd_acessos=0;
		float taxa_hit=0.0, taxa_miss=0.0, taxa_miss_comp=0.0,taxa_miss_conf=0.0, taxa_miss_cap=0.0;
		int n_bits_offset =0, n_bits_indice =0;
		int cache_val [nsets * assoc] = {0};  //é a lista dos bits de validade da cache
		int cache_tag [nsets * assoc] = {0};  //é a lista dos bits de tag da cache
		n_bits_offset = (log2(bsize));
		n_bits_indice = (log2(nsets));
		n_bits_tag = 32 - n_bits_offset - n_bits_indice;
		//----------------------------------------------------------
		//Variaveis para o conjunto associativo;
		typedef struct{
			int val;
			int tag;
			int info;
		}Cache; //cria uma struct de cache pq vai ter uma posicao em cada conjunto para o mesmo indice
		Cache cache[nsets * assoc][assoc] = {0}; //inicializa tudo em 0
		int CA_val_assoc[assoc] = {0};
		//----------------------------------------------------------
		//variaveis p/ o arquivo:
		FILE *fptr;
		fptr = fopen(arquivoEntrada, "rb"); //rb  para arquivos binarios.
		//----------------------------------------------------------

		if(fptr == NULL) {
			printf("Cannot open file \n");
			exit(0); //se não conseguir abrir o arquivo
		}
		while (!feof(fptr))
			qtd_acessos++;
			
			uint32_t mask = 0xffffffff >> n_bits_tag;
			tag = endereco >> ( n_bits_offset + n_bits_indice );
			indice = ( endereco & mask );
			indice = indice >> n_bits_offset;

			if(assoc == 1){ //Mapeamento Direto
			if (cache_val[indice] == 0)
			{
				miss++; 		
				miss_compulsorio++;
				cache_val[indice] = 1;
				cache_tag[indice] = tag;
			}
			else
				if (cache_tag[indice] == tag)
					hit++;
				else 			// a pos cheia e miss
				{
					miss++;
					
					// conflito ou capacidade?
					for(i=0; i < sizeof(cache_val); i++){
						if(cache_val[i] == 0){
							miss_conflito++;
							cache_val[indice] = 1;
							cache_tag[indice] = tag;
						}
						else{miss_capacidade++;
							cache_val[indice] = 1;
							cache_tag[indice] = tag;
							}
					}
				}
			} 
			// Totalmente associativo
			if (nsets == 1) {							// TA: n_conjuntos = tamanho da cachce 		      
				if (cache_val[indice] == 0) {					// Compulsory miss
					miss++; 
					miss_compulsorio++;
					cache_val[indice] = 1;					// Coloca os valores no local
					cache_tag[indice] = tag;
				} else { 
					if (cache_tag[indice] == tag) {                         // Hit
						hit++;
					} else {						// Capacity miss
						miss++; 
						miss_capacidade++;																 
						
						//indice = substitui(subst,assoc);          // Atribui o valor solicitado à posição escolhida 
						cache_val[indice] = 1;
						cache_tag[indice] = tag;  	
					}
				} 
			} 
			if (nsets != 1 && assoc > 1) { //Conjunto Associativo
				for(i=0; i<assoc; i++){
					if(cache[indice][i].info == endereco &&  cache[indice][i].tag == tag && cache[indice][i].val == 1 ){ //i representa o conjunto naquela posiçao. Ex.: Assoc = 2 temos cache[indice][0] e cache[indice][1]
						hit++;
						CA_val_assoc[i] = 1;
					}
					else if(cache[indice][i].val == 1){
						miss++;
						miss_conflito++;
						cache[indice][i].tag = tag;
						cache[indice][i].val =1;
						cache[indice][i].info = endereco;
						CA_val_assoc[i] = 1;
					}
					else if(acha0(CA_val_assoc,assoc) == false){
						miss++;
						miss_capacidade++;
						cache[indice][i].tag = tag;
						cache[indice][i].val =1;
						cache[indice][i].info = endereco;	
					}
					else if(cache[indice][i].val == 0){
						miss++;
						miss_compulsorio++;
						cache[indice][i].tag = tag;
						cache[indice][i].val = 1;
						cache[indice][i].info = endereco;
						CA_val_assoc[i] = 1;
					}
				}
			}
		taxa_hit = (((float)hit/qtd_acessos));
		taxa_miss = (((float)miss/qtd_acessos));
		taxa_miss_cap = (((float)miss_capacidade/qtd_acessos));
		taxa_miss_comp = (((float)miss_compulsorio/qtd_acessos));
		taxa_miss_conf = (((float)miss_conflito/qtd_acessos));

		if(flagOut==0){
			printf("\n-------------------------------\n");
			printf("Qntd Acessos: %d\n",qtd_acessos-1); //Lembrando que, o -1 pq o acesso do EOF tambem eh 1 acesso
			printf("hit: %d\n",hit);
			printf("miss: %d\n",miss);
			printf("Taxa de hit: %.2f\n",taxa_hit);
			printf("Taxa de miss: %.2f\n",taxa_miss);
			printf("Taxa de miss de compulsório: %.2f\n",taxa_miss_comp);
			printf("Taxa de miss de capacidade: %.2f\n",taxa_miss_cap);
			printf("Taxa de miss de conflito: %.2f\n",taxa_miss_conf);
			printf("-------------------------------\n");
		}
		else if (flagOut == 1){
			printf("%d, %.2f, %.2f, %.2f, %.2f, %.2f\n",qtd_acessos-1, taxa_hit, taxa_miss, taxa_miss_comp, taxa_miss_cap, taxa_miss_conf);
		}
		fclose(fptr);
		
	return 0;
}

int substitui(char *sub, int assoc) {
	if(*sub == 'r' || *sub == 'R'){
		return rand() % assoc;  
	}
	printf("Você não digitou uma política de substituição válida. Digite: 'r' ou 'R'.");
	exit(0);
}
bool acha0(int *vetor,int tam){ 
	int i;
    for(i=0; i<tam; i++)
    {
        if(vetor[i]==0)
        {
            return true;		 
        }
 
    }
	return false;
}
