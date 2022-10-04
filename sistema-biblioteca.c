#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define TAM 30

struct ficha{
    int codigo;
    char nome[TAM];
    float nota;
    char genero[TAM];
    char status;
    int ativo;
};
struct ficha dados;
FILE *arq;
char resp;

void AbrirArquivo(char livro[]){
    system("cls");
    arq = fopen(livro, "rb+"); // rb = somente para leitura
    if (arq == NULL) puts("NULL");
    if (arq == NULL)
        arq = fopen(livro, "wb+"); // wb = escrita a partir do início do arquivo
    puts("ARQUIVO ABERTO.");
}

void FecharArquivo(){
    system("cls");
    fclose(arq);
    puts("ARQUIVO FECHADO.");
}

void RegistrarLivro(){
    char opcao;
    do{
        system("cls");
        fseek(arq,0,SEEK_END);
        puts("** INCLUSAO DE LIVRO ***\n");
        printf("Codigo ......:");
        fflush(stdin);
        scanf("%d",&dados.codigo);
        printf("Nome ..:");
        fflush(stdin);
        scanf("%s",dados.nome);
        printf("Nota..:");
        fflush(stdin);
        scanf("%f",&dados.nota);
        printf("Genero..:");
        fflush(stdin);
        scanf("%s",dados.genero);
        printf("Status... (D - Disponivel ou E - Emprestado)");
        fflush(stdin);
        scanf("%s",&dados.status);
        if((dados.nome == "") || (dados.nome == NULL)){
            puts("REGISTRO RESTRINGIDO DEVIDO A DADOS ERRADOS INSERIDOS");
            puts("Deixou o campo NOME em branco");
            puts("Os dados nao foram registrados no sistema");
        }
        else if(dados.codigo == NULL){
            puts("REGISTRO RESTRINGIDO DEVIDO A DADOS ERRADOS INSERIDOS");
            puts("Deixou o campo CÓDIGO em branco");
            puts("Os dados nao foram registrados no sistema");
        }
        else if((dados.status == 'D') || (dados.status == 'E')){
            dados.ativo = 1;
            fwrite(&dados,sizeof(struct ficha),1,arq);
        }
        else{
            puts("REGISTRO RESTRINGIDO DEVIDO A DADOS ERRADOS INSERIDOS");
            puts("Somente digite D = disponivel ou E = Emprestado para o Status");
            puts("Os dados nao foram registrados no sistema");
        }
        if (ferror(arq))
            puts("ERRO!!");
        printf("\nDESEJA INCLUIR OUTRO LIVRO <S/N>?");
        fflush(stdin);
        scanf("%s",&opcao);
    }while(opcao=='S');
}

int Buscar(int codigo){
    int nreg = -1;
    rewind(arq);
    fread(&dados, sizeof(struct ficha),1,arq);
    if (!ferror(arq))
        while (!feof(arq)){
            nreg++;
            if ((dados.codigo) == codigo)
                return(nreg);
            fread(&dados,sizeof(struct ficha),1,arq);
        }
    else puts("ERROR!!");
    return(-1);
}

