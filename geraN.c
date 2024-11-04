/*
* Programa que gera N números inteiros positivos aleatórios.
* N deve ser dado pelo usuário na linha de comando.  
* Se houver um terceiro parâmetro de linha de comando, ele será o limite superior para os valores gerados. 
* ex.: "geraN 10" ir=a gerar 10 números entre 0 e 10
* ex.2: "geraN 10 2" irá gerar 10 valores entre 0 e 2
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  unsigned int i, val, N;
  if(argc == 2){
    N = val = atoi(argv[1]);
  } else {
    if(argc == 3){
      N = atoi(argv[1]);
      val = atoi(argv[2]);
    } else {
      printf("Uso: geraN <numero de valores a serem gerados> [limite superior para os valores]\n");
      exit(0);
    }
  }
  val++;
  for(i=0; i<N; i++)
    printf("%u\n", (unsigned int) random()%val);

  exit(0);
}
