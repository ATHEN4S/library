#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct {
char* user;
char* password;
} tUsuario;

void checagem_usuario(tUsuario algo,tUsuario *algo2, int *autorizar);

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
        FILE* cadastro;

        cadastro = fopen("login_usuario.txt","w");

        do{
        printf("\nDigite o seu Usuário: ");
        scanf("%s", &Usuario.user);
        
        checagem_usuario(Usuario,&Usuario, &autorizar);
        }while(autorizar == 0); //Erro aqui

        fclose(cadastro);
    }

    else if (quest_cadastro == 's')
    {
        printf("sim");
    }

    printf("\nSaindo do Programa...");

    return 0;
}

void checagem_usuario(tUsuario algo,tUsuario *algo2, int *autorizar)
{
    algo.user = "parasdsdfelo";
    algo2->user = algo.user;
    *autorizar = 0;

    if (algo.user == "paralelo")
    {
        *autorizar = 1;
    }
}