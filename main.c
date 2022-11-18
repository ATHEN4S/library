#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
FILE *CriaArquivo(char s[200])
{
FILE *p;
p = fopen(s, "wb");
return(p);
}
*/

typedef struct {
char* user;
char* password;
} tUsuario, *tPtrUsuario;

int main()
{
    char quest_cadastro;
    tUsuario Usuario;
    tPtrUsuario PtrUsuario;

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
        FILE* cadastro;

        cadastro = fopen("login_usuario.txt","w");

        do{
        printf("\nDigite o seu Usuário: ");
        scanf("%s", PtrUsuario->user);
        printf("\n%s", PtrUsuario->user);
        }while(PtrUsuario->user == "eu"); //Erro aqui

        fclose(cadastro);
    }

    else if (quest_cadastro == 's')
    {
        printf("sim");
    }

    printf("\nSaindo do Programa...");

    return 0;
}