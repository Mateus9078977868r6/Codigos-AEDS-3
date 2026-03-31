#include <stdio.h> // entrada e saída (printf)
#include <stdlib.h> // Utilização do malloc e do free

typedef long TChave; // long: inteiro que cabe mais digitos // Ex.: Typedef "char" 
// permite que o programa 'inteiro' agora trabalhe com letras
typedef struct {
/* outros componentes */ // Nome, Idade, Endereço
    TChave Chave;
} TItem; // Ao invés de um valor no nó, cria um "pacote" TItem
typedef struct No { 
    TItem item; // pacote de dados
    struct No *pEsq, *pDir; // ponteiro para os filhos
} TNo;
typedef TNo *TArvoreAVL; // estética

void TArvore_Inicia(TNo **pRaiz) { // muda a VARIAVÉL ORIGINAL no main, não apenas a cópia
    *pRaiz = NULL; // Toda arvore nasce vazia, garante que a raiz não esteja apontando 
                   // para nenhum "lixo de memória" que o Windows deixou para trás
}

TNo *TNo_Cria(TItem x){ // recebe o pacote de dados completo e fabrica a caixinha
    TNo *pAux = (TNo*) malloc(sizeof(TNo)); // separa um espaço na memória do tamanho do nosso
                                            // TNo que é guardado no Aux
    pAux->item = x; // pega a "carga" x e guarda dentro da caixinha
    pAux->pEsq = NULL; // o Nó acabou de nascer (não tem filhos)
    pAux->pDir = NULL;
    return pAux; // Devolve o endereço de memória para quem requisitou
}
// O TArvore_Inicia limpa o terreno para a árvore nascer, 
// e o TNo_Cria fabrica os tijolos que vamos usar para construir os galhos.

int TArvoreAVL_Pesquisa(TArvoreAVL pRaiz, TChave c, TItem *pX){
    // recebe a raiz, a chave (o que procuro) e um ponteiro
    // *pX: coloca o "pacote" inteiro de dados dele dentro desse pX para usar lá no main()
    if (pRaiz == NULL)
        return 0; // O numero não existe? Retorna falso.
    if (c < pRaiz->item.Chave) // se a chave procurada for menor
        return TArvoreAVL_Pesquisa(pRaiz->pEsq, c, pX); // joga o holofote para esquerda
    if (c > pRaiz->item.Chave) // se for maior
        return TArvoreAVL_Pesquisa(pRaiz->pDir, c, pX); // joga pra direita
    *pX = pRaiz->item; // Encontrada! Igual a que procuramos (não maior nem menor)
    // O código copia o pacote de dados do nó (pRaiz->item) para dentro da variável *pX 
    // e retorna 1 (Verdadeiro) para avisar o programa que a busca foi um sucesso.
    return 1;
}

void Sucessor(TNo *q, TNo **r){ // Pega o menor dos maiores! indo pra direita 
                                // e descendo tudo pela esquerda
    TNo *pAux;
    if ((*r)->pEsq != NULL) { // Desce pela esquerda até parar (null)
        Sucessor(q, &(*r)->pEsq); // achamos o menor numero possivel!
        return;
    }
    q->item = (*r)->item; // copia todos os dados do pacote e joga por cima do nó 
                          // que queremos apagar
    pAux = *r; 
    *r = (*r)->pDir; // salva o filho da direita
    free(pAux); // passa o free para apagar a caixinha vazia
}

void Antecessor (TNo *q, TNo **r){ // Busca o maior dos menores!
    TNo *pAux;
    if ((*r)->pDir != NULL) { // enquanto tiver alguem na direita desce, 
        Antecessor(q, &(*r)->pDir); // quando chegar null achou o maior daquele lado
        return;
    }
    q->item = (*r)->item; // pega todo pacote de dados do nosso subtituto
    pAux = *r;
    *r = (*r)->pEsq; // salva os possíveis órfãos 
    free(pAux); // passo o free para limpar a caixinha vazia 
}

int Altura(TNo* pRaiz) {
    // Essa função vai descendo, descendo, descendo... Quando bate no zero, ela volta subindo 
    // e somando 1 + 1 + 1 até chegar na raiz e dar a altura total da árvore!
    int iEsq,iDir;
    if (pRaiz == NULL)
        return 0; // Altura zero! Freio de mão recursividade
    iEsq = Altura(pRaiz->pEsq);
    iDir = Altura(pRaiz->pDir);
    // O nó pausa o que está fazendo e manda a fita métrica descer para o lado esquerdo e para o 
    // lado direito. O resultado da medição volta e fica guardado nas variáveis iEsq e iDir.
    if ( iEsq > iDir )
        return iEsq + 1;
    else
        return iDir + 1;
    // O código compara a altura da esquerda com a da direita, pega a maior delas e soma 1 
    // (que representa o próprio nó onde estamos medindo agora).
}


