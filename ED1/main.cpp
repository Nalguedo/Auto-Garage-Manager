#define _CRT_SECURE_NO_WARNINGS
#define MAX_VIATURAS 10
#define NR 1
#define RG 2
#define RL 3

#include <stdio.h>
#include <locale.h>
#include <stdlib.h> 
#include <malloc.h> 
#include <string.h>


//Ficha de Trabalho nr 2

typedef int Estado;

typedef struct parque
{
	char matricula[9];
	char data[10];
	char proprietario[30];
	char marca[15];
	char cor[15];
	char dataEntrada[11];
	Estado estadoRep;
	int tempoRep;
	int ocup;
}PARQUE, *ptViatura; //Tipos de variáveis

//Lista Ligada
typedef struct no
{
	PARQUE *info;

	struct no *por;
}NO, ptNo;

void inicializar(PARQUE *v, int numViaturas);
ptViatura aloca(int n);
void inserirViatura(PARQUE* v, int numViaturas);//PARQUE v[] ou PARQUE *v ou ptViatura v
int menu();
int matriculaValida(char* matricula); 
char* substring(char* src, int position, int length);
int dataValida(char* data);
char* mostrarEstado(Estado estado, char *mens);
void mostrarViatura(PARQUE *v, int a);
void mostrarViaturas(PARQUE *v, int numViaturas);
void listaMarca(PARQUE *v, int numViaturas, char *m);
int estadoValido(Estado estado);
int transicaoEstadoValida(Estado estadoAntes, Estado estadoDepois);
void actualizarEstado(PARQUE* v, int numViaturas);
void troca(PARQUE *a, PARQUE *b);
void seleccao(PARQUE* v, int numViaturas);
int maiorMatricula(PARQUE *viatura, int a);
void bubbleSort(PARQUE* v, int numViaturas);
int pesquisaSentinela(PARQUE chave, PARQUE* v, int numViaturas);
int pesquisaBinariaAux(PARQUE chave, PARQUE* v, int inf, int sup);
int pesquisaBinaria(PARQUE chave, PARQUE* v, int numViaturas);
int isAlpha(char c);
int isDigitChar(char c);

void limpaInput()
{
	fseek(stdin, 0, SEEK_END);
}

int main()
{
	setlocale(LC_ALL, "Portuguese");

	int opcao;
	int posicao_viatura;
	PARQUE pesquisa, *oficina;
	char marca0[20];

	//Alocar memória de 3 formas diferentes
	//PARQUE oficina[10];
	//PARQUE *oficina = (PARQUE *)malloc(11*sizeof(PARQUE));
	// ptViatura oficina = (ptViatura)malloc(10 * (sizeof(PARQUE)));

	oficina = aloca(MAX_VIATURAS);

	inicializar(oficina, MAX_VIATURAS);

	do {
		opcao = menu();
		switch (opcao) {
		case 1: inserirViatura(oficina, MAX_VIATURAS);  break;
		case 2: actualizarEstado(oficina, MAX_VIATURAS); break;
		case 3: mostrarViaturas(oficina, MAX_VIATURAS);  break;

		case 4: seleccao(oficina, MAX_VIATURAS);
			printf("\nA lista foi ordenada com sucesso!"); break;
		case 5: bubbleSort(oficina, MAX_VIATURAS);
			printf("\nA lista foi ordenada com sucesso!"); break;
		case 6:
		case 7: printf("\nQual a matricula que procura?\nMatricula: ");
			limpaInput();
			gets_s(pesquisa.matricula);
			if (opcao == 6)
				posicao_viatura = pesquisaSentinela(pesquisa, oficina, MAX_VIATURAS);
			else
				posicao_viatura = pesquisaBinaria(pesquisa, oficina, MAX_VIATURAS);
			if (posicao_viatura == -1)
				printf("\nNao foi encontrada nenhuma viatura com a matricula indicada");
			else {
				printf("\nDados da viatura procurada: ");
				printf("\n[Viatura %d]", posicao_viatura + 1);
				mostrarViatura(oficina, posicao_viatura);
			}
			break;

		case 8: printf("\nQual a marca a listar? ");
			scanf("%s", marca0);
			listaMarca(oficina, MAX_VIATURAS, marca0);
			break;
		}
	} while (opcao != 0);

	
}

