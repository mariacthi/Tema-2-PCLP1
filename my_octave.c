// Tudor Maria - Elena, 311CA

#include <stdio.h>
#include <stdlib.h>
#define VAL 10007

void verif_eroare(int *p)
{
	/* verifica daca pointerul este nul */
	if (!p) {
		fprintf(stderr, "alocarea a esuat");
		free(p);
		exit(-1);
	}
}

void verif(int ***p)
{
	/* verifica daca pointerul triplu este nul*/
	if (!p) {
		fprintf(stderr, "alocarea a esuat");
		free(p);
		exit(-1);
	}
}

int **alocare_matrice(int **a, int linii, int coloane)
{
	/* verifica daca alocarea matricei a functionat
	si aloca dinamic in continuare liniile acesteia */
	if (!a) {
		fprintf(stderr, "alocarea matricei a esuat");
		free(a);
		return NULL;
	}
	for (int i = 0; i < linii; i++) {
		a[i] = (int *)malloc(coloane * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "alocarea pe linii a esuat");
			for (int j = 0; j <= i; j++)
				free(a[j]);
			free(a);
			return NULL;
		}
	}
	return a;
}

void realocare(int ****a, int **lin, int **col, int *index)
{
	/* realoca cu cate o unitate cei trei pointeri
	si modifica prin referentiere indexul matricei*/
	int i = *index + 2;
	(*a) = (int ***)realloc((*a), i * sizeof(int **));
	verif(*a);
	(*lin) = (int *)realloc((*lin), i * sizeof(int));
	verif_eroare(*lin);
	(*col) = (int *)realloc((*col), i * sizeof(int));
	verif_eroare(*col);
	(*index)++;
}

void realocare2(int ****a, int index)
{
	index += 2;
	*a = (int ***)realloc((*a), index * sizeof(int **));
	verif(*a);
}

void incarca_matrice(int **a, int linii, int coloane, int *l, int *c)
{
	/* citeste elementele unei matrice si modifica
	prin referentiere valoarea pointerelor de linii
	si coloane corespunzatoare pozitiei */
	*l = linii;
	*c = coloane;
	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++)
			scanf("%d", &a[i][j]);
	}
}

void afiseaza_matrice(int **a, int linii, int coloane)
{
	/* printeaza elementele matricei cerute */
	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

void elibereaza_matrice(int **a, int linii)
{
	/* elibereaza mai intai liniile matricei,
	dupa care si matricea in sine */
	for (int i = 0; i < linii; i++)
		free(a[i]);
	free(a);
}

int **redim_matrice(int **a, int *l, int *c)
{
	/* retine intr-o variabila numarul vechi
	de linii */
	int lin0 = *l;
	/* citeste si retine cu ajutorul a doi
	pointeri liniile si coloanele de care are
	nevoie pentru redimensionare */
	scanf("%d", l);
	int *lin = (int *)malloc(*l * sizeof(int));
	verif_eroare(lin);
	for (int i = 0; i < *l; i++)
		scanf("%d", &lin[i]);
	scanf("%d", c);
	int *col = (int *)malloc(*c * sizeof(int));
	verif_eroare(col);
	for (int i = 0; i < *c; i++)
		scanf("%d", &col[i]);
	/* aloca dinamic un pointer dublu pentru
	a stoca elementele necesare */
	int **mat = (int **)malloc(*l * sizeof(int *));
	mat = alocare_matrice(mat, *l, *c);
	for (int i = 0; i < *l; i++) {
		for (int j = 0; j < *c; j++)
			mat[i][j] = a[lin[i]][col[j]];
	}
	/* elibereaza pointerii si matricea initiala */
	free(lin);
	free(col);
	elibereaza_matrice(a, lin0);
	return mat;
}

int **inm_mat(int **mat1, int **mat2, int l, int c, int m)
{
	/* aloca dinamic un pointer dublu pentru
	a retine valorile matricei-produs si il
	returneaza */
	int **a = (int **)malloc(l * sizeof(int *));
	a = alocare_matrice(a, l, c);
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++) {
			a[i][j] = 0;
			for (int k = 0; k < m; k++) {
				a[i][j] += mat1[i][k] * mat2[k][j];
				a[i][j] %= VAL;
				if (a[i][j] < 0)
					a[i][j] = VAL + a[i][j] % VAL;
			}
		}
	}
	return a;
}