int FB (TNo* pRaiz) { // Fator de Balanceamento
    if (pRaiz == NULL)
        return 0; // / Se não tem nó, o peso é zero.
        // Depois: ele pega a altura do lado esquerdo e subtrai a altura do lado direito (Esq - Dir)
        // Na regra da AVL, o FB só pode ser -1, 0 ou 1. Se essa conta der 2 ou -2, a sirene toca! 
        // A árvore avisa: "Fiquei torta demais, preciso fazer uma Rotação!".
    return Altura(pRaiz->pEsq) - Altura(pRaiz->pDir);
}


void RSE(TNo** ppRaiz) { // Rotação esquerda (Direita está cheia)
    // ppRaiz: chefe antigo (desbalanceado)
    TNo *pAux;
     // ppAux: novo chefe
    pAux = (*ppRaiz)->pDir; // Filho promovido
     // Novo Chefe vai subir, ele precisa soltar quem ele estava segurando na mão esquerda
     // (se é que tinha alguém). 
    // Esse "órfão" da esquerda é imediatamente adotado pelo Chefe Antigo, assumindo a vaga da 
    // direita que acabou de ficar vazia.
    (*ppRaiz)->pDir = pAux->pEsq;
    // Chefe antigo rebaixado
    pAux->pEsq = (*ppRaiz);
    // A rotação acabou e a árvore está balanceada de novo!
    (*ppRaiz) = pAux;
}

void RSD(TNo** ppRaiz) { // arvore pesada demais na esquerda
    TNo *pAux;
    // 1. O Filho (da esquerda) é promovido a Novo Chefe
    pAux = (*ppRaiz)->pEsq;
    // 2. A Adoção do Órfão (da direita do Novo Chefe)
    (*ppRaiz)->pEsq = pAux->pDir;
    // 3. O Chefe Antigo é rebaixado (vai para a direita do Novo Chefe)
    pAux->pDir = (*ppRaiz);
    // 4. A Coroa é passada
    (*ppRaiz) = pAux;
}

int BalancaEsquerda(TNo** ppRaiz) { // árvore caindo para o lado esquerdo
    int fbe = FB ( (*ppRaiz)->pEsq ); // entrevista com o filho "para qual lado está pesando?"
                                    // resultado armazenado em fbe
    if ( fbe > 0 ) { // se o filho também está pesando para a esquerda
        RSD(ppRaiz); // Solução fácil: basta uma simples rsd na raiz e está resolvido
        return 1;
    }
    else if (fbe < 0 ) { /* Rotação Dupla Direita */ // filho da esquerda pesando para a direita
        RSE( &((*ppRaiz)->pEsq) ); // Rotação Simples à Esquerda só no filho. Isso serve para "esticar o joelho", 
        // transformando o zigue-zague em uma linha reta (caindo para a esquerda).
        RSD( ppRaiz ); // Agora que virou uma linha reta, a gente faz a Rotação Simples à Direita na raiz principal
        return 1;
    }
    return 0;
}

int BalancaDireita(TNo** ppRaiz) { // árvore pesando demais no lado direito
    // 1. A Entrevista com o Filho da Direita
    int fbd = FB( (*ppRaiz)->pDir);
    if ( fbd < 0 ) { // Se o filho da direita também pesa para a direita (fbd < 0), é uma linha reta.
        RSE (ppRaiz); // Rotação Simples à Esquerda (RSE) na raiz principal.
        return 1;
    }
    else if (fbd > 0 ) { /* Rotação Dupla Esquerda */
        RSD( &((*ppRaiz)->pDir) ); // Passo 1: "Estica o joelho"
        RSE( ppRaiz ); /* &(*ppRaiz) */ // Dá um giro para a Esquerda (RSE) na raiz principal
        return 1;
    }
    return 0;
}
/* A árvore é extremamente inteligente. Ela percebe que ficou torta, olha para os próprios filhos 
   para entender o "formato" da tortura, conserta os filhos se for preciso (esticando a linha), e 
   depois arruma a raiz principal. */

int Balanceamento(TNo** ppRaiz) { // alarme que dispara se a árvore entortar.
    int fb = FB(*ppRaiz); // // Calcula o Fator de Balanceamento do nó atual
    if ( fb > 1)
        return BalancaEsquerda(ppRaiz); // Pendendo muito pra Esquerda!
    else if (fb < -1 )
        return BalancaDireita(ppRaiz); // Pendendo muito pra Direita!
    else
        return 0; // Tudo certo, nada a fazer.
}