void inicializar(PARQUE *v, int numViaturas)
{
	int i;
	for (i = 0; i < numViaturas; i++)
	{
		v[i].ocup = 0;
	}
}

//Função para alocar estrutura dinamicamente
ptViatura aloca(int n)
{
	ptViatura p;

	p = (ptViatura)malloc((n + 1) * sizeof(PARQUE));
	if (!p)
		exit(0);
	return p;
}

int menu()
{
	int opcao;

	//system("cls");
	printf("\n #---------------------------------------------------------#");
	printf("\n | (1) Registar viatura                                    |");
	printf("\n | (2) Actualizar estado de uma viatura                    |");
	printf("\n | (3) Mostrar todas as viaturas em reparacao              |");
	printf("\n | (4) Ordenar viaturas (Seleccao directa iterativo)       |");
	printf("\n | (5) Ordenar viaturas (BubbleSort)                       |");
	printf("\n | (6) Pesquisar viatura (Sequencial c/ Sentinela)         |");
	printf("\n | (7) Pesquisar viatura (Binaria => Viaturas ordenadas)   |");
	printf("\n | (8) Mostrar viaturas de uma determinada marca           |");
	printf("\n |---------------------------------------------------------|");
	printf("\n | (0) Sair                                                |");
	printf("\n #---------------------------------------------------------#");
	do {
		printf("\nQual a sua opcao: ");
		scanf("%d", &opcao);
	} while (opcao < 0 || opcao > 8);
	return opcao;
}

int matriculaValida(char* matricula)  //  função não implementada
{
	int soma = 0;

	if (matricula[2] != '-' || matricula[5] != '-')
		return 0;

	for (int i = 0; i < 7; i += 3)
	{
		if (isAlpha(matricula[i]) && isAlpha(matricula[i + 1]))
			soma += 10;
		else if (isDigitChar(matricula[i]) && isDigitChar(matricula[i + 1]))
			soma += 1;
		else soma -= 100;
	}

	if (soma != 12)
		return 0;

	return 1;
}

int isAlpha(char c) // Verifica se é uma letra ou outro digito
{
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z');
}

int isDigitChar(char c) // Verifica se é um digito
{
	return (c >= '0' && c <= '9');
}

char* substring(char* src, int position, int length)
{// devolve a substring de comprimento length, com início em position
	char *dest;
	dest = (char*)calloc(length + 1, sizeof(char));
	dest[0] = '\0';
	strncat(dest, (src + position), length);
	return dest;
}

int dataValida(char* data)  
{
	int ano = 0, mes = 0, dia = 0, check = 1000, soma = 0;


	if (data[4] != '-' || data[7] != '-')
		return 0;

	//int ano
	for (int a = 0; a < 4; a++)
	{
		soma = (data[a] - '0') * check;
		check /= 10;
		ano += soma;
	}
	
	check = 10;
	//int mes
	for (int a = 5; a < 7; a++)
	{
		soma = (data[a] - '0') * check;
		check /= 10;
		mes += soma;
	}
	check = 10;
	//int dia
	for (int a = 8; a < 10; a++)
	{
		soma = (data[a] - '0') * check;
		check /= 10;
		dia += soma;
	}
	
	if (ano < 1901 || ano > 2018 || mes < 1 || mes > 12 || dia < 1 || dia > 31)
		return 0;

	//Verificar dias e mês corretos e anos regulares

	return 1;
}

void inserirViatura(PARQUE* v, int numViaturas)
{  /* c) Atribuir um lugar disponivel na oficina no momento da chegada */
	int i;
	for (i = 0; i<numViaturas && v->ocup; i++)
		v++;

	if (i >= numViaturas)
		printf("\n\n Todos os lugares da oficina estao ocupados!");
	else
	{  // Ler dados da viatura
		printf("\nIntroduza os dados na nova viatura\n");
		/* a) Ler matricula enquanto nao for valida */
		do {
			printf("Matricula:");
			scanf("%s", v->matricula);
		} while (!matriculaValida(v->matricula));

		/* b) Ler a data de entrada enquanto nao for valida */
		do {
			printf("Data de entrada:");
			scanf("%s", v->dataEntrada);
		} while (!dataValida(v->dataEntrada));

		/* e) Ler nome do proprietario, a marca e a cor da viatura */
		printf("Proprietario:");
		limpaInput();
		gets_s(v->proprietario);
		printf("Marca:");
		limpaInput();
		gets_s(v->marca);
		printf("Cor:");
		limpaInput();
		gets_s(v->cor);
		/* d) Preencher automaticamente campos */
		v->estadoRep = NR;
		v->ocup = 1;
		v->tempoRep = 0;
	}
}