int ***sortare(int ***a, int index, int *l, int *c)
{
	/* aloca dinamic pointeri pentru pozitia fiecarei
	matrice si suma elementelor acestora */
	int *poz = (int *)malloc((index + 1) * sizeof(int));
	verif_eroare(poz);
	int *s = (int *)malloc((index + 1) * sizeof(int));
	verif_eroare(s);
	/* calculeaza suma elementelor fiecarei matrice */
	for (int i = 0; i < index; i++) {
		s[i] = 0;
		for (int j = 0; j < l[i]; j++) {
			for (int k = 0; k < c[i]; k++) {
				s[i] += a[i][j][k];
				s[i] %= VAL;
				if (s[i] < 0)
					s[i] = VAL + s[i] % VAL;
			}
		}
		poz[i] = i;
	}
	/* interschimba in functie de suma elementelor
	vectorilor de pozitie, linii, coloane si suma */
	for (int i = 0; i < index - 1; i++) {
		for (int j = i; j < index; j++) {
			if (s[i] > s[j]) {
				int aux = s[i];
				s[i] = s[j];
				s[j] = aux;
				aux = poz[i];
				poz[i] = poz[j];
				poz[j] = aux;
				aux = l[i];
				l[i] = l[j];
				l[j] = aux;
				aux = c[i];
				c[i] = c[j];
				c[j] = aux;
			}
		}
	}
	/* aloca un pointer triplu in care retine multimea de
	matrice, dar in noua ordine */
	int ***b = (int ***)malloc((index + 1) * sizeof(int **));
	verif(b);
	for (int i = 0; i < index; i++) {
		b[i] = (int **)malloc(l[i] * sizeof(int *));
		b[i] = alocare_matrice(b[i], l[i], c[i]);
		if (!b[i]) {
			fprintf(stderr, "alocarea a esuat");
			for (int j = 0; j < i; j++)
				free(b[j]);
			free(b);
			return NULL;
		}
		for (int j = 0; j < l[i]; j++) {
			for (int k = 0; k < c[i]; k++)
				b[i][j][k] = a[poz[i]][j][k];
		}
		elibereaza_matrice(a[poz[i]], l[i]);
	}
	free(a);
	free(poz);
	free(s);
	return b;
}

int **transpusa(int **a, int *l, int *c)
{
	/* aloca matricea in care retine valorile
	transpusei si modifica valorile initiale
	ale liniilor si coloanelor matricei */
	int **mat = (int **)malloc(*c * sizeof(int *));
	mat = alocare_matrice(mat, *c, *l);
	for (int i = 0; i < *c; i++) {
		for (int j = 0; j < *l; j++)
			mat[i][j] = a[j][i];
	}
	elibereaza_matrice(a, *l);
	int aux = *l;
	*l = *c;
	*c = aux;
	return mat;
}

int ***eliberare(int ***a, int nr, int *l, int *c, int index)
{
	index--;
	/* aloca un pointer triplu in care stocheaza
	toate matricele mai putin cea specificata,
	eliberand pointerul vechi */
	int ***b = (int ***)malloc((index + 1) * sizeof(int **));
	for (int i = 0; i < index; i++) {
		if (i < nr) {
			b[i] = (int **)malloc(l[i] * sizeof(int *));
			b[i] = alocare_matrice(b[i], l[i], c[i]);
			for (int j = 0; j < l[i]; j++) {
				for (int k = 0; k < c[i]; k++)
					b[i][j][k] = a[i][j][k];
		}
		elibereaza_matrice(a[i], l[i]);
		} else if (i >= nr) {
			b[i] = (int **)malloc(l[i + 1] * sizeof(int *));
			b[i] = alocare_matrice(b[i], l[i + 1], c[i + 1]);
			for (int j = 0; j < l[i + 1]; j++) {
				for (int k = 0; k < c[i + 1]; k++)
					b[i][j][k] = a[i + 1][j][k];
			}
			elibereaza_matrice(a[i + 1], l[i + 1]);
		}
	}
	elibereaza_matrice(a[nr], l[nr]);
	free(a);
	/* modifica pointerii de linii si coloane
	cu indexul dupa matricea eliminata */
	for (int i = nr; i < index; i++) {
		l[i] = l[i + 1];
		c[i] = c[i + 1];
	}
	return b;
}

