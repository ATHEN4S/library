#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pessoa {
  char *user;
  char *password;
  struct pessoa *next;
} tUsuario;

typedef struct livro {
  char *livros;
  char *disp;
  struct livro *next;
} tLivro;

int checagem_livro(const tLivro livro, const tLivro *plivro);
int checagem_usuario(const tUsuario login, const tUsuario *pLogin, int *autorizar);
int colocar_usuario(tUsuario usuario, tUsuario *tusuario);
int login(const tUsuario user, const tUsuario *pUser);
void mudarIND(char *ind, int line);
void mudarDIS(char *disp, int line);

int main() {
  tUsuario Usuario, Senha;
  tLivro Book;
  char quest_cadastro;
  int voltar = 0;

  do {
    printf("\n       Bem vindo a Biblioteca Athenas!\n\n");
    printf("Digite se você já tem cadastro(s/n)\n");
    printf("     (Digite 1 para sair)\n");
    printf(" ------> ");
    scanf(" %c", &quest_cadastro);
    quest_cadastro = tolower(quest_cadastro);
  } while (quest_cadastro != 's' && quest_cadastro != 'n' &&
           quest_cadastro != '1');

  if (quest_cadastro == '1') {
    printf("\n Você saiu da Biblioteca. Até a próxima!");
    return 0;
  }

  if (quest_cadastro == 'n') {
    int autorizar = 0;

    do {
      Usuario.user = calloc(30, sizeof(char *));
      printf("\nDigite o seu Usuário: ");
      scanf(" %s", Usuario.user);

      checagem_usuario(Usuario, &Usuario, &autorizar);
    } while (autorizar == 0 || strlen(Usuario.user) > 20);

    Usuario.password = calloc(30, sizeof(char *));
    do {
      printf("\nDigite a sua senha: ");
      scanf(" %s", Usuario.password);
    } while (strlen(Usuario.password) > 20);

    colocar_usuario(Usuario, &Usuario); // Colocando o usuario e senha no arquivo txt

    free(Usuario.password);
    free(Usuario.user);
  }

  else if (quest_cadastro == 's') {
    int result = 0;

    Usuario.user = calloc(30, sizeof(char *));
    printf("\n Usuário: ");
    scanf(" %s", Usuario.user);

    Usuario.password = calloc(20, sizeof(char *));
    printf("\n Senha: ");
    scanf(" %s", Usuario.password);

    result = login(Usuario, &Usuario);

    // Se o login for feito:
    while (result == 1) {
      int pegar_devolver;

      printf("\n \n Bem vindo, %s! \n", Usuario.user);
      printf("\n\n O que deseja fazer hoje?\n 1) Pegar um livro. \n 2) Devolver "
             "um livro. \n 3) Conferir data de devolução. \n 4) Sair \n");
      printf("\n ------> ");
      scanf(" %d", &pegar_devolver);

      switch (pegar_devolver) {
      
      char livroprocurado[100];
      case 1:

        Book.livros = calloc(200, sizeof(char*));
        printf("\n\n Digite o título da obra:");
        printf("\n (Informe o título entre aspas)");
        printf("\n ------> ");
        scanf(" %[^\n]", Book.livros);
        printf(" ue %s", Book.livros);

        checagem_livro(Book, &Book);
        break;
        

      case 2:
        printf("\n DEVOLUÇÃO \n \n");
        //mudarDIS(,);
        break;

      case 3:
        printf("\n DATA DE DEVOLUÇÃO \n \n");
        break;

      case 4:
        printf("\n\n Saindo! Volte sempre");
        return 0;

      default:
        printf("\n OPÇÃO INVÁLIDA\n \nEscolha uma das opções disponíveis.");
        voltar = 1;
        break;

      if (voltar == 1)
        break;
      else
      continue;
      }
    }

    if (result == -1)
      printf("Falha ao abrir arquivo\n");

    else
      printf("\n As informações dadas não batem com uma conta em nosso "
             "sistema... Confira seus dados e tente novamente! \n");
  }

  printf("Saindo do Programa...");

  return 0;
}

int checagem_usuario(const tUsuario login, const tUsuario *pLogin,
                     int *autorizar) {
  FILE *check_user;

  check_user = fopen("login_usuario.txt", "r");

  if (!check_user) {
    printf("Can't open file\n");
    return 1;
  }

  else {
    char buffer[100];
    int row = 0;

    // Splitting the data
    while (fgets(buffer, 100, check_user)) {
      row++;

      char *value = strtok(buffer, ", "); // Vai ser só o "user" do csv
      printf("\nrow: %d\n", row);

      if (row == 1)
        continue;

      printf("login: %s\n", pLogin->user);
      printf("value: %s\n", value);

      if (strcmp(pLogin->user, value) == 0) {
        printf("\nO usuário já existe, tente novamente\n");
        *autorizar = 0;
        break;
      } else
        *autorizar = 1;
    }
  }
  // Close the file
  fclose(check_user);
  return 0;
}