int BuscarPalavra(char genero[TAM]){
    int nreg = -1;
    rewind(arq);
    fread(&dados,sizeof(struct ficha),1,arq);
    if (!ferror(arq))
        while (!feof(arq))
        {
            nreg++;
            if (strcmp(dados.genero, genero) == 0)
                return(nreg);
            fread(&dados,sizeof(struct ficha),1,arq);
        }
    else puts("ERROR!!");
    return(-1);
}
void Emprestar(){
    int codigo;
    int nreg;
    system("cls");
    puts("*** EMPRESTAR LIVRO ***\n");
    printf("Digite o codigo do livro para registrar o emprestimo ......:");
    fflush(stdin);

    scanf("%d",&codigo);
    nreg = Buscar(codigo);
    if(nreg >=0){
        if(dados.status != 'E'){
            //printf("num %d\n",nreg);
            printf("Status ..: %c\n", dados.status);
            fflush(stdin);
            dados.status = 'E';
            printf("Novo Status..= %c: \n", dados.status);
            puts("Status E = Emprestado\n");
            fseek(arq,sizeof(struct ficha) * nreg, SEEK_SET);
            fwrite(&dados,sizeof(dados),1, arq);
            if (!ferror(arq)){
                puts("");
                puts("EMPRESTIMO REALIZADO");
            }
            else{
                puts("ERROR!!");
            }
        }
        else{
            puts("");
            puts("Livro ja se encontra em status E = Emprestado");
        }
        system("pause");
    }
    else
        puts("LIVRO NAO ESTA CADASTRADO OU NAO ESTA DISPONIVEL");
    fflush(stdin);
    getchar();
}
    void DevolucaoLivro(){
        int codigo;
        int nreg;
        system("cls");
        puts("*** DEVOLUCAO DO LIVRO ***\n");
        printf("Digite o codigo do livro para registar a devolucao......:");
        fflush(stdin);
        scanf("%d",&codigo);
        nreg = Buscar(codigo);
        if (nreg >=0 ){
            if(dados.status != 'D'){
                //printf("num %d\n", nreg);
                printf("Status ... %c\n",dados.status);
                fflush(stdin);
                dados.status = 'D';
                printf("Novo Status..: %C -> Disponivel\n", dados.status);
                puts("");
                fseek(arq,sizeof(struct ficha) * nreg, SEEK_SET);
                fwrite(&dados, sizeof(dados),1, arq);
                if (!ferror(arq)){
                    puts("DEVOLUCAO REALIZADA");
                }
                else{
                    puts("ERROR!!");
                }
            }
            else{
                puts("");
                puts("Livro ja se encontra em status D = Disponivel ou Devolvido");
            }
            system("pause");
        }
        else{
            puts("LIVRO NAO ESTA CADASTRADO OU NAO ESTA DISPONIVEL");
            fflush(stdin);
            getchar();
        }
    }
    void ExcluirLivro(){
        char opcao;
        int codigo;
        int nreg;
        system("cls");
        puts("*** EXCLUSAO DE LIVRO ****");
        printf("Codigo do livro......:");
        fflush(stdin);
        scanf("%d",&codigo);
        nreg = Buscar(codigo);
        if (nreg >=0){
            printf("num %d\n", nreg);
            printf("Nome do livro..: %s\n", dados.nome);
            printf("CONFIRMA EXCLUSAO DO LIVRO <S/N>?");
            fflush(stdin);
            scanf("%s",&opcao);
            if (opcao == 'S'){
                fseek(arq, sizeof(struct ficha) * nreg, SEEK_SET);
                // Exclusão Lógica
                dados.ativo = 0; // quando ativo = 0, ele é excluído logicamente
                fwrite(&dados,sizeof(dados),1, arq);
                if (!ferror(arq))
                    puts("LIVRO EXCLUIDO.");
                else
                    puts("ERRO!!");
            }
        }
        else puts("LIVRO NAO ENCONTRADO.");
        fflush(stdin);
        getchar();
    }

    void ConsultarLivro(){
        int codigo;
        int nreg;
        system("cls");
        puts("*** CONSULTA DE LIVROS ***\n");
        printf("Codigo do Livro .......");
        fflush(stdin);
        scanf("%d",&codigo);
        nreg = Buscar(codigo);
        if (nreg >= 0){
            if(dados.status == 'D'){
                printf("num %d\n",nreg);
                printf("Codigo: %d\n Livro ..: %s\n Genero: %s\n Nota: %.2f  \n Status: %c -> Disponivel\n", dados.codigo, dados.nome, dados.genero, dados.nota, dados.status);
            }

            else{
                printf("num %d\n",nreg);
                printf("Codigo: %d\n Livro ..: %s\n Genero: %s\n Nota: %.2f  \n Status: %c -> Emprestado\n", dados.codigo, dados.nome, dados.genero, dados.nota, dados.status);
            }
        }
        else
            puts("LIVRO NAO ENCONTRADO.");
        fflush(stdin);
        getchar();
    }
    void listarGeneros(){
        rewind(arq);
        fread(&dados,sizeof(struct ficha),1,arq);
        puts("Generos registradas para fazer a busca");
        puts("");
        if (!ferror(arq))
            while (!feof(arq)){
                if (dados.ativo)
                    printf("%s \n",dados.genero);
                fread(&dados, sizeof(struct ficha),1,arq);
            }
        else{
            puts("ERROR!!");
            fflush(stdin);
            getchar();
        }
        puts("");
    }
    void ListarLivro(){
        system("cls");
        puts("***** LISTA DE LIVROS*****\n");
        rewind(arq);
        fread(&dados,sizeof(struct ficha),1,arq);
        if (!ferror(arq))
            while (!feof(arq)){
                if (dados.ativo)
                    printf("Codigo: %d \n Nome: %s \n Nota: %.2f \n Genero: %s\n Status: %c\n Ativo: %d \n\n", dados.codigo,dados.nome,dados.nota, dados.genero, dados.status, dados.ativo);
                fread(&dados,sizeof(struct ficha),1,arq);
            }
        else puts("ERROR!!");
        fflush(stdin);
        getchar();
    }
    void ListarLivroGenero(){
        char genero[TAM];
        system("cls");
        puts("***** LISTA DE LIVROS POR GENERO *****\n");
        listarGeneros();
        puts("Digite o genero de livros que deseja consultar: ");
        scanf("%s",&genero);
        rewind(arq);
        fread(&dados,sizeof(struct ficha),1,arq);
        if (!ferror(arq))
            while (!feof(arq)){
                if (strcmp(dados.genero, genero) == 0)
                    printf("Codigo: %d \n Nome: %s: \n Nota: %.2f \n Genero: %s\n Status: %c\n Ativo: %d \n\n", dados.codigo, dados.nome,dados.nota, dados.genero, dados.status, dados.ativo);
                fread(&dados,sizeof(struct ficha),1,arq);
            }
        else puts("ERROR!!");
        fflush(stdin);
        getchar();
    }

    void Menu(){
        system("cls");
        puts("***** CONTROLE DE BIBLIOTECA *****\n");
        puts("1. Inclusao Livros");
        puts("2. Consulta Livro por codigo");
        puts("3. Emprestar um Livro.");
        puts("4. Devolver um Livro.");
        puts("5. Excluir Livro");
        puts("6. Listar Livros");
        puts("7. Listar Livros por genero");
        puts("0. Sair\n");
    }

    void opcoes(){
        int opcao;
        do{
            Menu();
            printf("Selecione uma opcao: ");
            scanf("%d", &opcao);
            switch(opcao){
            case 1:
                RegistrarLivro();
                break;
            case 2:
                ConsultarLivro();
                break;
            case 3:
                Emprestar();
                break;
            case 4:
                DevolucaoLivro();
                break;
            case 5:
                ExcluirLivro();
                break;
            case 6:
                ListarLivro();
                break;
            case 7:
                ListarLivroGenero();
                break;
            case 0:
                puts("Programa finalizado");
                break;
            default:
                puts("Opcao Invalida");
            }
        }
        while (opcao !=0);
    }

    // funcao principal
    int main(){
        system("cls");
        AbrirArquivo("livros.txt");
        opcoes();
        FecharArquivo();
        system("pause");
    }
