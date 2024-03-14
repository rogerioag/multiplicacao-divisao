#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int M = 0;
int CAQ = 0;
int mascaraQ = 0x000000FF;
int mascaraA = 0x0000FF00;
int mascaraC = 0x00010000;
int mascaraQ0 = 0x00000001;
int tamRegistrador = 8;

int getM() {
    return M;
}

void setM(int m) {
	M = m;
}

void setQ(int valor) {
	CAQ = (CAQ & ~mascaraQ) | valor;
}

void setA(int valor) {
	CAQ = (CAQ & ~mascaraA) | (valor << 8);
}

void setC(int valor) {
	CAQ = (CAQ & ~mascaraC) | (valor << 16);
}

int getQ() {
	return CAQ & mascaraQ;
}

int getCAQ() {
	return CAQ;
}

void setCAQ(int cAQ) {
	CAQ = cAQ;
}

int getA() {
	return (CAQ & mascaraA) >> 8;
}

int getC() {
	return (CAQ & mascaraC) >> 16;
}

int getQ0() {
	return CAQ & mascaraQ0;
}

int getAQ() {
	return ((getA() << 8) | getQ());
}

void executar(int multiplicando, int multiplicador) {
    int i = 0;
	// 1. Registrador M <- Multiplicando
	setM(multiplicando);
	// 2. Registrador Q <- Multiplicador
	setQ(multiplicador);
	// 3. Registrador A <- 0
	setA(0);
	// 4. Registrador C <- 0
	setC(0);

	// printf("CAQ: %x", getCAQ());

	// 6. Repete o passo 5, n vezes (n = no de bits de M e Q)
	for (i = 0; i < tamRegistrador; i++) {
		// 5. Bit q0 do Multiplicador é testado
		// Se q0=0 {
		if (getQ0() == 0) {
			// - Não soma M e A (o produto parcial não é somado ao
			// Multiplicando)
			// - Desloca os registradores C/A/Q para a direita
			setCAQ(getCAQ() >> 1);
		} else { // Se q0=1 {
				// - Soma M e A resultado fica em A)
				// A <- A + M.
				setA(getA() + getM());
				// - Desloca os registradores C/A/Q para a direita
				setCAQ(getCAQ() >> 1);
		}
	}
		// 7. Produto (resultado final) está armazenado em A e Q
}

int main(int argc,char *argv[]){
   int i;
   if(argc < 3){
       printf("Uso:\n ./multiplicador valor valor\n");
       return(0);
   }

   printf("Argumentos:\n");
   printf("argc = %d\n", argc);
   for(i=0;i<argc;i++)
       printf("argv[%d] = %s\n", i, argv[i]);

   // Pega os valores da linha de comando.
   int multiplicando = atoi(argv[1]);
   int multiplicador = atoi(argv[2]);

   // Executa o algoritmo de multiplicação.
   executar(multiplicando, multiplicador);

   // Recupera o resultado.
   int resultado = getAQ();

   // Verifica se deu certo.
   assert( (multiplicador * multiplicando) == resultado);
   printf("Resultado [AQ]: %X -> %db -> %d \n", resultado, resultado, resultado);
   return 0;
}
