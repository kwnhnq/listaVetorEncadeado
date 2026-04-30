#include <stdio.h>
#include <stdlib.h>

#define V int

struct _no;
typedef struct _no no;

struct _no {
    V valor;
    no* proximo;
};

struct _lista;
typedef struct _lista lista;

struct _lista {
    no* inicio;
    no* fim;
    int n;
};

lista criar_lista() {
    lista l;
    l.inicio = NULL;
    l.fim    = NULL;
    l.n      = 0;
    return l;
}

void liberar_lista(lista* l) {
    no* atual = l->inicio;
    while (atual != NULL) {
        no* prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    l->inicio = NULL;
    l->fim    = NULL;
    l->n      = 0;
}

static no* criar_no(V v) {
    no* novo = (no*) malloc(sizeof(no));
    if (novo == NULL) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
    novo->valor    = v;
    novo->proximo  = NULL;
    return novo;
}

void inserir_fim(lista* l, V v) {
    no* novo = criar_no(v);
    if (l->inicio == NULL) {
        l->inicio = novo;
        l->fim    = novo;
    } else {
        l->fim->proximo = novo;
        l->fim          = novo;
    }
    l->n++;
}

void inserir_inicio(lista* l, V v) {
    no* novo = criar_no(v);
    if (l->inicio == NULL) {
        l->inicio = novo;
        l->fim    = novo;
    } else {
        novo->proximo = l->inicio;
        l->inicio     = novo;
    }
    l->n++;
}

void inserir_em(lista* l, V v, int i) {
    if (i < 0 || i > l->n) {
        printf("Erro: posição %d inválida.\n", i);
        return;
    }
    if (i == 0) {
        inserir_inicio(l, v);
        return;
    }
    if (i == l->n) {
        inserir_fim(l, v);
        return;
    }
    no* novo  = criar_no(v);
    no* atual = l->inicio;
    for (int k = 0; k < i - 1; k++)
        atual = atual->proximo;
    novo->proximo  = atual->proximo;
    atual->proximo = novo;
    l->n++;
}

void alterar(lista* l, V v, int i) {
    if (i < 0 || i >= l->n) {
        printf("Erro: posição %d inválida.\n", i);
        return;
    }
    no* atual = l->inicio;
    for (int k = 0; k < i; k++)
        atual = atual->proximo;
    atual->valor = v;
}

V obter_primeiro(lista* l) {
    if (l->inicio == NULL) {
        printf("Erro: a lista está vazia.\n");
        return (V) 0;
    }
    return l->inicio->valor;
}

V obter_ultimo(lista* l) {
    if (l->fim == NULL) {
        printf("Erro: a lista está vazia.\n");
        return (V) 0;
    }
    return l->fim->valor;
}

V obter_em(lista* l, int i) {
    if (i < 0 || i >= l->n) {
        printf("Erro: posição %d inválida.\n", i);
        return (V) 0;
    }
    no* atual = l->inicio;
    for (int k = 0; k < i; k++)
        atual = atual->proximo;
    return atual->valor;
}

V remover_primeiro(lista* l) {
    if (l->inicio == NULL) {
        printf("Erro: a lista está vazia.\n");
        return (V) 0;
    }
    no* removido = l->inicio;
    V valor      = removido->valor;
    l->inicio    = removido->proximo;
    if (l->inicio == NULL)
        l->fim = NULL;
    free(removido);
    l->n--;
    return valor;
}

V remover_ultimo(lista* l) {
    if (l->inicio == NULL) {
        printf("Erro: a lista está vazia.\n");
        return (V) 0;
    }
    V valor;
    if (l->inicio == l->fim) {
        valor     = l->inicio->valor;
        free(l->inicio);
        l->inicio = NULL;
        l->fim    = NULL;
        l->n--;
        return valor;
    }
    no* atual = l->inicio;
    while (atual->proximo != l->fim)
        atual = atual->proximo;
    valor          = l->fim->valor;
    free(l->fim);
    l->fim         = atual;
    atual->proximo = NULL;
    l->n--;
    return valor;
}

V remover_em(lista* l, int i) {
    if (i < 0 || i >= l->n) {
        printf("Erro: posição %d inválida.\n", i);
        return (V) 0;
    }
    if (i == 0)        return remover_primeiro(l);
    if (i == l->n - 1) return remover_ultimo(l);

    no* anterior = l->inicio;
    for (int k = 0; k < i - 1; k++)
        anterior = anterior->proximo;
    no* removido       = anterior->proximo;
    V valor            = removido->valor;
    anterior->proximo  = removido->proximo;
    free(removido);
    l->n--;
    return valor;
}

int tamanho_lista(lista* l) {
    return l->n;
}

int lista_vazia(lista* l) {
    return l->n == 0;
}

void limpar_lista(lista* l) {
    liberar_lista(l);
}

void imprimir(lista* l) {
    no* atual = l->inicio;
    printf("[ ");
    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }
    printf("]\n");
}

