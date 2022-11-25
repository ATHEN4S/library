#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct pessoa{
char* user;
char* password;
struct pessoa * next;
} tUsuario;



int checagem_usuario(const tUsuario login, const tUsuario *pLogin, int *autorizar);
void colocar_usuario(tUsuario usuario,tUsuario *tusuario);
int login(const tUsuario user, const tUsuario password, const tUsuario *pUser,  const tUsuario *pPassw);

int main()
{
    tUsuario Usuario, Senha;
    
    char quest_cadastro;

    do
    {
        printf("\n       Bem vindo a Biblioteca Athenas !\n\n");
        printf("Digite se você já tem cadastro(s/n)\n");
        printf("     (Digite 1 para sair)\n");
        printf("------> ");
        scanf(" %c", &quest_cadastro);
        quest_cadastro = tolower(quest_cadastro);
    }while(quest_cadastro != 's' && quest_cadastro != 'n' && quest_cadastro != '1');

    if (quest_cadastro == 'n')
    {
        int autorizar = 0;

        do{
            Usuario.user = calloc(30, sizeof(char*));
        printf("\nDigite o seu Usuário: ");
        scanf(" %s", Usuario.user);
        
        checagem_usuario(Usuario,&Usuario, &autorizar);
        }while(autorizar == 0);

        Usuario.password = calloc(30, sizeof(char*));
        do{
        printf("\nDigite a sua senha: ");
        scanf(" %s", Usuario.password);
        }while(strlen(Usuario.password) > 20);
        colocar_usuario(Usuario,&Usuario);

        free(Usuario.password);
        free(Usuario.user);
    }

    else if (quest_cadastro == 's')
    {
        int result = 0;
          
        Usuario.user = calloc(30, sizeof(char*));
        printf("\n Usuário: ");
        scanf(" %s", Usuario.user);

        Usuario.password = calloc(20, sizeof(char*));
        printf("\n Senha: ");
        scanf(" %s", Usuario.password);

        result = login(Usuario, Senha, &Usuario, &Senha);
      
    }

    printf("\nSaindo do Programa...");

    return 0;
}

int checagem_usuario(const tUsuario login, const tUsuario *pLogin, int *autorizar)
{
    FILE* check_user;

    check_user = fopen("login_usuario.txt","r");

    if (!check_user)
    {
        printf("Can't open file\n");
        return 1;
    }
    
    else
    {
        char buffer[100];
        int row = 0;

        // Splitting the data
        while (fgets(buffer,100, check_user))
        {
            row++;

            char* value = strtok(buffer, ", "); // Vai ser só o "user" do csv
            printf("\nrow: %d\n", row);

            if (row == 1)
                continue;

            printf("login: %s\n", pLogin->user);
            printf("value: %s\n", value);

            if (strcmp(pLogin->user, value) == 0) 
            {
                printf("\nO usuário já existe, tente novamente\n");
                *autorizar = 0;
                break;
            }
            else
                *autorizar = 1;
            
        }
    }
        // Close the file
        fclose(check_user);
        return 0;
}


void colocar_usuario(tUsuario usuario, tUsuario *tusuario)
{
    FILE* user_in_txt_a;
    FILE* user_in_txt_r;
    user_in_txt_a = fopen("login_usuario.txt","a");
    user_in_txt_r = fopen("login_usuario.txt","r");

    if (!user_in_txt_r)
        printf("Can't open file\n");

    else
    {
        char buffer[50];
        int column = 0;
        char* comma = ", ";
        char destination[50] = "";

        strcat(destination, tusuario->user);
        strcat(destination,comma);
        strcat(destination, tusuario->password);
        printf("\ndest: %s", destination);
        
        fprintf(user_in_txt_a, "\n%s", destination, buffer);
    }
    fclose(user_in_txt_a);
    fclose(user_in_txt_r);
}


int login(const tUsuario user, const tUsuario password, const tUsuario *pUser,  const tUsuario *pPassw)
{
  FILE* login_txt_r;
  login_txt_r = fopen("login_usuario.txt","r");
  int find = 0;

  if (!login_txt_r){
        printf("Can't open file\n");}
  else{
    char buffer[100];
    char buffer2[100];
    int line_u = 0, line_s = 0;
    while (fgets(buffer, sizeof(buffer), login_txt_r))
        { 
          line_u++;
          char* user1 = strtok(buffer, ", ");
          printf("\n line: %d\n", line_u);

          printf("login: %s\n", pUser->user);
          printf("usuario: %s\n", user1);

        if (strcmp(pUser->user, user1) == 0){
          find = find+1;
        }
        }
    if (find == 1){
    printf("%d", find);
    while(fgets(buffer2, line_s, login_txt_r)){
            line_s++;
             printf("%d", find);
            if (line_s == line_u);
            {
            char* senha1 = strtok(NULL, ", ");
            if (strcmp(pPassw->password, senha1) == 0){
              printf("%d", line_s);
              return 1;
              }
            else{
              return 3;
            }
            }
      }
    }
  }
  }