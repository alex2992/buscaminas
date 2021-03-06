#include "Puntuacions.h"

Puntuacions::Puntuacions(void)
{
	m_llistat = nullptr;
}

Puntuacions::~Puntuacions(void)
{
	if	(m_llistat != nullptr)
	{
		delete [] m_llistat;
		m_llistat = nullptr;
	}
}

/*
 * Llegeix el llistat de resultats d'un arxiu.
 * La primera l�nia de l'arxiu �s el nombre de entrades del llistat
 * Cada entrada t� el format: NOM NIVELL PUNTUACIO
 */
void Puntuacions::m_llegirLlistat(void)
{
	ifstream fitxerResultats;
	char linia[200];

	m_numElements = -1;
	fitxerResultats.open (ARXIU_RESULTATS);
	if(fitxerResultats.is_open())
	{
		// Comptem el nombre d'elments per poder crear l'array din�mic
		while (!fitxerResultats.eof())
		{
			fitxerResultats.getline(linia, 200);
			m_numElements++;
		}

		// Torna l'apuntador de l'arxiu al inici
		fitxerResultats.clear();
		fitxerResultats.seekg(0, ios::beg);

		// Creem l'array i l'omplim
		m_llistat = new tResultat[m_numElements];
		int i = 0;
		while (!fitxerResultats.eof())
		{
			fitxerResultats >> m_llistat[i].nom >> m_llistat[i].nivell >> m_llistat[i].puntuacio;
			i++;
		}
	}

	fitxerResultats.close();
}

/*
 * Afegeix una nova puntuaci� al final de l'arxiu
 * @param resultat: nova entrada que cal afegir a l'arxiu
 */
void Puntuacions::desaResultat(tResultat resultat)
{
	ofstream fitxerResultats;

	fitxerResultats.open (ARXIU_RESULTATS, ios_base::app);
	if(fitxerResultats.is_open())
	{
 		fitxerResultats << resultat.nom << " " << resultat.nivell << " " << resultat.puntuacio << endl;
		fitxerResultats.close();
	}
	else 
		cout << "Error obrint el fitxer de resultats" << endl;
}

/**
 * Intercanvia els valors de dues posicions del vector
 * @param pos1
 * @param pos2
 */
void Puntuacions::m_intercanviar(int pos1, int pos2)
{
	tResultat auxi;
	auxi = m_llistat[pos1];
	m_llistat[pos1] = m_llistat[pos2];
	m_llistat[pos2] = auxi;

}

/**
 * Retorna la posici� del pivot en un vector entre els �ndexs primer i ultim
 * @param primer
 * @param ultim
 * @returns int: posici� del pivot
 */
int Puntuacions::m_seleccionarPivot(int primer, int ultim)
{
	return (primer + ultim) / 2;
}

/**
 * Divideix el vector entre els �ndexs primer i ultim, i retorna la posici� del pivot
 * per refer�ncia
 * @param primer
 * @param ultim
 * @param posPivot: retorna la posici� del pivot per refer�ncia
 */
