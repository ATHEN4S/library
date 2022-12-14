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


int checagem_livro(const tLivro livro, tLivro *plivro, tUsuario *pUser);
int checagemdois(const tLivro livro, const tLivro *plivro);
int checagem_usuario(const tUsuario login, const tUsuario *pLogin, int *autorizar);
int colocar_usuario(tUsuario usuario, tUsuario *tusuario);
int login(const tUsuario user, const tUsuario *pUser);
void mudarIND(char *frase, int line);
void mudarDIS(char *disp, int line);
int adiciona_date(int dia, int mes, int ano, tUsuario *pUser, tLivro *plivro);
int remove_devol(tUsuario *pUser);
int print_data(tUsuario *pUser);

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
      char pegar_devolver;

      printf("\n \n Bem vindo, %s! \n", Usuario.user);
      printf("\n\n O que deseja fazer hoje?\n a) Pegar um livro. \n b) Devolver "
             "um livro. \n c) Conferir data de devolução. \n s) Sair \n");
      printf("\n ------> ");
      scanf(" %c", &pegar_devolver);

      switch (pegar_devolver) {
      
      char livroprocurado[100];
      case 'a':

        Book.livros = calloc(200, sizeof(char*));
        printf("\n\n Digite o título da obra:");
        printf("\n (Informe o título entre aspas)");
        printf("\n ------> ");
        scanf(" %[^\n]", Book.livros);
        printf("\n --> Livro Digitado:  %s", Book.livros);

        checagem_livro(Book, &Book, &Usuario);
        break;
        

      case 'b':
        printf("\n DEVOLUÇÃO \n \n");
        
        Book.livros = calloc(200, sizeof(char*));
        printf("\n\n Digite o título da obra que deseja devolver:");
        printf("\n (Informe o título entre aspas)");
        printf("\n ------> ");
        scanf(" %[^\n]", Book.livros);
        printf("\n --> Livro Digitado:  %s", Book.livros);

        checagemdois(Book, &Book);
        remove_devol(&Usuario);
       
        break;

      case 'c':
        printf("\n DATA DE DEVOLUÇÃO \n \n");
        print_data(&Usuario);
        break;

      case 's':
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
    printf("Não pôde abrir o arquivo\n");
    return 1;
  }

  else {
    char buffer[100];
    int row = 0;

    // Acessando os dados
    while (fgets(buffer, 100, check_user)) {
      row++;

      char *value = strtok(buffer, ", "); // Vai ser só o "user" do csv

      if (row == 1)
        continue;

      if (strcmp(pLogin->user, value) == 0) {
        printf("\nO usuário já existe, tente novamente\n");
        *autorizar = 0;
        break;
      } else
        *autorizar = 1;
    }
  }
  // Fechar o arquivo
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
    printf("Não foi possível abrir o arquivo\n");
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
    printf("Não pôde abrir arquivo\n");
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
  return 0;
}

int checagem_livro(const tLivro livro, tLivro *plivro, tUsuario *pUser) //MUDAR PARA INDISPONIVEL, PEGAR EMPRESTADO
{
  FILE *livrotxt;
  struct tm *data_hora_atual;
  livrotxt = fopen("livros.txt", "r");
  tLivro Book;
  char buffer[1000];
  int row = 1, col = 0;
  int marcador = 0;
  int linha_count = 0;
  
  while (fgets(buffer, (sizeof(buffer)), livrotxt)) {
    col = 0;
    row++;
    char frase[1000];

    strcpy(frase, buffer);
    
    char *livroprocurado = strtok(buffer, ",");
    if (row == 1) {
      livroprocurado == NULL;
      continue;
    }

    while (livroprocurado != NULL) {
      col++;

      if (col == 2) {
        linha_count = linha_count+1;
  
        if (strcmp(plivro->livros, livroprocurado) == 0) {
          fflush(stdin);
          marcador = 1;
        }
        else
          marcador = marcador + 2;
        
      }

      if (col == 4) {
        if (marcador == 1) {
          if (strcmp(livroprocurado, "/DISPONIVEL\n") == 0)
          {
            printf("\nSTATUS: %s\n", livroprocurado);
            printf("\nDeseja pegar o livro(s/n)?\n");
            printf("---> ");
            char emprestimo;
            scanf(" %c", &emprestimo);
            marcador = 0;
  
            if (emprestimo == 's')
            {
              printf("\n FRASE: %s",frase);
              mudarIND(frase, linha_count);
    
    
              time_t segundos;
          
              time(&segundos);   
        
              data_hora_atual = localtime(&segundos);  
    
              
              printf("\nData de emprestimo:  %d/", data_hora_atual->tm_mday);
              printf("%d/",data_hora_atual->tm_mon+1); //mês
              printf("%d\n\n",data_hora_atual->tm_year+1900); //ano
              
              printf("\nData de devolução:  %d/", data_hora_atual->tm_mday);
              
              if((data_hora_atual->tm_mon+1)==12)
              {
                data_hora_atual->tm_mon = data_hora_atual->tm_mon-10;
                printf("%d/",data_hora_atual->tm_mon);
                data_hora_atual->tm_year = data_hora_atual->tm_year+1901;
                printf("%d\n\n",data_hora_atual->tm_year);
              }
              else
              {
                data_hora_atual->tm_mon = data_hora_atual->tm_mon+2;
                printf("%d/",data_hora_atual->tm_mon);
                data_hora_atual->tm_year = data_hora_atual->tm_year+1900;
                printf("%d\n\n",data_hora_atual->tm_year);
              }

              int d, m, ano;
              d = data_hora_atual->tm_mday;
              m = data_hora_atual->tm_mon;
              ano = data_hora_atual->tm_year;
              
            adiciona_date(d, m, ano, pUser, plivro);
            break;
            }
          break;
        }

      else{
        printf("Esse livro já foi emprestado. Escolha outro, ou tente novamente mais tarde.");
          break;
          }

        }
      }

      livroprocurado = strtok(NULL, ",");
    }
  }
    
    if (marcador == (row-1)*2){
      printf("\n \n Não achamos esse livro. Atente-se às aspas e ao case das palavras! Verifique o título e tente novamente.");
  }
  return 0;
}

