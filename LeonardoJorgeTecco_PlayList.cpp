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
		if (strcmp(musica.titulo, tituloMusica) == 0 && musica.exc != '*') {
			posicaoMusica = ftell(fp) - sizeof(Musica);
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

float verificarPorcentagemDeMusicasExcluidas() {
	FILE *fp;
	Musica musica;
	float quantidadeDeMusicas = 0, quantidadeDeMusicasExcluidas = 0, porcentagem;

	fp = fopen("musicas.dat", "rb");

	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		quantidadeDeMusicas++;

		if (musica.exc == '*') {
			quantidadeDeMusicasExcluidas++;
		}
	}

	fclose(fp);

	porcentagem = (quantidadeDeMusicasExcluidas / quantidadeDeMusicas) * 100;

	return porcentagem;
}

void reorganizarArquivoDeMusicas() {
	FILE *fp, *novo;
	Musica musica;

	novo = fopen("musicas.bak", "wb");
	fp = fopen("musicas.dat", "rb");

	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		if (musica.exc != '*') {
			fwrite(&musica, sizeof(Musica),1, novo);
		}
	}

	fclose(fp);
	fclose(novo);

	remove("musicas.dat");
	rename("musicas.bak", "musicas.dat");
}

/**** PLAYLIST ****/

void escolherMusica() {
	FILE *fp = fopen("musicas.dat", "rb");
	FILE *fpTxt = fopen("playlist.txt", "a");
	Musica musica;
	char tituloMusica[50];
	int achouMusica = 0;

	system("cls");
	
	puts("# ESCOLHER MUSICA\n\n");
	
	printf("TITULO: ");
	fflush(stdin);
	gets(tituloMusica);
	strupr(tituloMusica);
	
	while (fread(&musica, sizeof(Musica), 1, fp) == 1) {
		if (strcmp(tituloMusica, musica.titulo) == 0 && musica.exc != '*') {
			achouMusica = 1;

			Artista artista = retornaArtistaPorCodigo(musica.codArtista);

			fprintf(fpTxt, "TITULO: %s - ESTILO: %s - NOME ARTISTA: %s\n", musica.titulo, musica.estilo, artista.nome);
			puts("\nMUSICAS ADICIONADA COM SUCESSO!\n");
		}
	}

	if (achouMusica == 0) {
		puts("\nMUSICA NAO ENCONTRADA!\n");
	}
	
	system("pause");

	fclose(fp);
	fclose(fpTxt);

	return;
}

void escolherArtista() {
	FILE *fpArtistas = fopen("artistas.dat", "rb");
	FILE *fpMusicas = fopen("musicas.dat", "rb");
	FILE *fpTxt = fopen("playlist.txt", "a");
	Artista artista;
	Musica musica;
	char nomeArtista[50];
	
	system("cls");
	
	puts("# CONSULTAR ARTISTA\n\n");
	
	printf("NOME: ");
	fflush(stdin);
	gets(nomeArtista);
	strupr(nomeArtista);
	
	while (fread(&artista, sizeof(Artista), 1, fpArtistas) == 1) {
		if (strcmp(nomeArtista, artista.nome) == 0) {
			while (fread(&musica, sizeof(Musica), 1, fpMusicas) == 1) {
				if (artista.codigo == musica.codArtista) {
					fprintf(fpTxt, "TITULO: %s - ESTILO: %s - NOME ARTISTA: %s\n", musica.titulo, musica.estilo, artista.nome);
				}
			}

			puts("\nMUSICAS DO ARTISTA ADICIONADAS COM SUCESSO!\n");

			fclose(fpTxt);
			fclose(fpArtistas);
			fclose(fpMusicas);
			system("pause");
			return;
		}
	}

	puts("\nARTISTA NAO ENCONTRADO!\n");
	
	fclose(fpTxt);
	fclose(fpArtistas);
	fclose(fpMusicas);
	system("pause");
	return;
}

void exibirPlaylist() {
	FILE *fpTxt = fopen("playlist.txt", "r");
	char musica[500];
	
	system("cls");
	
	puts("# VER PLAYLIST\n\n");
	
	while (fgets(musica, 500, fpTxt)) {
		puts(musica);
	}
	
	fclose(fpTxt);
	system("pause");
	return;
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
		if (musica.exc != '*') {
			Artista artista = retornaArtistaPorCodigo(musica.codArtista);
			printf("TITULO: %s\n", musica.titulo);
			printf("ESTILO: %s\n", musica.estilo);
			printf("NOME ARTISTA: %s\n\n", artista.nome);
		}
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
		if (strcmp(tituloMusica, musica.titulo) == 0 && musica.exc != '*') {
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

void excluirMusica() {
	FILE *fp = fopen("musicas.dat", "rb+");
	Musica musica;
	char tituloMusica[50];
	int posicaoMusica = -1;
	
	system("cls");
	
	puts("# EXCLUIR MUSICA\n\n");
	
	printf("TITULO: ");
	fflush(stdin);
	gets(tituloMusica);
	strupr(tituloMusica);
	
	posicaoMusica = verificarMusicaExistePorTitulo(tituloMusica);

	if (posicaoMusica == -1) {
		puts("\nMUSICA NAO ENCONTRADA!\n");
		system("pause");
		fclose(fp);
		return;
	}

	fseek(fp, posicaoMusica, SEEK_SET);
	fread(&musica, sizeof(Musica), 1, fp);

	musica.exc = '*';

	fseek(fp, posicaoMusica, SEEK_SET);
	fwrite(&musica, sizeof(Musica), 1, fp);

	printf("\nMUSICA EXCLUIDA COM SUCESSO!\n");

	fclose(fp);
	
	if (verificarPorcentagemDeMusicasExcluidas() > 20) {
		reorganizarArquivoDeMusicas();
	}

	system("pause");
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
				excluirMusica();
				break;
		}
		
	} while (opcao != 5);
}

void menuPlaylist() {
	int opcao = 0;

	FILE *fp = fopen("playlist.txt", "w");
	fclose(fp);

	do {
		system("cls");
		
		puts("# PLAYLIST\n\n");
		puts("NOVA PLAYLIST GERADA!\n\n");
		puts("1 - ESCOLHER MUSICA");
		puts("2 - ESCOLHER ARTISTA");
		puts("3 - VER PLAYLIST");
		puts("4 - SAIR\n\n");
		
		printf("SELECIONE A OPCAO: ");
		fflush(stdin);
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				escolherMusica();
				break;
			case 2:
				escolherArtista();
				break;
			case 3:
				exibirPlaylist();
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
		puts("3 - GERAR PLAYLIST");
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