int Insere(TNo** ppRaiz, TItem * x) { 
    /*Ela usa a recursividade de um jeito especial: ela tem uma viagem de ida (descendo pela árvore
    para achar um buraco vazio) e uma viagem de volta (subindo de volta para a raiz, checando se a 
    adição do nó novo entortou algum galho pelo caminho).*/
    
    if (*ppRaiz == NULL) { // Desceu até bater no NULL? Achou a vaga! Ele aloca a memória, coloca o pacote x 
                           // lá dentro, e avisa que a missão de inserir acabou
        // Fazendo na mão...
        *ppRaiz = (TNo*)malloc(sizeof(TNo));
        (*ppRaiz)->item = *x;
        (*ppRaiz)->pEsq = NULL;
        (*ppRaiz)->pDir = NULL;
        return 1; // O return 1 aqui é crucial: é um grito para o nó de cima dizendo "Ei, eu cresci! Checa a sua altura aí!".
    }
    // Agora o mergulhador precisa nadar de volta para a superfície, passando por todos os nós que ele visitou na descida.
    else if ( (*ppRaiz)->item.Chave > x->Chave ) { // Se a chave é menor, ele manda a função Insere 
                                                   // descer para a esquerda.
        if ( Insere(&(*ppRaiz)->pEsq,x) ) {
            /*Fique aqui parado esperando o mergulhador voltar. Se ele voltar gritando 1 (que o galho cresceu), 
            acorde e faça alguma coisa!"*/
            if (Balanceamento(ppRaiz)) // precisa de chamar o balanceador novamente
                return 0;
            else
            return 1;
        }
        /*A função Insere não faz tudo de uma vez. Primeiro ela desce até achar a vaga. Depois, ela vem 
        subindo de ré, parando em cada nó e perguntando: "Você ficou desbalanceado? 
        Você ficou desbalanceado?". Se alguém ficou, ela gira a árvore na mesma hora.*/
    }
    else if ( (*ppRaiz)->item.Chave < x->Chave ) { // Se a chave é menor, ele manda a função Insere 
                                                   // descer pela direita
        if ( Insere(&(*ppRaiz)->pDir,x) ) {
            // Volta de ré passando por cada nó
            if (Balanceamento(ppRaiz)) // se desbalanceou chama a função para balancear novamente
            return 0;
            else
            return 1;
        }
        else
            return 0;
    }
    return 0; // Chegou no final sem ser NULL, sem ser maior e sem ser menor? É porque você tentou 
              // inserir um número que já existe na árvore.
}

int Remove (TNo** ppRaiz, TItem* pX) { // Exatamente como na inserção, o mergulhador vai descendo procurando o alvo
    if (*ppRaiz == NULL) 
        return 0; // Chegou no fundo e não achou? Aborta!
    if ( pX->Chave < (*ppRaiz)->item.Chave ) { // É menor? Mergulha pra esquerda!
        if (Remove(&(*ppRaiz)->pEsq, pX)) {
            Balanceamento(ppRaiz); // Rebalança na volta
            return 1;
        }
        return 0;
    }
    else if ( pX->Chave > (*ppRaiz)->item.Chave ) { // É maior? Mergulha pra direita!
        if (Remove(&(*ppRaiz)->pDir, pX)) {
            Balanceamento(ppRaiz); // Rebalança na volta!
            return 1;
        }
        return 0;
    }else{
        // Salva o item para devolver pro usuário (opcional, mas bom)
        *pX = (*ppRaiz)->item; 
        
        TNo *pAux = *ppRaiz; // Segura a caixinha velha

        // Cenário A: Não tem filho na Esquerda (Pode ser folha ou ter 1 na direita)
        if ( (*ppRaiz)->pEsq == NULL ) {
            *ppRaiz = (*ppRaiz)->pDir; // O avô adota o filho da direita (ou NULL)
            free(pAux);
        }
        // Cenário B: Não tem filho na Direita (Tem 1 na esquerda)
        else if ( (*ppRaiz)->pDir == NULL ) {
            *ppRaiz = (*ppRaiz)->pEsq; // O avô adota o filho da esquerda
            free(pAux);
        }
        // Cenário C: Tem os 2 filhos! (Hora de chamar o substituto)
        else {
            // Chama a função Antecessor (agora sim é seguro, pois sabemos que a esquerda existe!)
            Antecessor(*ppRaiz, &((*ppRaiz)->pEsq));
        }
        
        // Fase 3: A Fisioterapia final do nó alterado
        if (*ppRaiz != NULL) {
            Balanceamento(ppRaiz);
        }
        return 1; // Avisa o andar de cima que o galho encolheu!
    }
}
void Imprime(TNo *pRaiz) {
    if (pRaiz != NULL) {
        printf("%ld ", pRaiz->item.Chave);
        Imprime(pRaiz->pEsq);
        Imprime(pRaiz->pDir);
    }
}

int main() {
    TArvoreAVL raiz;
    TArvore_Inicia(&raiz); // Inicia a árvore vazia
    TItem item;

    printf("--- TESTE DA ARVORE AVL ---\n");
    
    // Inserindo números que forçam uma Rotação (10, 20, 30)
    printf("Inserindo 10, 20 e 30...\n");
    item.Chave = 10; Insere(&raiz, &item);
    item.Chave = 20; Insere(&raiz, &item);
    item.Chave = 30; Insere(&raiz, &item);

    // Se a AVL funcionou, o 20 tem que ter subido para a raiz!
    printf("Imprimindo a arvore: ");
    Imprime(raiz);
    printf("\n");

    return 0;
}