void imprime_desc(lista* l) {
    void aux(no* n) {
        if (n == NULL) return;
        aux(n->proximo);
        printf("%d ", n->valor);
    }
    printf("[ ");
    aux(l->inicio);
    printf("]\n");
}

void inserir_ordenado(lista* l, V v) {
    if (l->inicio == NULL || v <= l->inicio->valor) {
        inserir_inicio(l, v);
        return;
    }
    no* atual = l->inicio;
    int pos   = 0;
    while (atual->proximo != NULL && atual->proximo->valor < v) {
        atual = atual->proximo;
        pos++;
    }
    inserir_em(l, v, pos + 1);
}

int lista_ordenada(lista* l) {
    if (l->inicio == NULL) return 1;
    no* atual = l->inicio;
    while (atual->proximo != NULL) {
        if (atual->valor > atual->proximo->valor)
            return 0;
        atual = atual->proximo;
    }
    return 1;
}

int lista_fibonacci(lista* l) {
    if (l->inicio == NULL) return 1;
    if (l->n == 1) {
        return (l->inicio->valor == 0 || l->inicio->valor == 1);
    }
    no* a = l->inicio;
    no* b = a->proximo;
    if (!((a->valor == 0 && b->valor == 0) ||
          (a->valor == 0 && b->valor == 1) ||
          (a->valor == 1 && b->valor == 1) ||
          (a->valor == 1 && b->valor == 2))) {
    }
    no* atual = b->proximo;
    while (atual != NULL) {
        if (atual->valor != a->valor + b->valor)
            return 0;
        a     = b;
        b     = atual;
        atual = atual->proximo;
    }
    int fa = 0, fb = 1;
    int v0 = l->inicio->valor;
    int v1 = l->inicio->proximo->valor;
    while (fb < v0) { int tmp = fa + fb; fa = fb; fb = tmp; }
    if (fa != v0 && fb != v0) return 0;
    if (fa == v0 && fb != v1) return 0;
    if (fb == v0 && (fa + fb) != v1 && v1 != fa) return 0;
    return 1;
}

int buscar_posicao(lista* l, V v) {
    no* atual = l->inicio;
    int pos   = 0;
    while (atual != NULL) {
        if (atual->valor == v)
            return pos;
        atual = atual->proximo;
        pos++;
    }
    return -1;
}

int contem_todos(lista* l, lista* a) {
    no* atual = a->inicio;
    while (atual != NULL) {
        if (buscar_posicao(l, atual->valor) == -1)
            return 0;
        atual = atual->proximo;
    }
    return 1;
}

int listas_iguais(lista* l, lista* a) {
    if (l->n != a->n) return 0;
    return contem_todos(l, a) && contem_todos(a, l);
}

lista uniao(lista* l, lista* a) {
    lista resultado = criar_lista();
    no* atual = l->inicio;
    while (atual != NULL) {
        inserir_fim(&resultado, atual->valor);
        atual = atual->proximo;
    }
    atual = a->inicio;
    while (atual != NULL) {
        if (buscar_posicao(&resultado, atual->valor) == -1)
            inserir_fim(&resultado, atual->valor);
        atual = atual->proximo;
    }
    return resultado;
}

lista interseccao(lista* l, lista* a) {
    lista resultado = criar_lista();
    no* atual = l->inicio;
    while (atual != NULL) {
        if (buscar_posicao(a, atual->valor) != -1)
            inserir_fim(&resultado, atual->valor);
        atual = atual->proximo;
    }
    return resultado;
}

lista diferenca(lista* l, lista* a) {
    lista resultado = criar_lista();
    no* atual = l->inicio;
    while (atual != NULL) {
        if (buscar_posicao(a, atual->valor) == -1)
            inserir_fim(&resultado, atual->valor);
        atual = atual->proximo;
    }
    return resultado;
}