int checagemdois(const tLivro livro, const tLivro *plivro) {
  FILE *livro_a;
  livro_a = fopen("livros.txt", "r");

  char buffer[1000];
  int row = 1, col = 0;
  int marca = 0;
  int linha_count = 0;
  
  while (fgets(buffer, (sizeof(buffer)), livro_a)) {
    col = 0;
    row++;
    char frase[1000];

    strcpy(frase, buffer);
    
    char *livroprocurado = strtok(buffer, ",");
    if (row == 1) {
      livroprocurado == NULL;
      continue;
    }

    while (livroprocurado != NULL)
    {
      col++;
      if (col == 2)
      {
        linha_count = linha_count+1;
        
        if (strcmp(plivro->livros, livroprocurado) == 0)
        {
          marca = 1;
          fflush(stdin);
        }
        else
          marca = marca + 2;
      }
      

      if (col == 4)
      {
        if (marca == 1)
        {
          mudarDIS(frase, linha_count);
        }
      }
      
      livroprocurado = strtok(NULL, ",");
    }
  }
   
  return 0;
}//MUDAR PARA DISPONIVEL, DEVOLVER
      
void mudarIND(char *frase, int line) { //MUDAR PARA INDISPONIVEL, PEGAR EMPRESTADO
  
    FILE *fp;
    FILE *fv;
    int count=0;  //Contar o numero das linhas no txt
    int edited=0;  //como bool, 0=Falso, 1=Verdadeiro
    char *mudar = ",/INDISPONIVEL";   //String que queremos adicionar no final do livro selecionado.
    char *ponivel = frase; // Ponteiro para a frase passada.
    char buffer[1000];
    
    fp=fopen("livros.txt","r");
    fv=fopen("target.txt","w");

    size_t n = strlen(ponivel) + (strlen(mudar)) + 10;
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
            strcat(s, mudar);
            char *linha_nova = "\n";
            strcat(s, linha_nova);
            
            printf("\ns: %s", s);
            fprintf(fv,"%s", s);   
      
            edited=1;
           }
         
       else fprintf(fv,"%s", buffer);
    }

    if(edited==1)
        printf("\nLivro emprestado com sucesso ! Boa leitura.");
    else
        printf("\nLinha não encontrada.");


  printf("\nCHEGOU AQUI");
  free(s);
  remove("livros.txt");
  rename("target.txt","livros.txt");
  fclose(fp);
  fclose(fv);
}

void mudarDIS(char *disp, int line) {
  
    FILE *fp;
    FILE *fv;
    int count=0;  //count number lines in source file.
    int edited=0;  //0=false and 1=true
    char *devolver = ",/DISPONIVEL";   //temporary place to store input which you want to be replaced with error in text file.
    char *ponivel = disp;
    char buffer[1000];
    
    fp=fopen("livros.txt","r");
    fv=fopen("target.txt","w");


    size_t n = strlen(ponivel) + (strlen(devolver)) + 10;
    char *s = malloc(n);
    
  
    if(fp==NULL||fv==NULL){
        printf("\nErro... Não foi possível abrir os arquivos");
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        count++;
        
       if(count==line){
            char *disponibilidade = strtok(buffer, "/");
            printf("\nDISPONI: %s", disponibilidade);
            strcpy(s, disponibilidade); 
            s[strlen(s) - 1] = '\0';
            strcat(s, devolver);
            char *linha_nova = "\n";
            strcat(s, linha_nova);
            fprintf(fv,"%s \n", s); 
         
            edited=1;
           }
       else fprintf(fv,"%s", buffer);
    }
      
    fclose(fp);
    fclose(fv);

    if(edited==1)
        printf("\n\n Livro devolvido com sucesso!");
    else
        printf("\nLinha não encontrada.");

  free(s);
  remove("livros.txt");
  rename("target.txt","livros.txt");
}

