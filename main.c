#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pessoa {
  char *user;
  char *password;
} tUsuario;

typedef struct livro {
  char *livros;
  char *disp;
} tLivro;

//Login & Cadastro
int checagem_usuario(const tUsuario login, const tUsuario *pLogin, int *autorizar);
int colocar_usuario(tUsuario usuario, tUsuario *tusuario);
int login(const tUsuario user, const tUsuario *pUser);
//Emprestimo
int aval(const tUsuario user, const tUsuario *pUser);
int checagem_livro(tLivro livro, tLivro *plivro, tUsuario *pUser);
void mudarIND(char *frase, int line);
int adiciona_date(int dia, int mes, int ano, tUsuario *pUser, tLivro *plivro);
//Devolução
int checagemdvvr(const tUsuario user, const tUsuario *pUser);
int mudarDIS(char *nome_livro);
int remove_devol(tUsuario *pUser);
void copiar(FILE *ent, FILE *saida);
//Data de entrega
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
      printf("\n\n  Digite o seu Usuário: ");
      printf("\n (Não utilize ',' ou '/') \n ---> ");
      scanf(" %s", Usuario.user);
      checagem_usuario(Usuario, &Usuario, &autorizar);
      for(int i = 0; Usuario.user[i]; i++)
        {
          if(Usuario.user[i] == ',' || Usuario.user[i] == '/')
            autorizar = 0;
        }
    } while (autorizar == 0 || strlen(Usuario.user) > 20);

    int autorizar_senha = 0;
    Usuario.password = calloc(30, sizeof(char *));
    do {
      autorizar_senha = 0;
      printf("\n  Digite a sua senha ");
      printf("\n (Não utilize ',' ou '/'): \n---> ");
      scanf(" %s", Usuario.password);
      for(int i = 0; Usuario.password[i]; i++)
      {
        if(Usuario.password[i] == ',' || Usuario.password[i] == '/')
          autorizar_senha += 1;
      }
    } while (strlen(Usuario.password) > 20 || autorizar_senha != 0);
    
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

      printf("\n \n             MENU \n");
      printf("\n O que deseja fazer hoje, %s?\n a) Pegar um livro. \n b) Devolver "
             "um livro. \n c) Conferir data de devolução. \n s) Sair \n", Usuario.user);
      printf("\n ------> ");
      scanf(" %c", &pegar_devolver);

      switch (pegar_devolver) {
      
      case 'a':
        
        if (aval(Usuario, &Usuario) == 2){
          
        Book.livros = calloc(1000, sizeof(char*));
        printf("\n\n Digite o título da obra:");
        printf("\n ------> ");
        scanf(" %[^\n]", Book.livros);
        printf("\n Procurando em nossos arquivos:  %s", Book.livros);
          
        checagem_livro(Book, &Book, &Usuario);
        break;
        }
        else
        printf("\n Você já tem um livro! Para pegar outro, devolva-o primeiro. Pode conferir qual livro é e até quando tem para entrega-lo na opção C, 'Conferir data de devolução' do menu.");
        break;

      case 'b':
        printf("\n \n DEVOLUÇÃO \n");
    
        if (checagemdvvr(Usuario, &Usuario) == 2)
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
      printf("\n As informações dadas não batem com uma conta em nosso sistema... Confira seus dados e tente novamente! \n");
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

      if (strcmp(pLogin->user, value) == 0) { // Se o input corresponder ao que o stktok ta rodando...
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
  FILE *status;

  user_in_txt_a = fopen("login_usuario.txt", "a");
  status = fopen("leitores.txt", "a");

  if (!user_in_txt_a || !status) {
    printf("Não foi possível abrir o arquivo\n");
    return 1;
  } else {
    //para user_in_txt_a (login_usuario.txt)
    char buffer[50];
    int column = 0;
    char *comma = ", ";
    char destination[50] = "";

    strcat(destination, tusuario->user);
    strcat(destination, comma);
    strcat(destination, tusuario->password);
    
    fprintf(user_in_txt_a, "\n%s", destination, buffer);

    
    // Já para o status_user (leitores.txt)
    char leitor[50] = "";
    char *status_usuario = "disponivel";

    strcat(leitor, tusuario->user);
    strcat(leitor, comma);
    strcat(leitor, status_usuario);
    strcat(leitor, comma);
    strcat(leitor, comma);
    strcat(leitor, comma);

    fprintf(status, "\n%s", leitor, buffer);
  }

  printf("\nUsuário criado com sucesso !!\n");
  fclose(user_in_txt_a);
  fclose(status);
  return 0;
}

int login(const tUsuario user, const tUsuario *pUser) {
  FILE *login_txt_r;
  login_txt_r = fopen("login_usuario.txt", "r");

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

int aval(const tUsuario user, const tUsuario *pUser)
{
  FILE *leitores;
  
  int aut = 0;
  char buffer[1000];
  int col = 0;
  char pLivro[50];

  leitores = fopen("leitores.txt","r");
  
  if(!leitores)
  {
    printf("\nNão foi possível abrir o arquivo");
    return 1;
  }
  else
  {
    while (fgets(buffer, sizeof(buffer), leitores))
    {

      col=0;
      char *dados = strtok(buffer, ",");

      while(dados != NULL)
      {
        col++;
        
        if (col == 1 && strcmp(pUser->user, dados) == 0)
        {
          aut = 1;
        }
        
        if (col == 2)
        {
          if (aut == 1){
          if (strcmp(dados, " indisponível") == 0)
            return 3;
                   
          else
          {
            return 2;
          }
          aut = 0;
          }
        }
        dados = strtok(NULL, ",");
        }
      }
    }
  
  fclose(leitores);
  return 0;
}

//MUDAR PARA INDISPONIVEL, PEGAR EMPRESTADO
int checagem_livro(tLivro livro, tLivro *plivro, tUsuario *pUser)
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
        for(int i = 0;livroprocurado[i]; i++)
        {
          livroprocurado[i] = tolower(livroprocurado[i]);
        }
        if (strcmp(plivro->livros, livroprocurado) == 0) {
          fflush(stdin);
          printf("\n\n Livro encontrado!\n");
          
          marcador = 1;
        }
        else
          marcador = marcador + 2;
        
      }

      if (col == 4) {
        if (marcador == 1) {
          
          if (strcmp(livroprocurado, "/DISPONIVEL") >= 10)
          {
            printf("\n Informações: %s",frase);
            
            printf("\nDeseja pegar o livro(s/n)?");
            printf("\n----> ");
            char emprestimo;
            scanf(" %c", &emprestimo);
            marcador = 0;
  
            if (emprestimo == 's')
            {
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
            }
          }

          else
          {
            printf("Esse livro já foi emprestado. Escolha outro, ou tente novamente mais tarde.");
            break;
          }
        }   
        break;
      }
      livroprocurado = strtok(NULL, ",");
    }
  }
    
    if (marcador == (row-1)*2){
      printf("\n \n Não achamos esse livro. Verifique o título e tente novamente.");
      
  }
  return 0;
}

