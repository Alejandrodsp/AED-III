#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 100000000;

void troca(int vetor[], int i, int j);
int **AlocaMatriz(int m, int n);
void LiberaMatriz(int **M, int m);
int *conta_linhas_colunas(char *nome_arquivo);
int **leEntradas(char *arq, int linha, int coluna, int **m);
void Algoritmo_Otimo(int *vetor, int inf, int sup, int **m, int *min, int *result);
void Algoritmo_Aproximativo(int *vetor, int linha, int coluna, int **m, int *min, int *result);

int main()
{
	int **matriz_entradas, *qtd_linhas_colunas, *v, *resultado, i, j, n, min;
	char nome_arq[30];

	printf("Informe o nome do arquivo de entrada: ");
	scanf("%s", nome_arq);

	qtd_linhas_colunas = (int *)malloc(sizeof(int) * 2);												   // Vetor aonde será armazenado a quantidade de linhas e colunas lidas do arquivo de entrada, respectivamente
	qtd_linhas_colunas = conta_linhas_colunas(nome_arq);											   // retorna a quantidade de linhas e colunas que estao no arquivo de entrada
	matriz_entradas = leEntradas(nome_arq, qtd_linhas_colunas[0], qtd_linhas_colunas[1], matriz_entradas); // Le o arquivo de entrada e armazena numa matriz

	printf("Matriz de Adjacencia do Grafo de Entrada\n");
	for (i = 0; i < qtd_linhas_colunas[0]; i++)
	{
		for (j = 0; j < qtd_linhas_colunas[1]; j++)
		{
			printf("%d ", matriz_entradas[i][j]);
		}
		printf("\n");
	}

	v = (int *)malloc(sizeof(int) * qtd_linhas_colunas[1] + 1); // Vetor que vai conter o nodos do grafo, para realizar as permutaçoes

	for (i = 0; i < qtd_linhas_colunas[1]; i++)
	{
		v[i] = i;
	}
	v[qtd_linhas_colunas[1]] = 0;

	resultado = (int *)malloc(sizeof(int) * (qtd_linhas_colunas[1] + 1)); // Vetor que ira armazenar o caminho de menor custo
	min = INF;															  // Variavel que ira conter o menor custo

	/*Algoritmo_Otimo(v, 1, qtd_linhas_colunas[1] - 1, matriz_entradas, &min, resultado); // Algoritmo que vai retornar a melhor solução "talvez daqui alguns seculos"

	printf("Algoritmo otimo\n");
	printf("O menor caminho foi:");
	for (i = 0; i < qtd_linhas_colunas[1]; i++)
		printf(" %d ->", resultado[i]);
	printf(" %d", resultado[i + 1]);
	printf("\nCom custo de: %d\n", min);*/

	min = 0;
	for (i = 0; i < qtd_linhas_colunas[1]; i++)
	{
		v[i] = i;
	}
	v[qtd_linhas_colunas[1]] = 0;
	for (i = 0; i < qtd_linhas_colunas[1]+1; i++)
	{
		resultado[i] = -1;
	}

	Algoritmo_Aproximativo(v, qtd_linhas_colunas[0], qtd_linhas_colunas[1], matriz_entradas, &min, resultado);
	resultado[qtd_linhas_colunas[1]+1] = v[0];
	printf("Algoritmo aproximativo\n");
	printf("O menor caminho foi:");
	for (i = 0; i < qtd_linhas_colunas[1]; i++)
		printf(" %d ->", resultado[i]);
	printf(" %d", resultado[i + 1]);
	printf("\nCom custo de: %d", min);

	LiberaMatriz(matriz_entradas, qtd_linhas_colunas[0]);
	free(qtd_linhas_colunas);
	free(v);
	free(resultado);

	return 0;
}
void troca(int vetor[], int i, int j)
{
	int aux = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = aux;
}
void Algoritmo_Otimo(int *vetor, int inf, int sup, int **m, int *min, int *result)
{
	int soma, i;
	soma = 0;
	if (inf == sup)
	{
		for (i = 0; i <= sup; i++)
		{
			soma = soma + m[vetor[i]][vetor[i + 1]];
		}
		if (soma < (*min))
		{
			(*min) = soma;
			for (i = 0; i <= sup; i++)
			{
				result[i] = vetor[i];
			}
			result[i + 1] = vetor[0];
		}
	}
	else
	{
		for (i = inf; i <= sup; i++)
		{
			troca(vetor, inf, i);
			Algoritmo_Otimo(vetor, inf + 1, sup, m, min, result);
			troca(vetor, inf, i);
		}
	}
}

int **AlocaMatriz(int m, int n)
{
	int **M;
	int i;

	M = (int **)malloc(sizeof(int *) * m);
	if (M == NULL)
	{
		printf("Memoria insuficiente.\n");
		exit(1);
	}
	for (i = 0; i < m; i++)
	{
		M[i] = (int *)malloc(sizeof(int) * n);
		if (M[i] == NULL)
		{
			printf("Memoria insuficiente.\n");
			exit(1);
		}
	}
	return M;
}
void LiberaMatriz(int **M, int m)
{
	int i;
	for (i = 0; i < m; i++)
		free(M[i]);
	free(M);
}

int *conta_linhas_colunas(char *nome_arquivo)
{
	char aux;
	int *retorno, i;
	FILE *arquivo;

	arquivo = fopen(nome_arquivo, "r");
	retorno = (int *)malloc(sizeof(int) * 2);
	retorno[0] = 0;
	retorno[1] = 0;
	i = 0;
	while (!feof(arquivo))
	{
		fscanf(arquivo, "%c", &aux);
		if (aux != ' ' && aux != '\n' && i == 0)
		{
			retorno[1] = retorno[1] + 1;
			if (i == 0)
			{
				retorno[0] = retorno[0] + 1;
				i = 1;
			}
		}
		else
		{
			if (aux == '\n')
				i = 0;
		}
	}
	fclose(arquivo);
	return retorno;
}
int **leEntradas(char *arq, int linhas, int colunas, int **m)
{
	FILE *arquivo;
	int i, j;
	char s[30], lixo;

	m = AlocaMatriz(linhas, colunas);
	arquivo = fopen(arq, "r");
	i = 0;
	j = 0;
	while (!feof(arquivo))
	{

		fscanf(arquivo, "%s", s);
		m[i][j] = atoi(s);
		if (j == colunas - 1)
		{
			j = -1;
			i++;
		}
		j++;
	}
	fclose(arquivo);
	return m;
}
void Algoritmo_Aproximativo(int *vetor, int linha, int coluna, int **m, int *min, int *result)
{
	int i, j, k, menor, flag;

	result[0] = vetor[0];

	for(i = 0; i < linha; i++){
		menor = INF;
	for (j = 0; j < coluna; j++)
	{
		flag = 0;
		if((m[result[i]][j] != 0) && (m[result[i]][j] < menor)){
			for(k = i; k >= 0; k--){
				if(result[k] == j){
					flag = 1;
				}

			}
		if(flag == 0){
			menor = m[result[i]][j];
			result[i+1] = j;
		}
		}
	}
	}
	result[coluna] = vetor[0];
	for(i = 0; i < coluna; i++){
		*min = (*min) + m[result[i]][result[i+1]];
	}

}