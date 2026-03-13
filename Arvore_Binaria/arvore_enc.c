#include <stdlib.h> // Utilizamos para alocar memória (malloc)
#include <math.h> // Funções matemáticas
#include <stdio.h> // printf (entrada e saída)
#include "arvore_enc.h" // arquivo header, utilizado para nomear funções

struct tad { // Caixinha que vai guardar todas as informações
    // Possui três compartimentos
    int x; // Numero que aparece na saida (dado guardado)
    Tad *esq; // ponteiro que aponta para o nó filho (esquerdo), sempre menor que o pai
    Tad *dir; // ponteiro que aponta para o nó filho (direito), sempre maior que o pai
}; // cria bloquinhos, coloca um número no x, e amarra o esq e o dir em outros bloquinhos.

Tad *criar_no(int x) {
    // Pede um espaço na memória do tamanho exato da nossa "caixinha"
    Tad *t =(Tad*) malloc(sizeof(Tad));
    if(t == NULL) // Se o PC estiver sem memória, o malloc devolve NULL.
        return NULL;
    t->x = x; // Coloca o número que você passou dentro da caixinha
    t->dir = NULL; // Ainda não tem filhos 
    t->esq = NULL;
    return t; // Devolve a "caixa" pronta para ser usada
} // Sempre que o programa precisar adicionar um numero novo na arvore vai chamar essa função para fabricar o nó

int adicionar_no(Tad* t, int x) { // Essa função faz o papel do entregador procurando o endereço certo para deixar a encomenda (o novo número x)
    // Enquanto o entregador estiver em um nó que existe (não for NULL), ele continua procurando...
    while (t != NULL) { 
        if (x < t->x) { // O número novo (x) é MENOR que o número do nó atual?
            if (t->esq == NULL) { // // Olha para a cadeira da ESQUERDA. Está vazia (NULL)?
                t->esq = criar_no(x); // Vaga encontrada! Chama a fábrica, cria a caixa e pendura na esquerda!
                if (t->esq != NULL) // Retorna 1 se deu certo, 0 se o malloc falhou lá na fábrica
                    return 1;
                else
                    return 0;
            }
            else { // // A cadeira da esquerda está ocupada?
                t = t->esq; // // Então o entregador "desce" para esse nó da esquerda e o loop recomeça a partir dele!
            }
        } else { // O número novo (x) é MAIOR (ou igual) ao número do nó atual?
            if (t->dir == NULL) { // // Olha para a cadeira da DIREITA. Está vazia (NULL)?
                t->dir = criar_no(x); // Vaga encontrada! Chama a fábrica, cria a caixa e pendura na direita!
                if (t->dir != NULL)
                    return 1; // Retorna 1 (sucesso) ou 0 (falha).
                else
                    return 0;
            }
            else { // A cadeira da direita está ocupada?
                t = t->dir; // Então o entregador "desce" para esse nó da direita e o loop recomeça.
            }
        }
    }
    return 0; // Se a árvore estivesse totalmente vazia logo de cara, ele cai aqui.
}

// n! = n * (n-1)!
// 0! = 1
// a recursividade é a forma mais fácil de navegar por uma árvore. Funções de árvore 
// (como aquela pre_ordem que imprimiu os números no terminal) usam exatamente 
// essa mesma lógica de "chamar a si mesmas" para ir descendo pelos nós da esquerda e da direita.

int fatorial_no(int n) {
    if (n == 0) // Se chegarmos no zero, pare de chamar a função e apenas devolva o número 1
        return 1;
    else
        return n * fatorial_no(n-1); // Se o número não for zero, a função devolve o número atual multiplicado pelo fatorial do número anterior

}

//  n  0 1 2 3 4 5 6  7  8  9
//f(n) 0 1 1 2 3 5 8 13 21 34
// n == 0 -> f(n) = 0
// n == 1 -> f(n) = 1
// caso contrario -> f(n) = f(n-1) + f(n-2)
int fibonacci_no(int n) {
    if (n == 0) // Como a gente precisa olhar para trás duas vezes, precisamos de 
    // dois pontos de parada fixos para a função não tentar calcular o "Fibonacci de -1".
        return 0;
    else if (n == 1)
        return 1;
    else // É como se ela tivesse dois "filhos": o filho da esquerda calcula o n-1 e o filho da direita calcula o n-2.
        return fibonacci_no(n-1)+fibonacci_no(n-2);
}
// Isso é exatamente o que as funções da sua árvore (como pre_ordem, in_ordem ou pos_ordem) 
// vão fazer para imprimir ou buscar números. Elas vão se dividir 
// em duas (uma chamando t->esq e a outra t->dir) e vão até o fundo da árvore antes de voltar.

