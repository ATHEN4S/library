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

int login(tUsuario user, tUsuario *pUser, int *autorizar_login);

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
        }while(autorizar == 0 || strlen(Usuario.user) > 20);

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
        int autorizar_login = 0;

        Usuario.user = calloc(30, sizeof(char*));
        printf("\n Usuário: ");
        scanf(" %s", Usuario.user);

        Usuario.password = calloc(30, sizeof(char*));
        printf("\n Senha: ");
        scanf(" %s", Usuario.password);

        login(Usuario, &Usuario, &autorizar_login);

        free(Usuario.password);
        free(Usuario.user);

        if (autorizar_login == 1)
        {
            printf("Continuando programa...");
        }
        else
        {
            printf("O programa não pôde ser continuado");
        }
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


int colocar_usuario(tUsuario usuario, tUsuario *tusuario)
{
    FILE* user_in_txt_a;
    FILE* user_in_txt_r;
    user_in_txt_a = fopen("login_usuario.txt","a");
    user_in_txt_r = fopen("login_usuario.txt","r");

    if (!user_in_txt_r)
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
        printf("\ndest: %s", destination);
        
        fprintf(user_in_txt_a, "\n%s", destination, buffer);
    }
    fclose(user_in_txt_a);
    fclose(user_in_txt_r);

    return 0;
}


int login(tUsuario user, tUsuario *pUser, int *autorizar_login)
{
    FILE* login_txt_r;
    login_txt_r = fopen("login_usuario.txt","r");

    if (!login_txt_r)
    {
        printf("Can't open file\n");
        return 1;
    }

    else
    {
        char buffer[100];
        char buffer2[100];
        int line_u = 0, col = 0;

        while (fgets(buffer, sizeof(buffer), login_txt_r)) // Vai rodar as linhas
        { 
            col=0;
            int find = 0;
            line_u++;

            char* user1 = strtok(buffer, ", ");
            
            while(user1 != NULL)
            {
                col++;
                
                printf("\nline: %d\n", line_u);
                printf("col: %d\n", col);
                printf("usuario: %s\n", user1);

                if(line_u == 1)
                {
                    user1 = NULL; // Para não rodar a mesma linha novamente
                    continue;
                }

                if(col == 1)
                {
                    printf("login: %s\n", pUser->user);
                    printf("usuario_col = 1: %s\n", user1);

                    if (strcmp(pUser->user, user1) == 0) // Se o usuário existe...
                    {
                        find++;
                        printf("\nfind=%d\n", find);
                    }
                }

                if(col == 2)
                {
                    printf("finda=%d\n", find);
                    printf("password:%s\n", pUser->password);
                    printf("usuario_col=2:%s\n", user1);
                    if (strcmp(pUser->password, user1) == 0) // NÃO TA PEGANDO ESSA PARTE <<-------
                    {
                        if (find == 1) // Se o usuário também existe nesta mesma linha
                        {
                            printf("Senha corresponde ao usuário\n");
                            *autorizar_login = 1;
                            return 0;
                        }
                        else
                        {
                            printf("Senha não corresponde ao usuário\n");
                            *autorizar_login = 0;
                            return 1;
                        }
                    }
                }

                user1 = strtok(NULL, ", "); //Para ter uma mudança de coluna com a ajuda do while != NULL
            }
        }
    }
}