#include <stdio.h>
#include "arvore_enc.h"

int main() {
    Tad* t = criar_no(8);
    adicionar_no(t,13);
    adicionar_no(t,12);
    adicionar_no(t,11);
    adicionar_no(t,5);
    adicionar_no(t,0);
    adicionar_no(t,1);
    adicionar_no(t,2);
    adicionar_no(t,6);
    adicionar_no(t,7);
    pre_ordem(t);
    printf("\n\n");
    remover(&t, 5);
    pre_ordem(t);
    return 0;
}
