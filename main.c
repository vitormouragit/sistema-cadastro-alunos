#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int matricula;
    char nome[100];
    char curso[50];
    int periodo;
    char situacao[10]; 
    Data dataCadastro;
    Data dataNascimento;
} Aluno;

typedef struct {
    char login[30];
    char senha[21];
} Usuario;


int loginValido(char login[], char senha[]) {
    FILE *f = fopen("usuario.txt", "r");
    Usuario u;

    if (f == NULL)
        return 0;

    while (fscanf(f, "%s %s", u.login, u.senha) != EOF) {
        if (strcmp(u.login, login) == 0 &&
            strcmp(u.senha, senha) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void cadastrarUsuario() {
    Usuario u;
    FILE *f = fopen("usuario.txt", "a");

    if (f == NULL) {
        printf("Erro ao abrir arquivo de usuários.\n");
        return;
    }

    printf("Novo login: ");
    scanf("%s", u.login);

    printf("Nova senha: ");
    scanf("%s", u.senha);

    fprintf(f, "%s %s\n", u.login, u.senha);
    fclose(f);

    printf("Usuário cadastrado com sucesso!\n");
}

int menu() {
    int op;
    printf("\n===== MENU PRINCIPAL =====\n");
    printf("1 - Inserir aluno\n");
    printf("2 - Listar todos os alunos\n");
    printf("3 - Listar alunos ativos\n");
    printf("4 - Listar alunos inativos\n");
    printf("5 - Listar ordenado\n");
    printf("6 - Alterar aluno\n");
    printf("7 - Remover aluno\n");
    printf("0 - Sair\n");
    printf("Opção: ");
    scanf("%d", &op);
    return op;
}

Data dataAtual() {
    Data d;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    d.dia = tm.tm_mday;
    d.mes = tm.tm_mon + 1;      
    d.ano = tm.tm_year + 1900; 

    return d;
}


int matriculaExiste(int mat) {
    FILE *f = fopen("aluno.txt", "r");
    Aluno a;

    if (f == NULL)
        return 0;

    while (fscanf(f,
        "%d;%[^;];%[^;];%d;%[^;];%d/%d/%d;%d/%d/%d\n",
        &a.matricula, a.nome, a.curso, &a.periodo, a.situacao,
        &a.dataCadastro.dia, &a.dataCadastro.mes, &a.dataCadastro.ano,
        &a.dataNascimento.dia, &a.dataNascimento.mes, &a.dataNascimento.ano
    ) != EOF) {
        if (a.matricula == mat) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void inserirAluno() {
    Aluno a;
    FILE *f = fopen("aluno.txt", "a");

    if (f == NULL) {
        printf("Erro ao abrir arquivo de alunos.\n");
        return;
    }

    printf("Matrícula: ");
    scanf("%d", &a.matricula);

    if (matriculaExiste(a.matricula)) {
        printf("Erro: matrícula já existente.\n");
        fclose(f);
        return;
    }

    getchar();
    printf("Nome completo: ");
    fgets(a.nome, 100, stdin);
    a.nome[strcspn(a.nome, "\n")] = '\0';

    printf("Curso: ");
    fgets(a.curso, 50, stdin);
    a.curso[strcspn(a.curso, "\n")] = '\0';

    printf("Período: ");
    scanf("%d", &a.periodo);

	printf("Situação (Ativo ou Inativo): ");
	scanf("%s", &a.situacao);

    printf("Data de nascimento (dd mm aaaa): ");
    scanf("%d %d %d",
          &a.dataNascimento.dia,
          &a.dataNascimento.mes,
          &a.dataNascimento.ano);

	a.dataCadastro = dataAtual();

    fprintf(f, "%d;%s;%s;%d;%s;%d/%d/%d;%d/%d/%d\n",
        a.matricula, a.nome, a.curso, a.periodo, a.situacao,
        a.dataCadastro.dia, a.dataCadastro.mes, a.dataCadastro.ano,
        a.dataNascimento.dia, a.dataNascimento.mes, a.dataNascimento.ano
    );

    fclose(f);
    printf("Aluno inserido com sucesso!\n");
}

void listarAlunos(int filtro) {
    FILE *f = fopen("aluno.txt", "r");
    Aluno a;
    int encontrou = 0;

    if (f == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    while (fscanf(f,
        "%d;%[^;];%[^;];%d;%[^;];%d/%d/%d;%d/%d/%d\n",
        &a.matricula, a.nome, a.curso, &a.periodo, a.situacao,
        &a.dataCadastro.dia, &a.dataCadastro.mes, &a.dataCadastro.ano,
        &a.dataNascimento.dia, &a.dataNascimento.mes, &a.dataNascimento.ano
    ) != EOF) {

        if (filtro == 0 ||
           (filtro == 1 && strcmp(a.situacao, "Ativo") == 0) ||
           (filtro == 2 && strcmp(a.situacao, "Inativo") == 0)) {

            printf("\nMatrícula: %d", a.matricula);
            printf("\nNome: %s", a.nome);
            printf("\nCurso: %s", a.curso);
            printf("\nPeríodo: %d", a.periodo);
            printf("\nSituação: %s", a.situacao);
            printf("\n-------------------------\n");
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("Nenhum aluno encontrado.\n");

    fclose(f);
}

void alterarAluno() {
    int mat, achou = 0, opcao;
    FILE *f = fopen("aluno.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Aluno a;

    if (f == NULL) {
        printf("Arquivo de alunos não encontrado.\n");
        return;
    }

    printf("Informe a matrícula do aluno: ");
    scanf("%d", &mat);

    while (fscanf(f,
        "%d;%[^;];%[^;];%d;%[^;];%d/%d/%d;%d/%d/%d\n",
        &a.matricula, a.nome, a.curso, &a.periodo, a.situacao,
        &a.dataCadastro.dia, &a.dataCadastro.mes, &a.dataCadastro.ano,
        &a.dataNascimento.dia, &a.dataNascimento.mes, &a.dataNascimento.ano
    ) != EOF) {

        if (a.matricula == mat) {
            achou = 1;

            printf("\nAluno encontrado:\n");
            printf("Nome: %s\n", a.nome);
            printf("Curso: %s\n", a.curso);
            printf("Período: %d\n", a.periodo);
            printf("Situação: %s\n", a.situacao);

            printf("\nO que deseja alterar?\n");
            printf("1 - Nome\n");
            printf("2 - Curso\n");
            printf("3 - Período\n");
            printf("4 - Situação\n");
            printf("Opção: ");
            scanf("%d", &opcao);

            getchar();
            switch (opcao) {
                case 1:
                    printf("Novo nome: ");
                    fgets(a.nome, 100, stdin);
                    a.nome[strcspn(a.nome, "\n")] = '\0';
                    break;

                case 2:
                    printf("Novo curso: ");
                    fgets(a.curso, 50, stdin);
                    a.curso[strcspn(a.curso, "\n")] = '\0';
                    break;

                case 3:
                    printf("Novo período: ");
                    scanf("%d", &a.periodo);
                    break;

                case 4:
                    printf("Situação (Ativo/Inativo): ");
                    scanf("%s", a.situacao);
                    break;

                default:
                    printf("Opção inválida.\n");
            }
        }

        fprintf(temp, "%d;%s;%s;%d;%s;%d/%d/%d;%d/%d/%d\n",
            a.matricula, a.nome, a.curso, a.periodo, a.situacao,
            a.dataCadastro.dia, a.dataCadastro.mes, a.dataCadastro.ano,
            a.dataNascimento.dia, a.dataNascimento.mes, a.dataNascimento.ano
        );
    }

    fclose(f);
    fclose(temp);

    remove("aluno.txt");
    rename("temp.txt", "aluno.txt");

    if (!achou)
        printf("Erro: matrícula não encontrada.\n");
    else
        printf("Aluno alterado com sucesso.\n");
}

void listarOrdenado() {
    FILE *f = fopen("aluno.txt", "r");
    Aluno alunos[1000];
    int total = 0, criterio, i, j;
    Aluno aux;

    if (f == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    while (fscanf(f,
        "%d;%[^;];%[^;];%d;%[^;];%d/%d/%d;%d/%d/%d\n",
        &alunos[total].matricula,
        alunos[total].nome,
        alunos[total].curso,
        &alunos[total].periodo,
        alunos[total].situacao,
        &alunos[total].dataCadastro.dia,
        &alunos[total].dataCadastro.mes,
        &alunos[total].dataCadastro.ano,
        &alunos[total].dataNascimento.dia,
        &alunos[total].dataNascimento.mes,
        &alunos[total].dataNascimento.ano
    ) != EOF) {
        total++;
    }

    fclose(f);

    if (total == 0) {
        printf("Nenhum aluno para ordenar.\n");
        return;
    }

    printf("\nOrdenar por:\n");
    printf("1 - Matrícula\n");
    printf("2 - Nome\n");
    printf("Opção: ");
    scanf("%d", &criterio);

    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - 1 - i; j++) {

            int troca = 0;

            if (criterio == 1) {
                if (alunos[j].matricula > alunos[j + 1].matricula)
                    troca = 1;
            } 
            else if (criterio == 2) {
                if (strcmp(alunos[j].nome, alunos[j + 1].nome) > 0)
                    troca = 1;
            }

            if (troca) {
                aux = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = aux;
            }
        }
    }

    printf("\n===== ALUNOS ORDENADOS =====\n");
    for (i = 0; i < total; i++) {
        printf("\nMatrícula: %d", alunos[i].matricula);
        printf("\nNome: %s", alunos[i].nome);
        printf("\nCurso: %s", alunos[i].curso);
        printf("\nPeríodo: %d", alunos[i].periodo);
        printf("\nSituação: %s", alunos[i].situacao);
        printf("\n---------------------------\n");
    }
}

void removerAluno() {
    int mat, achou = 0, op;
    FILE *f = fopen("aluno.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Aluno a;

    if (f == NULL) {
        printf("Arquivo não encontrado.\n");
        return;
    }

    printf("Matrícula a remover: ");
    scanf("%d", &mat);

    while (fscanf(f,
        "%d;%[^;];%[^;];%d;%[^;];%d/%d/%d;%d/%d/%d\n",
        &a.matricula, a.nome, a.curso, &a.periodo, a.situacao,
        &a.dataCadastro.dia, &a.dataCadastro.mes, &a.dataCadastro.ano,
        &a.dataNascimento.dia, &a.dataNascimento.mes, &a.dataNascimento.ano
    ) != EOF) {

        if (a.matricula == mat) {
            achou = 1;
            printf("\nAluno encontrado: %s\n", a.nome);
            printf("Confirmar remoção? (1-Sim / 0-Não): ");
            scanf("%d", &op);

            if (op == 0)
                fprintf(temp, "%d;%s;%s;%d;%s;%d/%d/%d;%d/%d/%d\n",
                    a.matricula, a.nome, a.curso, a.periodo, a.situacao,
                    a.dataCadastro.dia, a.dataCadastro.mes, a.dataCadastro.ano,
                    a.dataNascimento.dia, a.dataNascimento.mes, a.dataNascimento.ano
                );
        } else {
            fprintf(temp, "%d;%s;%s;%d;%s;%d/%d/%d;%d/%d/%d\n",
                a.matricula, a.nome, a.curso, a.periodo, a.situacao,
                a.dataCadastro.dia, a.dataCadastro.mes, a.dataCadastro.ano,
                a.dataNascimento.dia, a.dataNascimento.mes, a.dataNascimento.ano
            );
        }
    }

    fclose(f);
    fclose(temp);

    remove("aluno.txt");
    rename("temp.txt", "aluno.txt");

    if (!achou)
        printf("Aluno não encontrado.\n");
    else
        printf("Operação finalizada.\n");
}

int main() {
    char login[30], senha[21];
    int opcao;
	setlocale(LC_ALL, "Portuguese");
	
    printf("Login: ");
    scanf("%s", login);
    printf("Senha: ");
    scanf("%s", senha);

    if (!loginValido(login, senha)) {
        int op;
        printf("Login inválido. Deseja cadastrar? (1-Sim / 0-Não): ");
        scanf("%d", &op);

        if (op == 1)
            cadastrarUsuario();
        else
            return 0;
    }

    do {
        opcao = menu();
        switch (opcao) {
            case 1: inserirAluno(); break;
            case 2: listarAlunos(0); break;
            case 3: listarAlunos(1); break;
            case 4: listarAlunos(2); break;
            case 5: listarOrdenado();break;
            case 6: alterarAluno(); break;
            case 7: removerAluno(); break;
        }
    } while (opcao != 0);

    printf("Sistema encerrado.\n");
    return 0;
}
