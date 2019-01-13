# Bares - Basic ARithmetic Expression Evaluator based on Stacks

###### Autor
```
Lucas Lima Marques de Oliveira
```
### Introdução

The **B**asic **AR**ithmetic **E**xpression **E**valuator based on **S**tacks:

* constantes numéricas inteiras (-32.768 a 32.767)
* operadores (+, -, /, *, ^, %)
* parênteses


### Descrição

O programa receberá através de um arquivo uma ou mais expressões e deverá conferir se as expressões estão corretas (de acordo com a gramática EBNF (Extended Backus-Naur Form)), e imprimir um resultado sendo ele correto (com o valor resultante da expressão), ou incorreto (com o erro específico).

### Gramática

A gramática que será utilizada no programa BARES :

```
<expr>            := <term>,{ ("+"|"-"),<term> };
<term>            := "(",<expr>,")" | <integer>;
<integer>         := 0 | ["-"],<natural_number>;
<natural_number>  := <digit_excl_zero>,{<digit>};
<digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
<digit>           := "0"| <digit_excl_zero>;
```

## Compilação

Para compilar o programa Bares:
----------------------------------------------
```
	$ make
```

## Execução

Para executar o programa Bares:
----------------------------------------------
```
	$ ./Bares
```

Para entra com os dados atraves de um arquivo:
-------------------------------------------------
```
	$ ./Bares <arquivo_de_entrada
```

Para entra com os dados atraves de um arquivo e exportar para outro arquivo:
-------------------------------------------------
```
	$ ./Bares <arquivo_de_entrada >arquivo_de_saida
```
## Co-autor

Parte do codigo foi desenvolvido pelo professor de Linguagem de Programação Basica I, *Selan R. dos Santos.*


## Link
link para o projeto BARES no GitLab [BARES](https://Limao96@projetos.imd.ufrn.br/Limao96/Bares.git).