int colocar_usuario(tUsuario usuario, tUsuario *tusuario) {
  FILE *user_in_txt_a;
  FILE *user_in_txt_r;
  FILE *status;

  user_in_txt_a = fopen("login_usuario.txt", "a");
  user_in_txt_r = fopen("login_usuario.txt", "r");
  status = fopen("leitores.txt", "a");

  if (!user_in_txt_r || !status) {
    printf("Can't open file\n");
    return 1;
  } else {
    char buffer[50];
    int column = 0;
    char *comma = ", ";
    char destination[50] = "";

    strcat(destination, tusuario->user);
    strcat(destination, comma);
    strcat(destination, tusuario->password);
    fprintf(user_in_txt_a, "\n%s", destination, buffer);

    // Já para o status_user
    char leitor[50] = "";
    char *status_usuario = "disponivel";
    char *livros = "";
    char *devolver = "";
    char *lista_desejos = "";

    strcat(leitor, tusuario->user);
    strcat(leitor, comma);
    strcat(leitor, status_usuario);
    strcat(leitor, comma);
    strcat(leitor, livros);
    strcat(leitor, comma);
    strcat(leitor, devolver);
    strcat(leitor, comma);
    strcat(leitor, lista_desejos);

    fprintf(status, "\n%s", leitor, buffer);
  }

  printf("\nUsuário criado com sucesso !!\n");
  fclose(user_in_txt_a);
  fclose(user_in_txt_r);
  fclose(status);
  return 0;
}

int login(const tUsuario user, const tUsuario *pUser) {
  FILE *login_txt_r;
  login_txt_r = fopen("login_usuario.txt", "r");
  int find = 0;
  tLivro Livro;

  if (!login_txt_r) {
    printf("Can't open file\n");
    return -1;
  } else {
    char buffer[100];
    int line = 0;
    while (fgets(buffer, sizeof(buffer), login_txt_r)) {
      line++;
      char *user1 = strtok(buffer, ", ");
      char *senha1 = strtok(NULL, ", ");

      // Usando concatenação de string
      char pass[30] = "";
      char *n = "\n";

      strcat(pass, pUser->password);
      strcat(pass, n);

      if ((strcmp(pUser->user, user1) == 0) &&
          ((strcmp(pass, senha1) == 0) ||
           (strcmp(pUser->password, senha1) ==
            0))) // Se usuario e senha se correspondem...
      {
        fclose(login_txt_r);
        return 1;
      }

      if (line == sizeof(buffer)) {
        fclose(login_txt_r);
        return 0;
      }
    }
  }
}

int checagem_livro(const tLivro livro, const tLivro *plivro) {
  FILE *livro_a;
  livro_a = fopen("livros.txt", "r");

  char buffer[1000];
  int row = 1, col = 0;
  int marcador = 0;
  int linha_count = 0;
  
  while (fgets(buffer, (sizeof(buffer)), livro_a)) {
    col = 0;
    row++;
    char frase[1000];

    strcpy(frase, buffer);
    printf("\n%s 1111", frase);
    
    char *livroprocurado = strtok(buffer, ",");
    printf("\n%s", livroprocurado);
    if (row == 1) {
      livroprocurado == NULL;
      continue;
    }

    while (livroprocurado != NULL) {
      col++;

      if (col == 2) {
        linha_count = linha_count+1;
        
        if (strcmp(plivro->livros, livroprocurado) == 0) {
          printf("\n TITULO: %s", plivro->livros);
          fflush(stdin);
          marcador = 1;
        }
        else
          marcador = marcador + 2;
        
      }

      if (col == 4) {
        printf("\n");
        if (marcador == 1) {
          printf("\n STATUS: %s\n", livroprocurado);
          printf("DIF: %d", strcmp(livroprocurado, "/DISPONIVEL"));
          if (strcmp(livroprocurado, "/DISPONIVEL") == 10){
            printf("\nSTATUS: %s\n", livroprocurado);
            printf("\nDeseja pegar o livro?");
            char emprestimo;
            scanf(" %c", &emprestimo);
            marcador = 0;
  
            if (emprestimo == 's') {
            printf("\n LINHAS: %d",linha_count) ;
            printf("\n FRASE: %s",frase) ;
            mudarIND(frase, linha_count);
  
           struct tm *data_hora_atual;
  
           time_t segundos;
      
          time(&segundos);   
    
          data_hora_atual = localtime(&segundos);  
   
          printf("\nData de emprestimo:  %d/", data_hora_atual->tm_mday);
          printf("%d/",data_hora_atual->tm_mon+1); //mês
          printf("%d\n\n",data_hora_atual->tm_year+1900); //ano
  
          printf("\nData de devolução:  %d/", data_hora_atual->tm_mday);
          if((data_hora_atual->tm_mon+1)==12) {   
          printf("%d/",data_hora_atual->tm_mon-10);
          printf("%d\n\n",data_hora_atual->tm_year+1901);
            }else{
                printf("%d/",data_hora_atual->tm_mon+2);
                printf("%d\n\n",data_hora_atual->tm_year+1900);
              }
          
            }
            }

      else{
        printf("Esse livro já foi emprestado. Escolha outro, ou tente novamente mais tarde.");
          break;
          }

        }
        
        printf("\n MARCADOR: %d ", marcador);
        printf("\n ROW: %d ", row);
        break;
      }

      livroprocurado = strtok(NULL, ",");
    }
    }
    
    if (marcador == (row-1)*2){
      printf("\n \n Não achamos esse livro. Atente-se às aspas e ao case das palavras! Verifique o título e tente novamente.");
  }
  return 0;
}