int **ridicare_timp_log(int **a, int l, long p)
{
	/* aloca o matrice care are initial
	rol de element neutru la inmultire*/
	int **aux = (int **)malloc(l * sizeof(int *));
	aux = alocare_matrice(aux, l, l);
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			if (i == j)
				aux[i][j] = 1;
			else
				aux[i][j] = 0;
		}
	}
	if (p == 0)
		return aux;
	unsigned long i;
	int ***copie_a = (int ***)malloc(sizeof(int **));
	verif(copie_a);
	int ***copie_aux = (int ***)malloc(sizeof(int **));
	verif(copie_aux);
	long contor_a = 0, contor_aux = 0;
	/* parcurg bit cu bit puterea la care
	trebuie ridicata matricea */
	for (i = 0; (1 << i) <= p; ++i) {
		if (((1 << i) & p) > 0) {
			realocare2(&copie_aux, contor_aux);
			copie_aux[contor_aux] = inm_mat(a, aux, l, l, l);
			for (int  i = 0; i < l; i++) {
				for (int j = 0; j < l; j++)
					aux[i][j] = copie_aux[contor_aux][i][j];
			}
			contor_aux++;
		}
		realocare2(&copie_a, contor_a);
		copie_a[contor_a] = inm_mat(a, a, l, l, l);
		for (int  i = 0; i < l; i++) {
			for (int j = 0; j < l; j++)
				a[i][j] = copie_a[contor_a][i][j];
		}
		contor_a++;
	}
	/* eliberez copiile create si matricea initiala */
	for (long i = 0; i < contor_a; i++)
		elibereaza_matrice(copie_a[i], l);
	free(copie_a);
	for (long i = 0; i < contor_aux; i++)
		elibereaza_matrice(copie_aux[i], l);
	free(copie_aux);
	for (int i = 0; i < l; i++)
		free(a[i]);
	free(a);
	return aux;
}

void mesaje_ridicare(int m, long putere, int ****a, int *l, int *c, int index)
{
	if (m >= index || m < 0)
		printf("No matrix with the given index\n");
	else if (putere < 0)
		printf("Power should be positive\n");
	else if (l[m] != c[m])
		printf("Cannot perform matrix multiplication\n");
	else
		(*a)[m] = ridicare_timp_log((*a)[m], l[m], putere);
}

int mesaje_afis_transp_redim(int nr, int index)
{
	if (nr >= index || nr < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else {
		return 1;
	}
}

int mesaje_transpusa(int nr, int index)
{
	if (nr >= index || nr < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else {
		return 1;
	}
}

int mesaje_inmultire(int m, int n, int index)
{
	if (m >= index || n >= index || n < 0 || m < 0) {
		printf("No matrix with the given index\n");
		return 0;
	} else {
		return 1;
	}
}

int mesaje_inmultire2(int c1, int l2)
{
	if (c1 != l2) {
		printf("Cannot perform matrix multiplication\n");
		return 0;
	} else {
		return 1;
	}
}

void dimensiuni(int nr, int *lin, int *col, int index)
{
	if (nr >= index || nr < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", lin[nr], col[nr]);
}

int main(void)
{
	int index = 0, m, n, nr, ok = 0;
	char ch;
	int ***a = (int ***)malloc(sizeof(int **));
	verif(a);
	int *lin = (int *)malloc(sizeof(int));
	verif_eroare(lin);
	int *col = (int *)malloc(sizeof(int));
	verif_eroare(col);
	long putere;
	while (ok == 0) {
		scanf("%c", &ch);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
			switch (ch) {
			case 'L':
				scanf("%d %d", &m, &n);
				a[index] = (int **)malloc(m * sizeof(int *));
				a[index] = alocare_matrice(a[index], m, n);
				incarca_matrice(a[index], m, n, &lin[index], &col[index]);
				realocare(&a, &lin, &col, &index);
				break;
			case 'D':
				scanf("%d", &nr);
				dimensiuni(nr, lin, col, index);
				break;
			case 'P':
				scanf("%d", &nr);
				if (mesaje_afis_transp_redim(nr, index) == 1)
					afiseaza_matrice(a[nr], lin[nr], col[nr]);
				break;
			case 'C':
				scanf("%d", &nr);
				if (mesaje_afis_transp_redim(nr, index) == 1)
					a[nr] = redim_matrice(a[nr], &lin[nr], &col[nr]);
				break;
			case 'M':
				scanf("%d %d", &m, &n);
				if (mesaje_inmultire(m, n, index) == 1)
					if (mesaje_inmultire2(col[m], lin[n]) == 1) {
						a[index] = inm_mat(a[m], a[n], lin[m], col[n], lin[n]);
						lin[index] = lin[m];
						col[index] = col[n];
						realocare(&a, &lin, &col, &index);
					}
				break;
			case 'O':
				a = sortare(a, index, lin, col);
				break;
			case 'T':
				scanf("%d", &nr);
				if (mesaje_afis_transp_redim(nr, index) == 1)
					a[nr] = transpusa(a[nr], &lin[nr], &col[nr]);
				break;
			case 'R':
				scanf("%d %ld", &m, &putere);
				mesaje_ridicare(m, putere, &a, lin, col, index);
				break;
			case 'F':
				scanf("%d", &nr);
				if (nr >= index || nr < 0)
					printf("No matrix with the given index\n");
				else
					a = eliberare(a, nr, lin, col, index--);
				break;
			case 'Q':
				for (int i = 0; i < index; i++)
					elibereaza_matrice(a[i], lin[i]);
				free(a);
				free(lin);
				free(col);
				ok = 1;
				break;
			default:
				printf("Unrecognized command\n");
				break;
			}
	}
	return 0;
}
