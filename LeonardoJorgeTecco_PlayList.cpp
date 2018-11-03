#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct Artista {
	int codigo;
	char nome[50];
};

typedef struct Musica {
	int codArtista;
	char titulo[50];
	char estilo[50];    
	char exc;
};

int contadorCodigoArtista = 1;

int verificarArtistaExistePorNome(char *nomeArtista) {
	FILE *fp = fopen("artistas.dat", "ab+");
	Artista artista;
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		if (strcmp(nomeArtista, artista.nome) == 0) {
			fclose(fp);
			return 1;
		}
	}
	
	fclose(fp);
	return 0;
}

void listarArtistas() {
	FILE *fp = fopen("artistas.dat", "rb");
	Artista artista;
	
	system("cls");
	
	puts("# LISTAGEM DE ARTISTAS\n\n");
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		printf("CODIGO: %d", artista.codigo);
		printf("\nNOME: %s\n\n", artista.nome);
	}
	
	fclose(fp);
	system("pause");
}

void cadastrarArtista() {
	FILE *fp = fopen("artistas.dat", "ab+");
	Artista artista;
	int artistaCadastrado = 0;
	
	system("cls");
	
	puts("# CADASTRAR ARTISTAS\n\n");
	
	artista.codigo = contadorCodigoArtista++;
	
	do {
		printf("NOME: ");
		fflush(stdin);
		gets(artista.nome);
		strupr(artista.nome);
	
		artistaCadastrado = verificarArtistaExistePorNome(artista.nome);
		
		if (artistaCadastrado == 1) {
			puts("\nARTISTA JA CADASTRADO! CADASTRE NOVAMENTE!\n");
		}
	} while(artistaCadastrado == 1);
	
	puts("\nARTISTA CADASTRADO!\n");
	
	fwrite(&artista, sizeof(Artista), 1, fp);
	fclose(fp);
	
	system("pause");
}

void consultarArtista() {
	FILE *fp = fopen("artistas.dat", "rb");
	Artista artista;
	char nomeArtista[50];
	
	system("cls");
	
	puts("# CONSULTAR ARTISTA\n\n");
	
	printf("NOME: ");
	fflush(stdin);
	gets(nomeArtista);
	strupr(nomeArtista);
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		if (strcmp(nomeArtista, artista.nome) == 0) {
			printf("\nCODIGO: %d", artista.codigo);
			printf("\nNOME: %s\n\n", artista.nome);
			
			system("pause");
			fclose(fp);
			return;
		}
	}
	
	system("pause");
	puts("ARTISTA NAO ENCONTRADO!");
	fclose(fp);
	return;
}

void alterarArtista() {
	FILE *fp = fopen("artistas.dat", "rb+");
	Artista artista;
	int codigoArtista, artistaCadastrado, achouArtista;
	char nomeArtista[50];
	
	system("cls");
	
	puts("# ALTERAR ARTISTA\n\n");
	
	printf("CODIGO: ");
	fflush(stdin);
	scanf("%d", &codigoArtista);
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		if (codigoArtista == artista.codigo) {
			achouArtista = 1;
			break;
		}
	}
	
	if (achouArtista == 0) {
		puts("\nARTISTA NAO ENCONTRADO!\n");
		system("pause");
		fclose(fp);
		return;
	}
	
	printf("\nCODIGO: %d", artista.codigo);
	printf("\nNOME: %s\n\n", artista.nome);
	
	do {
		printf("NOVO NOME: ");
		fflush(stdin);
		gets(nomeArtista);
		strupr(nomeArtista);
		
		artistaCadastrado = verificarArtistaExistePorNome(nomeArtista);
		
		if (artistaCadastrado == 1) {
			puts("\nARTISTA JA CADASTRADO! ALTERE NOVAMENTE!\n");
		}
	} while(artistaCadastrado == 1);
	
	strcpy(artista.nome, nomeArtista);
	
	fseek(fp, -sizeof(Artista), SEEK_CUR);
	fwrite(&artista, sizeof(Artista), 1, fp);
	fclose(fp);

	puts("\nARTISTA ALTERADO!\n");
	system("pause");
}

void menuArtistas() {
	int opcao = 0;
	
	do {
		system("cls");
		
		puts("# ARTISTAS\n\n");
		puts("1 - CADASTRAR ARTISTA");
		puts("2 - LISTAGEM DE ARTISTAS");
		puts("3 - CONSULTA DE ARTISTA");
		puts("4 - ALTERAR ARTISTA");
		puts("5 - SAIR\n\n");
		
		printf("SELECIONE A OPCAO: ");
		fflush(stdin);
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				cadastrarArtista();
				break;
			case 2:
				listarArtistas();
				break;
			case 3:
				consultarArtista();
				break;
			case 4:
				alterarArtista();
				break;
		}
		
	} while (opcao != 5);
}

void menuMusicas() {
	int opcao = 0;
	
	do {
		system("cls");
		
		puts("# MUSICAS\n\n");
		puts("1 - CADASTRAR MUSICA");
		puts("2 - LISTAGEM DE MUSICAS");
		puts("3 - CONSULTA DE MUSICA");
		puts("4 - REMOVER MUSICA");
		puts("5 - SAIR\n\n");
		
		printf("SELECIONE A OPCAO: ");
		fflush(stdin);
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
		}
		
	} while (opcao != 5);
}

void menuPlaylist() {
	int opcao = 0;
	
	do {
		system("cls");
		
		puts("# PLAYLIST\n\n");
		puts("1 - ESCOLHER MUSICA");
		puts("2 - ESCOLHER ARTISTA");
		puts("3 - VER PLAYLIST");
		puts("4 - SAIR\n\n");
		
		printf("SELECIONE A OPCAO: ");
		fflush(stdin);
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		}
		
	} while (opcao != 4);
}

void menuPrincipal() {
	int opcao = 0;
	
	do {
		system("cls");
		
		puts("# PLAYLIST\n\n");
		puts("1 - CADASTRO DE ARTISTAS");
		puts("2 - CADASTRO DE MUSICAS");
		puts("3 - PLAYLIST");
		puts("4 - SAIR\n\n");
		
		printf("SELECIONE A OPCAO: ");
		fflush(stdin);
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				menuArtistas();
				break;
			case 2:
				menuMusicas();
				break;
			case 3:
				menuPlaylist();
				break;
		}
		
	} while (opcao != 4);
}

int main() {
	menuPrincipal();
	
    return 0;
}