void mudarIND(char *ind, int line) {
  
    FILE *fp;
    FILE *fv;
    int count=0;  //count number lines in source file.
    int ch;   //temporary place to store character of source file(one at a time).
    int edited=0;  //0=false and 1=true
    char *t = ",/INDISPONIVEL";   //temporary place to store input which you want to be replaced with error in text file.
    char *ponivel = ind;
    char buffer[1000];
    
    printf("AAAAAAA %s", ponivel);
    //printf("BBBBBBB %s", *&disponi);
    fp=fopen("livros.txt","r");
    fv=fopen("target.txt","w");


    size_t n = strlen(ponivel) + (strlen(t)) + 10;
    char *s = malloc(n);
    

    printf("PONIVEL: %s", s);
  
    if(fp==NULL||fv==NULL){
        printf("\nErro... Não foi possível abrir os arquivos");
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        count++;
        printf("COUNT: %d", count);
        
       if(count==line){
            char *disponibilidade = strtok(buffer, "/");
            printf("PONIVEL_NEW: %s", disponibilidade);
            strcpy(s, disponibilidade); 
            s[strlen(s) - 1] = '\0';
            strcat(s, t);
            s[strlen(s)] = '\n';
            fflush(stdin);
            fprintf(fv,"%s", s);   
      
            printf("\n BBBB %s", disponibilidade);
            edited=1;
           }
       else fprintf(fv,"%s", buffer);
    }
      
    fclose(fp);
    fclose(fv);

    if(edited==1)
        printf("\nCongrates...Error Edited Successfully.");
    else
        printf("\nLinha não encontrada.");

  free(s);
  remove("livros.txt");
  rename("target.txt","livros.txt");
}

void mudarDIS(char *disp, int line) {
  
    FILE *fp;
    FILE *fv;
    int count=0;  //count number lines in source file.
    int ch;   //temporary place to store character of source file(one at a time).
    int edited=0;  //0=false and 1=true
    char *t = ",/DISPONIVEL";   //temporary place to store input which you want to be replaced with error in text file.
    char *ponivel = disp;
    char buffer[1000];
    
    fp=fopen("livros.txt","r");
    fv=fopen("target.txt","w");


    size_t n = strlen(ponivel) + (strlen(t)) + 10;
    char *s = malloc(n);
    
  
    if(fp==NULL||fv==NULL){
        printf("\nErro... Não foi possível abrir os arquivos");
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        count++;
        
       if(count==line){
            char *disponibilidade = strtok(buffer, "/");
            strcpy(s, disponibilidade); 
            s[strlen(s) - 1] = '\0';
            strcat(s, t);
            s[strlen(s)] = '\n';
            fflush(stdin);
            fprintf(fv,"%s", s); 
         
            edited=1;
           }
       else fprintf(fv,"%s", buffer);
    }
      
    fclose(fp);
    fclose(fv);

    if(edited==1)
        printf("\n Livro devolvido com sucesso!");
    else
        printf("\nLinha não encontrada.");

  free(s);
  remove("livros.txt");
  rename("target.txt","livros.txt");
}
