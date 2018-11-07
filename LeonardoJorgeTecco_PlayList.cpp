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

int verificarArtistaExistePorCodigo(int codigoArtista) {
	FILE *fp = fopen("artistas.dat", "rb");
	Artista artista;
	int posicaoArtista = -1;
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		if (codigoArtista == artista.codigo) {
			posicaoArtista = ftell(fp) - sizeof(Artista);
			fclose(fp);
			return posicaoArtista;
		}
	}
	
	fclose(fp);
	return -1;
}

int verificarMusicaExistePorTitulo(char *tituloMusica) {
	FILE *fp = fopen("musicas.dat", "rb");
	Musica musica;
	int posicaoMusica = -1;
	
	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		if (strcmp(musica.titulo, tituloMusica) == 0) {
			posicaoMusica = ftell(fp) - sizeof(Artista);
			fclose(fp);
			return posicaoMusica;
		}
	}
	
	fclose(fp);
	return -1;
}

Artista retornaArtistaPorCodigo(int codigoArtista) {
	FILE *fp = fopen("artistas.dat", "rb");
	Artista artista;
	int posicaoArtista = verificarArtistaExistePorCodigo(codigoArtista);
	
	fseek(fp, posicaoArtista, SEEK_SET);
	fread(&artista, sizeof(Artista), 1, fp);
	fclose(fp);
	
	return artista;
}

/**** MUSICA ****/

void cadastrarMusica() {
	FILE *fp = fopen("musicas.dat", "ab+");
	Musica musica;
	Artista artista;
	int posicaoMusica = -1;
	int posicaoArtista = -1;
	
	system("cls");
	
	puts("# CADASTRAR MUSICAS\n\n");
	
	do {
		printf("TITULO: ");
		fflush(stdin);
		gets(musica.titulo);
		strupr(musica.titulo);

		posicaoMusica = verificarMusicaExistePorTitulo(musica.titulo);

		if (posicaoMusica != -1) {
			puts("\nMUSICA JA CADASTRADA! CADASTRE NOVAMENTE!\n");
		}
	} while (posicaoMusica != -1);
	
	printf("ESTILO: ");
	fflush(stdin);
	gets(musica.estilo);
	strupr(musica.estilo);
	
	printf("CODIGO DO ARTISTA: ");
	fflush(stdin);
	scanf("%d", &musica.codArtista);
	
	posicaoArtista = verificarArtistaExistePorCodigo(musica.codArtista);
	
	if (posicaoArtista == -1) {
		puts("\nARTISTA NAO ENCONTRADO!\n");
		system("pause");
		return;
	}
	
	artista = retornaArtistaPorCodigo(musica.codArtista);
	
	printf("ARTISTA: %s\n", artista.nome);
	
	musica.exc = ' ';

	puts("\nMUSICA CADASTRADA!\n");
	
	fwrite(&musica, sizeof(Musica), 1, fp);
	fclose(fp);
	
	system("pause");
}

void listarMusicas() {
	FILE *fp = fopen("musicas.dat", "rb");
	Musica musica;
	
	system("cls");
	
	puts("# LISTAGEM DE MUSICAS\n\n");
	
	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		Artista artista = retornaArtistaPorCodigo(musica.codArtista);
		printf("TITULO: %s\n", musica.titulo);
		printf("ESTILO: %s\n", musica.estilo);
		printf("NOME ARTISTA: %s\n", artista.nome);
		printf("EXC: %c\n\n", musica.exc);
	}
	
	fclose(fp);
	system("pause");
}

void consultarMusica() {
	FILE *fp = fopen("musicas.dat", "rb");
	Musica musica;
	char tituloMusica[50];
	int achouMusica = 0;
	
	system("cls");
	
	puts("# CONSULTAR MUSICA\n\n");
	
	printf("TITULO: ");
	fflush(stdin);
	gets(tituloMusica);
	strupr(tituloMusica);
	
	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		if (strcmp(tituloMusica, musica.titulo) == 0) {
			achouMusica = 1;

			Artista artista = retornaArtistaPorCodigo(musica.codArtista);
			printf("TITULO: %s\n", musica.titulo);
			printf("ESTILO: %s\n", musica.estilo);
			printf("NOME ARTISTA: %s\n\n", artista.nome);
		}
	}

	if (achouMusica == 0) {
		puts("\nMUSICA NAO ENCONTRADA!\n");
	}
	
	system("pause");
	fclose(fp);
	return;
}

/**** ARTISTA ****/

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
	int posicaoArtista = -1;
	
	system("cls");
	
	puts("# CADASTRAR ARTISTAS\n\n");
	
	do {
		printf("CODIGO: ");
		fflush(stdin);
		scanf("%d", &artista.codigo);

		posicaoArtista = verificarArtistaExistePorCodigo(artista.codigo);
		
		if (posicaoArtista != -1) {
			puts("\nARTISTA JA CADASTRADO! CADASTRE NOVAMENTE!\n");
		}
	} while(posicaoArtista != -1);
	
	printf("NOME: ");
	fflush(stdin);
	gets(artista.nome);
	strupr(artista.nome);

	puts("\nARTISTA CADASTRADO!\n");
	
	fwrite(&artista, sizeof(Artista), 1, fp);
	fclose(fp);
	
	system("pause");
}

void consultarArtista() {
	FILE *fp = fopen("artistas.dat", "rb");
	Artista artista;
	char nomeArtista[50];
	int achouArtistas = 0;
	
	system("cls");
	
	puts("# CONSULTAR ARTISTA\n\n");
	
	printf("NOME: ");
	fflush(stdin);
	gets(nomeArtista);
	strupr(nomeArtista);
	
	while (fread(&artista, sizeof(Artista), 1, fp) == 1) {
		if (strcmp(nomeArtista, artista.nome) == 0) {
			achouArtistas = 1;

			printf("\nCODIGO: %d", artista.codigo);
			printf("\nNOME: %s\n\n", artista.nome);
		}
	}

	if (achouArtistas == 0) {
		puts("\nARTISTA NAO ENCONTRADO!\n");
	}
	
	system("pause");
	fclose(fp);
	return;
}

void alterarArtista() {
	FILE *fp = fopen("artistas.dat", "rb+");
	Artista artista;
	int codigoArtista, posicaoArtista = -1;
	char nomeArtista[50];
	
	system("cls");
	
	puts("# ALTERAR ARTISTA\n\n");
	
	printf("CODIGO: ");
	fflush(stdin);
	scanf("%d", &codigoArtista);
	
	posicaoArtista = verificarArtistaExistePorCodigo(codigoArtista);

	if (posicaoArtista == -1) {
		puts("\nARTISTA NAO ENCONTRADO!\n");
		system("pause");
		fclose(fp);
		return;
	}

	fseek(fp, posicaoArtista, SEEK_SET);
	fread(&artista, sizeof(Artista), 1, fp);

	printf("\nCODIGO: %d", artista.codigo);
	printf("\nNOME: %s\n\n", artista.nome);
	
	printf("NOVO NOME: ");
	fflush(stdin);
	gets(artista.nome);
	strupr(artista.nome);

	fseek(fp, posicaoArtista, SEEK_SET);
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
				cadastrarMusica();
				break;
			case 2:
				listarMusicas();
				break;
			case 3:
				consultarMusica();
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