char* mostrarEstado(Estado estado, char *mens)
{

	switch (estado)
	{
	case NR: strcpy(mens, "Nao reparado"); break;
	case RG: strcpy(mens, "Reparado e nao levantado"); break;
	case RL: strcpy(mens, "Reparado e levantado"); break;
	default: strcpy(mens, "Estado invalido"); break;
	}
	return mens;
}

void mostrarViatura(PARQUE* v, int a)
{
	char mens[30];
	printf("\nMatricula: %s", v[a].matricula);
	printf("\nProprietario: %s", v[a].proprietario);
	printf("\nMarca: %s", v[a].marca);
	printf("\nCor: %s", v[a].cor);
	printf("\nData de entrada: %s", v[a].dataEntrada);
	printf("\nEstado da reparacao: %s", mostrarEstado(v[a].estadoRep, mens));
	printf("\nTempo de reparacao (minutos): %d\n", v[a].tempoRep);
}

void mostrarViaturas(PARQUE* v, int numViaturas)
{
	int i;

	for (i = 0; i<numViaturas; i++)
	{
		if (v[i].ocup)
		{
			printf("\n\n[Viatura %d]", i + 1);
			mostrarViatura(v, i);
		}
	}
}

void listaMarca(PARQUE* v, int numViaturas, char *m)
{
	int i, num = 0;

	printf("\n\nLista das viaturas da marca [%s]:", m);
	for (i = 0; i < numViaturas; i++)
	{
		if ((v[i].ocup) && strcmp(v[i].marca, m) == 0)
		{
			printf("\n\n[Viatura %d]", i + 1);
			mostrarViatura(v, i);
			num++;
		}
	}
	if (num <= 0)
		printf("\nNao existem viaturas da marca [%s]", m);
	else if (num == 1)
		printf("\nExiste apenas uma viatura da marca [%s]", m);
	else
		printf("\nExistem %d viaturas da marca [%s]",num, m);
}

int estadoValido(Estado estado)
{
	return (estado == NR || estado == RG || estado == RL);
}

int transicaoEstadoValida(Estado estadoAntes, Estado estadoDepois)
{
	return ((estadoAntes == NR && estadoDepois == RG)
		|| (estadoAntes == NR && estadoDepois == RL)
		|| (estadoAntes == RG && estadoDepois == RL));
}

void actualizarEstado(PARQUE* v, int numViaturas)
{
	int i;

	printf("\nQual o nº do registo da viatura a actualizar?\nRegisto: ");
	scanf("%d", &i);
	if (i>0 && i <= numViaturas && (v + i - 1)->ocup == 1)
	{
		PARQUE* viaturaActualizar = v + i - 1;
		Estado novoEstado;

		/* Mostrar dados da viatura */
		printf("\n[Viatura %d]", i);
		mostrarViatura(viaturaActualizar, i);

		/* Ler novo estado de reparacao da viatura até que este seja válido e
		em conjunção com o antigo represente uma transição válida */
		do {
			printf("\nQual o novo estado da viatura?");
			printf("\n\t 1 – Não reparado");
			printf("\n\t 2 – Reparado e nao levantado");
			printf("\n\t 3 – Reparado e levantado\nNovo estado: ");
			scanf("%d", &novoEstado);
		} while (!estadoValido(novoEstado) ||
			!transicaoEstadoValida(viaturaActualizar->estadoRep, novoEstado));

		if (viaturaActualizar->estadoRep == NR)
		{  /* Inserir tempo de reparacao */
			printf("\nQual o tempo de reparacao?\nTempo (min): ");
			scanf("%d", &viaturaActualizar->tempoRep);
		}
		if (novoEstado == RL) {   /* Libertar lugar */
			(viaturaActualizar)->ocup = 0;
		}
		/* Guardar o novo estado Guardar reparacao */
		viaturaActualizar->estadoRep = novoEstado;
	}
	else
		printf("\nNº de registo invalido ou o registo nao esta ocupado!");
}