void remover(Tad **pRaiz, int numero) { // Esse ** (ponteiro duplo) significa que, em vez de apenas olhar para o nó, essa função tem o poder de alterar a conexão do pai dele
    if(*pRaiz == NULL){   // esta verificacao serve para caso o numero nao exista na arvore.
       printf("Numero nao existe na arvore!"); // ele foi descendo pelos galhos da árvore, chegou no chão e não achou o número.
       return;  
    }
    if(numero < (*pRaiz)->x) // Se o numero que quero for menor do que o para onde o olofote aponta
       remover(&(*pRaiz)->esq, numero); // obrigatoriamente tem que estar para o lado esquerdo.
    else 
       if(numero > (*pRaiz)->x) // Se o numero que quero for maior do que o para onde o olofote aponta
          remover(&(*pRaiz)->dir, numero); // A função chama a si mesma de novo, mas agora mandando a busca
        // descer pelo galho da direita (dir). "Vai procurar no meu filho da direita!".
       else{    // se nao eh menor nem maior, logo, eh o numero que estou procurando! :)

          Tad *pAux = *pRaiz; // O *pRaiz é o nosso holofote, que está exatamente em cima do nó 
          // que queremos deletar. O pAux (ponteiro auxiliar) é como se fosse um "marcador".
          if (((*pRaiz)->esq == NULL) && ((*pRaiz)->dir == NULL)){ // se nao houver filhos...
                free(pAux); // Destroi a caixinha de memória
                (*pRaiz) = NULL;  // partir de agora você não tem mais filho desse lado, é NULL".
               }
          else{     // so tem o filho da direita, teste lá no if
             if ((*pRaiz)->esq == NULL){
                (*pRaiz) = (*pRaiz)->dir; // O holofote (que é a conexão com o avô) pula o nó que vai 
                // ser deletado e se conecta direto com o neto (o filho da direita). O neto foi adotado pelo avô!
                pAux->dir = NULL; // Corta a cordinha que prendia o nó deletado ao neto.
                free(pAux); pAux = NULL; // Destrói a caixinha velha da memória.
                }
             else{   //so tem filho da esquerda
                if ((*pRaiz)->dir == NULL){
                    // o avô "adota" o neto da esquerda ((*pRaiz) = (*pRaiz)->esq).
                    // Depois, cortamos a cordinha e passamos o free() no nó antigo.
                    (*pRaiz) = (*pRaiz)->esq;
                    pAux->esq = NULL;
                    free(pAux); pAux = NULL;
                    }
                else{  // Obs: Escolhi fazer o maior filho direito da subarvore esquerda.
                    // Imagine a situação: você quer apagar o número 8. Só que o 8 tem 
                    // uma família inteira pendurada na esquerda (menores) e outra na direita (maiores).
                    // Quem pode assumir o lugar do 8 sem quebrar a regra da árvore? Tem que ser alguém 
                    // que seja maior que todo mundo da esquerda, mas menor que todo mundo da direita.
                   pAux = MaiorDireita(&(*pRaiz)->esq);
                   // O código manda um "caçador de recompensas" (a função MaiorDireita) entrar na porta 
                   // da esquerda (&(*pRaiz)->esq) e procurar o maior número que existir lá dentro. 
                   // Esse número é o substituto perfeito! 
                   // A função MaiorDireita arranca esse substituto lá do fundo da árvore e o entrega na mão do pAux.
                   pAux->esq = (*pRaiz)->esq;  // "vestindo a roupa" do nó que vai morrer
                   pAux->dir = (*pRaiz)->dir; // O substituto se conecta com as duas metades da árvore
                   (*pRaiz)->esq = (*pRaiz)->dir = NULL; // Ele zera seus ponteiros para ter certeza de que não vai puxar ninguém com ele quando for destruído.
                   // free((*pRaiz)): O nó original é finalmente apagado da memória
                   // pRaiz = pAux: O avô (que estava apontando para o 8) agora é atualizado para 
                   // apontar para o nosso substituto heroico. A coroa foi passada.
                   free((*pRaiz));  *pRaiz = pAux;  pAux = NULL;   
                   // pAux = NULL: O nosso ajudante pAux não é mais necessário, então limpamos ele.
                   }
                }
             }
          }
}
Tad *MaiorDireita(Tad **no)
{ // entrar na subárvore da esquerda e buscar o maior número possível para ser o substituto perfeito.
    if((*no)->dir != NULL) // O holofote (*no) chega em um nó. Ele olha para a direita. Tem alguém lá? Se tiver (!= NULL), ele não é o maior! Então, a função usa a recursividade 
    // para "descer" para o filho da direita e continua fazendo isso até não ter mais caminho.
       return MaiorDireita(&(*no)->dir);
    else{
       Tad *aux = *no; // Segura o nó substituto!
       if((*no)->esq != NULL) // se nao houver essa verificacao, esse nó vai perder todos os seus filhos da esquerda!
          *no = (*no)->esq; // ligar o "pai" do nosso alvo diretamente ao "filho esquerdo" do nosso alvo
       else
          *no = NULL; // se não tiver filho esquerda retorna null
       return aux; // A Ação: Ele devolve o nó extraído para a função remover usar como a nova "coroa" da árvore.
       }
}
// nó sae sad
// em ordem: sae nó sad
void pre_ordem(Tad *t) {
    if (t == NULL)
        return; // Se o holofote chegou em um buraco vazio (no fundo da árvore), ele simplesmente para e volta.
    else {
        printf("no %d\n", t->x); // imprime a si mesmo antes de visitar os filhos
        pre_ordem(t->esq); // Depois imprime os menores (à esquerda)
        pre_ordem(t->dir); // Por ultimo os maiores (à direita)
    }
}

