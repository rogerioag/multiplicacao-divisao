#include <stdio.h>
#include <assert.h>

int M=0;
int AQ=0;

int mascaraQ = 0x000000FF;
int mascaraA = 0x0001FF00;
int mascaraA8 = 0x00010000;
int mascaraM8 = 0x00000100;
int mascaraQ0 = 0x00000001;

int tamRegistrador = 8;

int getM() {
	return M;
}

void setM(int m) {
	M = m;
}

void setM8(int valor) {
	M = (M & ~mascaraM8) | (valor << 8);
}

void setQ(int valor) {
	AQ = (AQ & ~mascaraQ) | valor;
}

void setA(int valor) {
	AQ = (AQ & ~mascaraA) | (valor << 8);
}

void setAQ(int pAQ) {
	AQ = pAQ;
}

void setQ0(int valor) {
	AQ = (AQ & ~mascaraQ0) | (valor);
}

int getQ() {
	return AQ & mascaraQ;
}

int getA() {
	return (AQ & mascaraA) >> 8;
}

int getA8() {
	return (AQ & mascaraA8) >> 16;
}

int getAQ() {
	return AQ;
}

void executar(int dividendo, int divisor) {
	int i=0;
    // 1. Registrador Q <- Dividendo
	setQ(dividendo);
	// 2. Registrador M <- Divisor
	setM(divisor);
	// 3. Registrador A <- 0
	setA(0);
	// 4. Bit mais significativo de M (m4) é zerado
	setM8(0);
	// 8. Repete os passos 5 a 7, n vezes (n = número de bits de M e Q)
	for (i = 0; i < tamRegistrador; i++) {
		// 5. Desloca os registradores A e Q para a esquerda
		setAQ(getAQ() << 1);
		// 6. Subtrai A - M para saber se o divisor cabe no dividendo
		setA(getA() - getM());
		// 7. Bit A8 do dividendo (Registrador A) é testado
		if (getA8() == 0) {
			// - Não soma A e M
			// - q0 <- 1 (significa que é possível subtrair o divisor do
			// dividendo)
			setQ0(1);
        } else {
			// - Soma A e M para restaurar o dividendo
			setA(getA() + getM());
            // - q0 <- 0 (significa que ainda não é possível fazer a
			// divisão)
			setQ0(0);
        }
    }
	// 9. Quociente (Resultado da divisão) está armazenado em Q e o resto em A
}

int main(int argc,char *argv[]){
   int i;
   printf("Argumentos:\n");
   printf("argc = %d\n", argc);
   for(i=0;i<argc;i++)
       printf("argv[%d] = %s\n", i, argv[i]);

   // Pega os valores da linha de comando.
   int dividendo = atoi(argv[1]);
   int divisor = atoi(argv[2]);

   // Executa o algoritmo de multiplicação.
   executar(dividendo, divisor);

   // Verifica se deu certo.
   // assert( (int)(dividendo / divisor) == getQ());
   printf("Resultado [Q]: %X -> %d e resto %X \n", getQ(), getQ(), getA());
   return 0;
}