void troca(PARQUE *a, PARQUE *b)
{
	PARQUE *temp = (PARQUE *)malloc(sizeof(PARQUE));
	*temp = *a;
	*a = *b;
	*b = *temp;
}

/* Ordenar por matricula usando o Método de Selecao Direta iterativo */
void seleccao(PARQUE *v, int numViaturas)
{
	int i, j, posMax, mat1 = 0, matMax = 0;
	
	for (j = numViaturas - 1; j > 0; j--)
	{	//Determinar maior matricula
		//procura o máximo
		posMax = 0;
		matMax = maiorMatricula(v, posMax);
		
		for (i = 1; i <= j; i++)
		{
			mat1 = maiorMatricula(v, i);
			if (mat1 > matMax)
			{
				posMax = i;
				matMax = mat1;
			}
		}	
		troca(&v[j], &v[posMax]);
	}
}

//Determinar valor matricula
int maiorMatricula(PARQUE *viatura, int a)
{
	int somaNumA = 0, somaLetrasA = 0, i = 0;

	if (!viatura[a].ocup)
		return 0;
	
	while (!isAlpha(viatura[a].matricula[i]))
	{
		i++;
	}

	if (i < 2)
	{
		somaLetrasA = 0 + (viatura[0].matricula[i] * 1000) + (viatura[a].matricula[1] * 1000);
		somaNumA = ((viatura[a].matricula[3] - '0') * 1000) + ((viatura[a].matricula[4] - '0') * 100) + ((viatura[a].matricula[6] - '0') * 10) + (viatura[a].matricula[7] - '0');
	}
	else
	{
		if (i > 2 && i < 5)
		{
			somaLetrasA = 200000 + (viatura[3].matricula[i] * 1000) + (viatura[a].matricula[4] * 1000);
			somaNumA = ((viatura[a].matricula[0] - '0') * 1000) + ((viatura[a].matricula[1] - '0') * 100) + ((viatura[a].matricula[6] - '0') * 10) + (viatura[a].matricula[7] - '0');
		}
		else
		{
			somaLetrasA = 100000 + (viatura[0].matricula[6] * 1000) + (viatura[a].matricula[7] * 1000);
			somaNumA = ((viatura[a].matricula[0] - '0') * 1000) + ((viatura[a].matricula[1] - '0') * 100) + ((viatura[a].matricula[3] - '0') * 10) + (viatura[a].matricula[4] - '0');
		}
	}
	
	return (somaLetrasA + somaNumA);
}

/* Ordenar por matricula usando o Método Bubblesort */
void bubbleSort(PARQUE* v, int numViaturas)
{
	bool flag = true;
	int mat1 = 0, mat2 = 0;

	while (flag)
	{
		flag = false;

		for (int i = 0; i < numViaturas - 1; i++)
		{
			mat1 = maiorMatricula(v, i);
			mat2 = maiorMatricula(v, i+1);
			if (mat1 < mat2)
			{
				troca(&v[i], &v[i+1]);
				flag = true;
			}
		}
	}
}

int pesquisaSentinela(PARQUE chave, PARQUE* v, int numViaturas)
{  /* 6a) Pesquisa sequencial com sentinela por matricula*/
	v[numViaturas] = chave;
	int i = 0;
	while (strcmp(v[i].matricula, chave.matricula) != 0)
		i++;
	if (i < numViaturas)
		return i;
	else return -1;
}

int pesquisaBinariaAux(PARQUE chave, PARQUE* v, int inf, int sup)
{  /* 6b) Pesquisa binaria => Elementos ordenados */
   // completar
	int posicao, indMeio;
	posicao = -1; inf = 0;

	while (sup >= inf && posicao == -1) 
	{
		indMeio = (sup + inf) / 2;

		if (strcmp(v[indMeio].matricula, chave.matricula) == 0)
			posicao = indMeio;
		else
		{
			if (strcmp(v[indMeio].matricula, chave.matricula) < 0)
				inf = indMeio + 1;
			else
				//v[indMeio] > chave;
				sup = indMeio - 1;
		}
	} 
	return posicao;

}

int pesquisaBinaria(PARQUE chave, PARQUE* v, int numViaturas)
{
	return pesquisaBinariaAux(chave, v, 0, numViaturas - 1);
}