int checagemdvvr(const tUsuario user, const tUsuario *pUser) {
  FILE *livro_user;
  livro_user = fopen("leitores.txt", "r");

  char buffer[1000];
  int row = 1, col = 0;
  int marca = 0;
  char nome_liv[100];
  char resp;
  
  while (fgets(buffer, (sizeof(buffer)), livro_user)) {
    col = 0;
    row++;
    char frase[1000];

    strcpy(frase, buffer);
    
    char *procurando = strtok(buffer, ",");
    if (row == 1) {
      procurando == NULL;
      continue;
    }

    while (procurando != NULL)
    {
      col++;
      if (col == 1)
      {
        if (strcmp(pUser->user, procurando) == 0)
        {
          marca = 1;
          fflush(stdin);
        }
        else
          marca = marca + 2;
      }

      if (col == 2 && marca == 1)
      {
        if (strlen(procurando) > 13)
        {
          marca = 1;
        }
        else
        {
          printf("\n Você ainda não pegou um livro! Para pegar, selecione a opção A, 'Pegar um livro.' no menu do usuário.");
          marca = 0;
          return 0;
        }
      }

      if (col == 3 && marca == 1)
      {
        printf("Devolver '%s'(s/n)?", procurando);
        printf("\n---> ");
        scanf(" %c", &resp);
        strcpy(nome_liv, procurando);
        if (resp == 's')
        {
          printf("NOME %s", nome_liv);
          if (mudarDIS(nome_liv) == 2)
            return 2;
        }
        else
          return 0;
         
      }
      
      procurando = strtok(NULL, ",");
    }
  }
   
  return 0;
}//MUDAR PARA DISPONIVEL, DEVOLVER
      