int adiciona_date(int dia,int mes,int ano, tUsuario *pUser, tLivro *plivro)
{
  FILE *data_r;
  FILE *temp;
  
  int aut = 0;
  char buffer[1000];
  int col = 0;
  char data_imprimir[100];

  snprintf(data_imprimir, 100, "%d/%d/%d", dia,mes,ano);
  char *pd = data_imprimir;

  data_r = fopen("leitores.txt","r");
  temp = fopen("temp_colocar_data.txt","w");
  
  if(!data_r || !temp)
  {
    printf("\nNão foi possível abrir o arquivo");
    return 1;
  }
  else
  {
    while (fgets(buffer, sizeof(buffer), data_r))
    {
      col = 0;
      char *data = strtok(buffer, ",");
      char *comma = ",";
      char *status = " indisponível";
      
      while(data != NULL)
      {
        col++;
        if (col == 1 && strcmp(pUser->user, data) == 0)
        {
          fprintf(temp,"%s",data);
          fprintf(temp,comma);
          aut = 1;
        }

        else if (col == 2 && aut == 1)
        {
          fprintf(temp,"%s",status);
          fprintf(temp,comma);
        }

        else if (col == 3 && aut == 1)
        {
          fprintf(temp," %s",plivro->livros);
          fprintf(temp,comma);
        }
          
        else if(col == 4 && aut == 1)
        {
          data = data_imprimir;
          fprintf(temp,"%s",data_imprimir);
          fprintf(temp,comma);
          aut = 0;
        }
        else
        {
          fprintf(temp,"%s",data);
          fprintf(temp,comma);
        }
        data = strtok(NULL, ",");
      }
    }
  }

  fclose(data_r);
  fclose(temp);
  remove("leitores.txt");
  rename("temp_colocar_data.txt","leitores.txt");
  return 0;
}

int remove_devol(tUsuario *pUser)
{
  FILE *data_r;
  FILE *temp;
  
  int aut = 0;
  char buffer[1000];
  int col = 0;
  char *status = " disponível";


  data_r = fopen("leitores.txt","r");
  temp = fopen("temp_tirar_data.txt","w");
  
  if(!data_r || !temp)
  {
    printf("\nNão foi possível abrir o arquivo");
    return 1;
  }
  else
  {
    while (fgets(buffer, sizeof(buffer), data_r))
    {

      col=0;
      char *data = strtok(buffer, ",");
      char *comma = ",";
      
      while(data != NULL)
      {
        col++;
        if (col == 1 && strcmp(pUser->user, data) == 0)
        {
          fprintf(temp,"%s",data);
          fprintf(temp,comma);
          aut = 1;
        }


        else if (col == 2 && aut == 1)
        {
          fprintf(temp,"%s",status);
          fprintf(temp,comma);
          aut = 1;
        }
        else if (col == 3 && aut == 1)
        {
          fprintf(temp," ");
          fprintf(temp,comma);
        }
        else if(col == 4 && aut == 1)
        {
          fprintf(temp," ");
          fprintf(temp,comma);
          aut = 0;
          
        }
        else
        {
          fprintf(temp,"%s",data);
          fprintf(temp,comma);
        }
        data = strtok(NULL, ",");
      }
    }
  }
  fclose(data_r);
  fclose(temp);
  remove("leitores.txt");
  rename("temp_tirar_data.txt","leitores.txt");
  return 0;
}


int print_data(tUsuario *pUser)
{
  FILE *data_r;
  
  int aut = 0;
  char buffer[1000];
  int col = 0;

  data_r = fopen("leitores.txt","r");
  
  if(!data_r)
  {
    printf("\nNão foi possível abrir o arquivo");
    return 1;
  }
  else
  {
    while (fgets(buffer, sizeof(buffer), data_r))
    {

      col=0;
      char *data = strtok(buffer, ",");
      char *comma = ",";
      
      while(data != NULL)
      {
        col++;
        if (col == 1 && strcmp(pUser->user, data) == 0)
        {
          aut = 1;
          printf("\nData: %s\n", data);
        }
          
        else if(col == 4 && aut == 1)
        {
          if(data == NULL || data == " " || data == "")
          {
            printf("\n -> Não existe data de devolução\n");
          }

          else
          {
            printf("\nData de Devolução: %s\n", data);
          }
          aut = 0;
          
        }
        data = strtok(NULL, ",");
      }
    }
  }
  fclose(data_r);
  return 0;
}