void Puntuacions::m_dividirVector(int primer, int ultim, int& posPivot)
{
	int indexEsq, indexDreta;
	m_intercanviar(primer, posPivot); /* Pivot a la 1� pos. */ 
	posPivot = primer;
	indexEsq = primer; indexDreta = ultim;
	while (indexEsq != indexDreta)
	{ /* A la dreta elements >= pivot */ 
		switch (m_criteri)
		{
		case ORDRE_NOM:

			while ((indexDreta > indexEsq) && (m_llistat[indexDreta].nom > m_llistat[posPivot].nom))
				indexDreta = indexDreta - 1;
			/* A l�esquerra elements <= pivot */
			while ((indexEsq < indexDreta) && (m_llistat[indexEsq].nom <= m_llistat[posPivot].nom))
				indexEsq = indexEsq + 1;
			if (m_llistat[indexDreta].nom < m_llistat[indexEsq].nom)
				m_intercanviar(indexEsq, indexDreta);
			break;

		case ORDRE_NIVELL:

			while ((indexDreta > indexEsq) && (m_llistat[indexDreta].nivell > m_llistat[posPivot].nivell))
				indexDreta = indexDreta - 1;
			/* A l�esquerra elements <= pivot */
			while ((indexEsq < indexDreta) && (m_llistat[indexEsq].nivell <= m_llistat[posPivot].nivell))
				indexEsq = indexEsq + 1;
			if (m_llistat[indexDreta].nivell < m_llistat[indexEsq].nivell)
				m_intercanviar(indexEsq, indexDreta);
			break;
		case ORDRE_PUNTUACIO:

			while ((indexDreta > indexEsq) && (m_llistat[indexDreta].puntuacio < m_llistat[posPivot].puntuacio))
				indexDreta = indexDreta - 1;
			/* A l�esquerra elements <= pivot */
			while ((indexEsq < indexDreta) && (m_llistat[indexEsq].puntuacio >= m_llistat[posPivot].puntuacio))
				indexEsq = indexEsq + 1;
			if (m_llistat[indexDreta].puntuacio > m_llistat[indexEsq].puntuacio)
				m_intercanviar(indexEsq, indexDreta);
			break;
		}
	} 
	m_intercanviar(indexEsq, posPivot);
	posPivot = indexEsq; /* Colocar el pivot a la seva posici� */
}

/**
 * Ordena el vector utilitzant l'algorisme QuickSort
 * @param primer: �ndex del primer element
 * @param ultim: �ndex del darrer element
 */
void Puntuacions::m_ordenar(int primer, int ultim)
{
	int posPivot;
	if (ultim > primer) /* Hi ha m�s d�un element per ordenar */ 
	{ 
		posPivot = m_seleccionarPivot(primer, ultim); 
		m_dividirVector(primer, ultim, posPivot);
		m_ordenar(primer, posPivot - 1); 	/* Ordenar V1 */
		m_ordenar(posPivot + 1, ultim); /* Ordenar V2 */
	}
}

/**
 * Mostra el llistat de puntuacions
 */
void Puntuacions::mostrar(void)
{
	system("cls");
	cout << left;
	for (int i=0; i < m_numElements; i++)
	{
		switch (m_criteri)
		{
		case ORDRE_NOM:
			cout << "* " << setw(15) << m_llistat[i].nom << " " << m_llistat[i].nivell << " " << m_llistat[i].puntuacio << endl;
			break;
		case ORDRE_NIVELL:
			cout << " * " << m_llistat[i].nivell << " " << setw(15) << m_llistat[i].nom << " " << m_llistat[i].puntuacio << endl;
			break;
		case ORDRE_PUNTUACIO:
			cout << "* " << setw(5) << m_llistat[i].puntuacio << " " << m_llistat[i].nivell << " " << m_llistat[i].nom << endl;
			break;
		}
	}
	_getch();
	
}

/**
 * Mostra el llistat de puntuacions ordenat per nom
 */
void Puntuacions::veureOrdenatPerNom(void)
{
	m_criteri = ORDRE_NOM;
	m_llegirLlistat();
	m_ordenar(0, m_numElements-1);
	mostrar();
}

/**
 * Mostra el llistat de puntuacions ordenat per nivell
 */
void Puntuacions::veureOrdenatPerNivell(void)
{
	m_criteri = ORDRE_NIVELL;
	m_llegirLlistat();
	m_ordenar(0, m_numElements-1);
	mostrar();
}

/**
 * Mostra el llistat de puntuacions ordenat per puntuaci�
 */
void Puntuacions::veureOrdenatPerPuntuacio(void)
{
	m_criteri = ORDRE_PUNTUACIO;
	m_llegirLlistat();
	m_ordenar(0, m_numElements-1);
	mostrar();
}