void mudarIND(char *frase, int line) { //MUDAR PARA INDISPONIVEL NO LIVROS, PEGAR EMPRESTADO
  
    FILE *fp;
    FILE *fv;
    int count=0;  //Contar o numero das linhas no txt
    int edited=0;  //como bool, 0=Falso, 1=Verdadeiro
    char *mudar = ",/INDISPONIVEL";  //String que queremos adicionar no final do livro selecionado.
    char *pFrase = frase; // Ponteiro para a frase passada.
    char buffer[1000];
    
    fp=fopen("livros.txt","r");
    fv=fopen("target.txt","w");

    size_t n = strlen(pFrase) + (strlen(mudar)) + 10;
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
            fprintf(fv,"%s\n", s);   
      
            edited=1;
           }
         
       else fprintf(fv,"%s", buffer);
    }
      
    fclose(fp);
    fclose(fv);

    if(edited==1)
        printf("\nLivro emprestado com sucesso ! Boa leitura.");
    else
        printf("\nLinha não encontrada.");

  free(s);
  remove("livros.txt");
  rename("target.txt","livros.txt");
}

int mudarDIS(char *nome_livro) {
  
    FILE *fp;
    FILE *fr;
    FILE *fv;
    int count=0; 
    int edited=0;  
    char *devolver = ",/DISPONIVEL"; 
    char buffer[1000];
    char buffer1[1000];
    char parci[200];
    int row = 0;
    int col = 0;
    int fecha = 0;

  
    fp=fopen("livros.txt","r");
    copiar(fp, fr);
    fr=fopen("livros1.txt","r");
    fv=fopen("target.txt","w");


    size_t n = strlen(nome_livro) + (strlen(devolver)) + 100;
    char *s = malloc(n);
    
    int linha_count = 0;
  
    if(fp==NULL||fv==NULL){
        printf("\nErro... Não foi possível abrir os arquivos");
    }


  while (fgets(buffer1, sizeof(buffer1), fr)) {
    
    col++;
    
    char *livroprocurado = strtok(buffer1, ",");

    while (livroprocurado != NULL) {

      for(int i = 0;livroprocurado[i]; i++)
        {
          livroprocurado[i] = tolower(livroprocurado[i]);
        }
      if (strcmp(nome_livro, livroprocurado) == 0) {
          fecha = 1;
          break;
        } 
      
      livroprocurado = strtok(NULL, ",");
      }
    if (fecha == 1){
      break;
    }
  }
  

    while (fgets(buffer, sizeof(buffer), fp)) {
        count++;
      
        char *linhacmp = buffer;
        char *disp = strtok(buffer, "/");
        disp = strtok(NULL, "/");
       if(count == col){
            
            strcpy(s, linhacmp); 
            s[strlen(s) - 1] = '\0';
            strcat(s, devolver);
            fprintf(fv,"%s \n", s); 
         
            edited=1;
           }
       else fprintf(fv,"%s/%s", linhacmp,disp);
    }
      
    fclose(fp);
    fclose(fr);
    fclose(fv);
    free(s);
    remove("livros.txt");
    remove("livros1.txt");
    rename("target.txt","livros.txt");

    if(edited==1)
    {
        printf("\n\n Livro devolvido com sucesso!");
        return 2;
    }
    else
        printf("\nLinha não encontrada.");

  
}

int adiciona_date(int dia,int mes,int ano, tUsuario *pUser, tLivro *plivro)//MUDAR PARA INDISPONIVEL NO LEITORES TXT, PEGAR EMPRESTADO, BOTAR DATA DE ENTREGA
{
  FILE *data_r;
  FILE *temp;
  
  int aut = 0;
  char buffer[1000];
  int col = 0;
  char data_imprimir[100];

  snprintf(data_imprimir, 100, "%d/%d/%d", dia,mes,ano);

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
          fprintf(temp,"%s",plivro->livros);
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
  //fclose(data_w);
  fclose(data_r);
  fclose(temp);
  remove("leitores.txt");
  rename("temp_colocar_data.txt","leitores.txt");
  return 0;
}

int remove_devol(tUsuario *pUser)//MUDAR PARA DISPONIVEL NO LEITORES TXT, DEVOLVER
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
  char pLivro[50];

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
        }

        if (col == 3 && aut == 1)
        {
          strcpy(pLivro,data);
        }
        
        else if(col == 4 && aut == 1)
        {
          if(strlen(data) < 3)
          {
            printf("\n ------>  Você ainda não pegou um livro emprestado! \n");
          }

          else
          {
            printf("\n  Livro Emprestado: %s\n", pLivro);
            printf("\n  Data de Devolução: %s\n", data);
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

void copiar(FILE *ent, FILE *saida){
  ent = fopen("livros.txt", "r");
  saida = fopen("livros1.txt", "w");
  char buffer[300];
   while (fgets(buffer, sizeof(buffer), ent))
    {
      fprintf(saida,"%s",buffer);
      }
  fclose(ent);
  fclose(saida);
}
