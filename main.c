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
int colocar_usuario(tUsuario usuario,tUsuario *tusuario);
int login(const tUsuario user, const tUsuario *pUser);

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

    if(quest_cadastro == '1'){
      printf("\n Você saiu da Biblioteca. Até a próxima!");
      return 0;
    }

    if (quest_cadastro == 'n')
    {
        int autorizar = 0;

        do{
            Usuario.user = calloc(30, sizeof(char*));
        printf("\nDigite o seu Usuário: ");
        scanf(" %s", Usuario.user);
        
        checagem_usuario(Usuario,&Usuario, &autorizar);
        }while(autorizar == 0 || strlen(Usuario.user) > 20);

        Usuario.password = calloc(30, sizeof(char*));
        do{
        printf("\nDigite a sua senha: ");
        scanf(" %s", Usuario.password);
        }while(strlen(Usuario.password) > 20);

        colocar_usuario(Usuario,&Usuario); // Colocando o usuario e senha no arquivo txt

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

        result = login(Usuario, &Usuario);

        if (result == 1)
        {
            printf("\nLogin realizado com sucesso! \n");
        }

        else if (result == -1)
            printf("Falha ao abrir arquivo\n");
            

        else
          printf("\n As informações dadas não batem com uma conta em nosso sistema... Confira seus dados e tente novamente! \n");
    }

    printf("Saindo do Programa...");

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


int colocar_usuario(tUsuario usuario, tUsuario *tusuario)
{
    FILE* user_in_txt_a;
    FILE* user_in_txt_r;
    FILE* status;

    user_in_txt_a = fopen("login_usuario.txt","a");
    user_in_txt_r = fopen("login_usuario.txt","r");
    status = fopen("leitores.txt", "a");

    if (!user_in_txt_r || !status)
    {
        printf("Can't open file\n");
        return 1;
    }
    else
    {
        char buffer[50];
        int column = 0;
        char* comma = ", ";
        char destination[50] = "";

        strcat(destination, tusuario->user);
        strcat(destination,comma);
        strcat(destination, tusuario->password);
        fprintf(user_in_txt_a, "\n%s", destination, buffer);

        // Já para o status_user
        char leitor[50] = "";
        char* status_usuario = "disponivel";
        char* livros = "";
        char* devolver = "";
        char* lista_desejos = "";

        strcat(leitor, tusuario->user);
        strcat(leitor,comma);
        strcat(leitor, status_usuario);
        strcat(leitor,comma);
        strcat(leitor,livros);
        strcat(leitor,comma);
        strcat(leitor,devolver);
        strcat(leitor,comma);
        strcat(leitor,lista_desejos);

        fprintf(status, "\n%s", leitor, buffer);
    }

    printf("\nUsuário criado com sucesso !!\n");
    fclose(user_in_txt_a);
    fclose(user_in_txt_r);
    fclose(status);
    return 0;
}


int login(const tUsuario user, const tUsuario *pUser)
{
    FILE* login_txt_r;
    login_txt_r = fopen("login_usuario.txt","r");
    int find = 0;

    if (!login_txt_r)
    {
        printf("Can't open file\n");
        return -1;
    }
    else
    {
        char buffer[100];
        int line = 0;
        while (fgets(buffer, sizeof(buffer), login_txt_r))
        { 
            line++;
            char* user1 = strtok(buffer, ", ");
            char* senha1 = strtok(NULL, ", ");
            
            // Usando concatenação de string
            char pass[30] = "";
            char* n = "\n";

            strcat(pass, pUser->password);
            strcat(pass,n);

            if ((strcmp(pUser->user, user1) == 0) && ((strcmp(pass, senha1) == 0) || (strcmp(pUser->password, senha1) == 0))) // Se usuario e senha se correspondem...
            {
                fclose(login_txt_r);
                return 1;
            }

            if (line == sizeof(buffer))
            {
                fclose(login_txt_r);
                return 0;
            }
        }
    }
}