int main() {
    printf("=== TESTES DA LISTA ENCADEADA ===\n\n");

    lista l = criar_lista();
    printf("Lista vazia? %d (esperado: 1)\n", lista_vazia(&l));

    inserir_fim(&l, 10);
    inserir_fim(&l, 20);
    inserir_fim(&l, 30);
    inserir_inicio(&l, 5);
    inserir_em(&l, 15, 2);

    printf("Lista após inserções: ");
    imprimir(&l);

    printf("Impressão invertida: ");
    imprime_desc(&l);

    printf("Primeiro: %d (esperado: 5)\n",  obter_primeiro(&l));
    printf("Último:   %d (esperado: 30)\n", obter_ultimo(&l));
    printf("Posição 2: %d (esperado: 15)\n", obter_em(&l, 2));

    alterar(&l, 99, 2);
    printf("Após alterar posição 2 para 99: ");
    imprimir(&l);
    alterar(&l, 15, 2);

    printf("Removido primeiro: %d (esperado: 5)\n",  remover_primeiro(&l));
    printf("Removido último:   %d (esperado: 30)\n", remover_ultimo(&l));
    printf("Removido posição 1: %d (esperado: 15)\n", remover_em(&l, 1));
    printf("Lista após remoções: ");
    imprimir(&l);

    printf("Tamanho: %d (esperado: 2)\n", tamanho_lista(&l));

    inserir_em(&l, 999, 10);

    limpar_lista(&l);
    printf("Após limpar — lista vazia? %d (esperado: 1)\n\n", lista_vazia(&l));

    printf("--- Inserção ordenada ---\n");
    lista ord = criar_lista();
    inserir_ordenado(&ord, 10);
    inserir_ordenado(&ord, 3);
    inserir_ordenado(&ord, 7);
    inserir_ordenado(&ord, 1);
    inserir_ordenado(&ord, 15);
    printf("Ordenada: ");
    imprimir(&ord);
    printf("Está ordenada? %d (esperado: 1)\n\n", lista_ordenada(&ord));

    printf("--- Fibonacci ---\n");
    lista fib = criar_lista();
    inserir_fim(&fib, 0);
    inserir_fim(&fib, 1);
    inserir_fim(&fib, 1);
    inserir_fim(&fib, 2);
    inserir_fim(&fib, 3);
    inserir_fim(&fib, 5);
    inserir_fim(&fib, 8);
    printf("Lista Fibonacci: ");
    imprimir(&fib);
    printf("É Fibonacci? %d (esperado: 1)\n", lista_fibonacci(&fib));
    alterar(&fib, 99, 3);
    printf("Após alterar posição 3 para 99 — É Fibonacci? %d (esperado: 0)\n\n",
           lista_fibonacci(&fib));

    printf("--- Busca ---\n");
    printf("Posição do 5: %d (esperado: -1, pois foi alterado)\n", buscar_posicao(&fib, 5));
    printf("Posição do 1: %d (esperado: 1 ou 2)\n\n", buscar_posicao(&fib, 1));

    printf("--- Operações de conjunto ---\n");
    lista la = criar_lista();
    lista lb = criar_lista();

    int va[] = {5, 15, 8, 13, 90, 42, 7};
    int vb[] = {18, 5, 42, 13, 10, 8, 9, 50, 61};

    for (int i = 0; i < 7; i++) inserir_fim(&la, va[i]);
    for (int i = 0; i < 9; i++) inserir_fim(&lb, vb[i]);

    printf("l: "); imprimir(&la);
    printf("a: "); imprimir(&lb);

    lista u = uniao(&la, &lb);
    printf("União:      "); imprimir(&u);

    lista inter = interseccao(&la, &lb);
    printf("Interseção: "); imprimir(&inter);

    lista dif = diferenca(&la, &lb);
    printf("Diferença:  "); imprimir(&dif);

    printf("\nContém todos (l contém a)? %d (esperado: 0)\n", contem_todos(&la, &lb));
    printf("Listas iguais?             %d (esperado: 0)\n",   listas_iguais(&la, &lb));
    printf("Listas iguais (l,l)?       %d (esperado: 1)\n",   listas_iguais(&la, &la));

    liberar_lista(&la);
    liberar_lista(&lb);
    liberar_lista(&u);
    liberar_lista(&inter);
    liberar_lista(&dif);
    liberar_lista(&ord);
    liberar_lista(&fib);

    printf("\nTodos os testes concluídos.\n");
    return 0;
}