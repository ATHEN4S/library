#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct {
char* user;
char* password;
} tUsuario;

void checagem_usuario(tUsuario login,tUsuario *login2, int *autorizar);
void colocar_usuario(tUsuario usuario,tUsuario *tusuario);

int main()
{
    tUsuario Usuario;
    char quest_cadastro;

    do
    {
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
        printf("\nDigite o seu Usuário: ");
        scanf("%s", &Usuario.user);
        
        checagem_usuario(Usuario,&Usuario, &autorizar);
        }while(autorizar == 0);

        printf("\nDigite a sua senha: ");
        scanf("%s", &Usuario.password);

        colocar_usuario(Usuario,&Usuario);
    }

    else if (quest_cadastro == 's')
    {
        printf("sim");
    }

    printf("\nSaindo do Programa...");

    return 0;
}

void checagem_usuario(tUsuario login,tUsuario *login2, int *autorizar)
{
    FILE* check_user;

    check_user = fopen("login_usuario.txt","r");

    if (!check_user)
        printf("Can't open file\n");
 
    else
    {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[1024];
 
        int row = 0;
        int column = 0;
 
        while (fgets(buffer,1024, check_user))
        {
            column = 0;
            row++;
 
            // To avoid printing of column
            // names in file can be changed
            // according to need
            printf("row: %d\n", row);
            if (row == 1)
                continue;

 
            // Splitting the data
            char* value = strtok(buffer, ", "); // Vai ser só o "user" do csv

            printf("login: %s\n", &login.user);
            printf("value: %s\n", value);
            if (login.user == value) // -------------> não ta identificando
            {
                printf("gfd\n");
                printf("\nO usuário já existe, tente novamente\n");
                *autorizar = 0;
                break;
            }
            else
            {
                printf("zzzzzzzzzzzzzzzzzzzzzz");
                *autorizar = 1;
            }
        }
    }
        // Close the file
        fclose(check_user);
}


void colocar_usuario(tUsuario usuario,tUsuario *tusuario)
{
    FILE* user_in_txt;
    user_in_txt = fopen("login_usuario.txt","a");
    if (!user_in_txt)
        printf("Can't open file\n");

    else{
        char buffer[1024];
        int column = 0;

        char* value = strtok(buffer, ", ");
        while (value != NULL) {
            if (column == 0) {
                value = usuario.user;
            }
            // Column 2
            if (column == 1) {
                value = usuario.password;
            }
            value = strtok(NULL, ", ");
            column++;
        }
    }
    fclose(user_in_txt);
}