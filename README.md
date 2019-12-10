# **Paralelismo com OpenMP**
![logo](https://user-images.githubusercontent.com/32513366/70485302-4f57e680-1acd-11ea-8cd2-00d77d829923.png)

**OpenMP** é uma interface API que suporta multiprocessamento de memória compartilhada em CPU para desenvolvimento de aplicações paralelas que utilizem ao máximo a capacidade do computador.

## **Prática**
Para entender melhor como funciona essa biblioteca e seus conceitos foi avaliado o desempenho de um código em paralelismo com um sequencial para a resolução de um simples problema matemático de multiplicação de matrix e vetor dado por:

![problema](https://user-images.githubusercontent.com/32513366/70485525-f89edc80-1acd-11ea-91de-c2b65ba628f9.png)

Para definir o desempenho avalia-se o **speedup** da aplicação em paralelo em comparação com a sequencial.

## **Como utilizar o código**
É recomendado a utilização do sistema **Linux** (para este código, foi usado o Ubuntu 18.04) para a construção e manuseio desse material visto a facilidade que este sistema possui com o openMP que já vem instalado nativamente. 

Dado a limitação do sistema UNIX para o tamanho da memória stack, usado bastante neste código, é necessário aumentar o limite de memória (8mb) para permitir que o código calcule para uma alta quantidade de linhas e colunas. Para isso, basta rodar em um terminal:
```sh
ulimit -S 2048000
```
Esse comando permite um limite máximo de 2gb para a memória stack e evita erros de **core dumped**.

Agora, após fazer **git clone** ou **baixar em zip** este material, basta em um terminal (na mesma pasta que o código em C):
```sh
gcc -o code -fopenmp multiplication_matrix_vector_parallel_openMP.c
```
E em seguida rodar o executável:
```sh
./code 1
```

## **Resultados**
Ao executar, será imprimido na tela:

![image](https://user-images.githubusercontent.com/32513366/70486427-8d0a3e80-1ad0-11ea-86c7-37bb9e504bdd.png)

No cabeçalho está a quantidade de linhas (r) e colunas (c) testadas seguida por uma tabela onde a primeira coluna mostra os *N* testes realizados, a segunda mostra o tempo para aplicação sequencial e a terceira o tempo para a aplicação em paralelo.

Fazendo diversos testes para diferentes números de linha e coluna, pode-se gerar um gráfico de speedup como o abaixo:

![image](https://user-images.githubusercontent.com/32513366/70486222-c2625c80-1acf-11ea-94c7-0e40a8459e77.png)

Onde a ordenada é o valor do Speedup e a abscissa é a quantidade de linha (r) X quantidade de coluna (c).
