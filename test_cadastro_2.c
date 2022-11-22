#include <stdio.h>
#include <stdlib.h>>

/* Session 1 - Struct para login e cadastro,  informações essenciais*/
typedef struct login_us{
  char user[10];
  char senha[10];
  struct login_us* next;
} login;

void inserir(login **cabeca);

void listar (login *cabeca);


/* Session 2 - Parte do menu */
int main() {
  int op;
  login *cabeca = NULL;
  login *el_atual = NULL;

  do{
  
  printf("\n       Bem vindo a Biblioteca Athenas !\n\n");
  printf(" 1) Login\n 2) Cadastro\n 3) Sair\n" );
  printf("\n Insira a opção escolhida: " );
  scanf("%d", &op);

  switch(op){
    case 1:
    /* Login em desenvolvimento */
    break;

    case 2:
    inserir(&cabeca);
    break;

    case 3:
    printf("\n Você saiu da Biblioteca. Até mais!" );
    break;

    default: 
    printf("\n\n Opção não valida.");
    break;
  }

  }while((op == '1') && (op == '2') && (op == '3'));
/* Desaloca a memoria alocada para os elementos da lista */
  
el_atual = cabeca;
while (el_atual != NULL)
{
cabeca = el_atual->next;
free(el_atual);
el_atual = cabeca;
}
}
  
  
  /* Session 3 - inserir(Novo usuario) */
  void inserir(login**cabeca){
  login *el_atual, *novono;
  char user_nv[10];
  char senha_nv[10];
  printf("\n\nUsername: ");
  scanf("%s", user_nv);
  printf("Senha: ");
  scanf("%s", senha_nv);
  printf("\nSeu user é %s, e sua senha é %s, lembre-se deles depois! Vão ser importantes.", user_nv, senha_nv);
  
  if (*cabeca == NULL) /* Se ainda nao existe nenhum produto na lista */
  {
  *cabeca = (login *) malloc(sizeof(login));
  /*(*cabeca)->user = user_nv;*/
  strcpy((*cabeca) -> user, user_nv);
  /*(*cabeca)->senha = senha_nv;*/
  strcpy((*cabeca) -> senha, senha_nv);
  (*cabeca)->next = NULL;
  }
    
  else
  {
  /* Se ja existem elementos na lista, deve percorre-la ate' o seu final e inserir o novo elemento */
  el_atual = *cabeca;
  while(el_atual->next != NULL)
  el_atual = el_atual->next; /* Ao final do while, noatual aponta para o ultimo no */
  novono = (login *) malloc(sizeof(login));/* Aloca memoria para o novo no */
  strcpy(novono -> user, user_nv);
  strcpy(novono -> user, user_nv);
  el_atual->next = novono; /* Faz o ultimo no apontar para o novo no */
  }
}
