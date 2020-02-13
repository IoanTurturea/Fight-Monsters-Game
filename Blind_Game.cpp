#include "pch.h"

//acest joc a fost inspirat din: https://www.learncpp.com/cpp-tutorial/11-x-chapter-11-comprehensive-quiz/

using namespace std;
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


int getRandomNumber(int min, int max)// Aceasta este o functie pentru a genera un numar la intamplare intre valorile min si max(inclusiv)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	// Sursa pentru codul functiei: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
}

class Fiinta {
protected:
	string m_nume;
	int m_viata;
	int m_distruge;
	int m_aur;
	int m_sageti;
	int m_potiune;
public:
	Fiinta(string nume, int viata, int distruge, int aur, int sageti, int potiune) :
		m_nume(nume), m_viata(viata), m_distruge(distruge), m_aur(aur), m_sageti(sageti), m_potiune(potiune)
	{
	}
	string getNume() { return m_nume; }
	int getViata() { return m_viata; }
	int getAur() { return m_aur; }
	int getDistruge() { return m_distruge; }
	int getSageti() { return m_sageti; }
	int getPotiune() { return m_potiune; }
	void reduceViata(int distruge) { m_viata -= distruge; }
	bool eMort() { return m_viata <= 0; }
	void plusAur(int aur) { m_aur += aur; }
	void plusSageti(int sageti) { m_sageti += sageti; }
	void plusPotion(int potiune) { m_potiune += potiune; }
	void minusSageti(int sageti) { m_sageti -= sageti; }
	void plusViata(int viata) { m_viata += viata; }
	void minusPotiune(int potiune) { m_potiune -= potiune; }
	void resetPotiune() { m_potiune = 0; }
	void resetSageti() { m_sageti = 7; }
	//	void resetViata()				{ m_viata = ; } // o sa revin
};



class Jucator : public Fiinta {
protected:
	int m_nivel = 1;
public:
	Jucator(string nume) :
		Fiinta(nume, 10, 1, 0, 5, 0)
	{
	}
	void cresteNivel()
	{
		++m_nivel;
		++m_distruge;
	}
	int getNivel() { return m_nivel; }
	bool aCastigat() { return m_nivel > 30; }
};



class Monstru : public Fiinta {
public:
	enum TipMonstru {
		SALBATIC,
		ZMEU,
		VARCOLAC,
		CAPCAUN,
		DRAGON,
		NECUNOSCUT,
		MAX
	};
	struct DateMonstru {
		const char *nume;
		int viata;
		int distruge;
		int aur;
		int sageti;
		int potiune;
	};
	static DateMonstru v[MAX];
	TipMonstru m_tip;

	Monstru(TipMonstru tip) :
		Fiinta(v[tip].nume, v[tip].viata, v[tip].distruge, v[tip].aur, v[tip].sageti, v[tip].potiune)
	{
	}

	static Monstru Monstru_la_intamplare()
	{
		int x = getRandomNumber(0, MAX - 4);
		return Monstru(static_cast<TipMonstru>(x));
	}
	static Monstru Monstru_la_intamplare2()
	{
		int x = getRandomNumber(1, MAX - 3);
		return Monstru(static_cast<TipMonstru>(x));
	}
	static Monstru Monstru_la_intamplare3()
	{
		int x = getRandomNumber(2, MAX - 2);
		return Monstru(static_cast<TipMonstru>(x));
	}
	static Monstru necunoscut()
	{
		return Monstru(NECUNOSCUT);
	}
	//	friend bool operator== (const Monstru &c1, const TipMonstru &c2)
	//	{
	//		return (c1.m_tip == c2);
	//	}
};


void Dificultate(char c, Monstru *v);

Monstru::DateMonstru Monstru::v[Monstru::MAX]
{
	{"Salbatic al padurii", 2,   1,  0,   1, 0},
	{           "Varcolac", 3,   2,  5,   2, 0},
	{               "Zmeu", 7,   3,  10,  3, 0},
	{            "Capcaun", 20,  5,  30,  4, 0},
	{             "Dragon", 30,  8,  50,  5, 1},
	{    "MONSTRU NEVAZUT", 140, 12, 500, 0, 0}
};



void atacaJucator(Monstru &m, Jucator &j)
{
	if (m.eMort())
		return;

	j.reduceViata(m.getDistruge());
	cout << " " << m.getNume() << " te-a atacat cu " << m.getDistruge() << endl;
}



void atacaMonstru(Jucator &j, Monstru &m, char c)
{
	if (j.eMort())
		return;

	if (c == 'a' || c == 'A')
	{
		cout << " Ai atacat " << m.getNume() << " cu " << j.getDistruge() << ".\n";
		m.reduceViata(j.getDistruge());
	}

	if (c == 's' || c == 'S')
	{
		if (j.getSageti() <= 0)
		{
			cout << " Nu mai ai sageti.\n";
			atacaJucator(m, j);
		}
		else
		{
			m.reduceViata(1);
			j.minusSageti(1);
			cout << " Ai dat damage cu o sageata 1 puncte. Mai ai " << j.getSageti() << endl;
		}

	}

	if (c == 'W' || c == 'w')
	{
		if (j.getPotiune() == 0)
		{
			cout << " Nu mai ai potiuni.\n";
			atacaJucator(m, j);
		}
		else
		{
			m.reduceViata(10);
			j.minusPotiune(1);
			cout << " Ai aruncat cu o potiune. Mai ai " << j.getPotiune() << endl;
		}
	}

	if (c == 'l' || c == 'L')
	{
		if (j.getSageti() <= 0)
		{
			cout << " Nu mai ai sageti de foc!\n";
			atacaJucator(m, j);
		}
		else
		{
			m.reduceViata(7);
			j.minusSageti(1);
			cout << " Ai tras cu o sageata de foc. Mai ai " << j.getSageti() << endl;
		}
	}
	if (c == '3')
	{
		if (j.getSageti() <= 2)
		{
			cout << " Ai mai putin de 3 sageti!\n";
			atacaJucator(m, j);
		}
		else
		{
			m.reduceViata(3);
			j.minusSageti(3);
			cout << " Ai dat damage cu 3 sageti 3 puncte. Mai ai " << j.getSageti() << endl;
		}
	}
	if (m.eMort())
	{
		j.plusAur(m.getAur());
		j.plusSageti(m.getSageti());
		j.cresteNivel();
		cout << " Ai omorat " << m.getNume() << ".";
		if (Monstru::DRAGON)
		{
			j.plusPotion(m.getPotiune());
			cout << "\n Ai gasit " << m.getAur() << " monede si " << m.getSageti() << " sageti si " << m.getPotiune() << " potiuni\n";
		}
		else if (Monstru::NECUNOSCUT)
		{
			cout << "\n Ai gasit " << m.getAur() << " monede!\n\n";
		}
		else
			cout << "\n Ai gasit " << m.getAur() << " monede si " << m.getSageti() << " sageti.\n";
		cout << " Acum ai nivelul " << j.getNivel() << ".\n";
		// Modul de citire de deasupra este precar si nu da rezultatele dorite.
		// Am incercat cu supraincarcatrea operatorului == astfel : if(m == Montru::DRAGON){} 
		// dar nu a functionat, la fel ca si alte metode... p.p: sintaxa din if() e gresita!
		// totodata, sintaxa actuala functioneaza doar pe jumatate.
	}
}



void lupta(Jucator &j)
{
	Monstru	m = Monstru::Monstru_la_intamplare();

	cout << "\n Te-ai intalnit cu un " << m.getNume() << endl;
	while (!m.eMort() && !j.eMort())
	{
		cout << " (A),(S),(D),(W),(3): ";
		char c;
		cin >> c;
		if (c == 'A' || c == 'a')
		{
			atacaMonstru(j, m, c);
			atacaJucator(m, j);
		}
		if (c == 'S' || c == 's')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'D' || c == 'd')
		{
			int f = getRandomNumber(0, 1);
			if (f == 1)
			{
				cout << " Ai reusit sa te ascunzi cu bine.\n";
				return;
			}
			if (f == 0)
			{
				cout << " Nu te-ai putut ascunde.\n";
				atacaJucator(m, j);
			}
		}
		if (c == '3')
		{
			atacaMonstru(j, m, c);
		}
	}
}



void lupta1(Jucator &j)
{
	Monstru	m = Monstru::Monstru_la_intamplare2();

	cout << "\n Te-ai intalnit cu un " << m.getNume() << endl;
	while (!m.eMort() && !j.eMort())
	{
		cout << " (A),(S),(D),(W),(3): ";
		char c;
		cin >> c;;
		if (c == 'A' || c == 'a')
		{
			atacaMonstru(j, m, c);
			atacaJucator(m, j);
		}
		if (c == 'S' || c == 's')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'D' || c == 'd')
		{
			int f = getRandomNumber(0, 1);
			if (f == 1)
			{
				cout << " Ai reusit sa te ascunzi cu bine.\n";
				return;
			}
			if (f == 0)
			{
				cout << " Nu te-ai putut ascunde.\n";
				atacaJucator(m, j);
			}
		}
		if (c == '3')
		{
			atacaMonstru(j, m, c);
		}
	}
}



void lupta2(Jucator &j)
{
	Monstru	m = Monstru::Monstru_la_intamplare3();

	cout << "\n Te-ai intalnit cu un " << m.getNume() << endl;
	while (!m.eMort() && !j.eMort())
	{
		cout << " (A),(S),(D),(W),(3): ";
		char c;
		cin >> c;
		if (c == 'A' || c == 'a')
		{
			atacaMonstru(j, m, c);
			atacaJucator(m, j);
		}
		if (c == 'S' || c == 's')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'D' || c == 'd')
		{
			int f = getRandomNumber(0, 1);
			if (f == 1)
			{
				cout << " Ai reusit sa te ascunzi cu bine.\n";
				return;
			}
			if (f == 0)
			{
				cout << " Nu te-ai putut ascunde.\n";
				atacaJucator(m, j);
			}
		}
		if (c == 'W' || c == 'w')
		{
			atacaMonstru(j, m, c);
		}
		if (c == '3')
		{
			atacaMonstru(j, m, c);
		}
	}
}



void lupta3(Jucator &j)
{
	Monstru	m = Monstru::necunoscut();

	while (!m.eMort() && !j.eMort())
	{
		cout << "\n (A),(S),(D),(W),(3): ";
		char c;
		cin >> c;
		if (c == 'A' || c == 'a')
		{
			atacaMonstru(j, m, c);
			atacaJucator(m, j);
		}
		if (c == 'S' || c == 's')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'D' || c == 'd')
		{
			int f = getRandomNumber(0, 1);
			if (f == 1)
			{
				cout << " Ai reusit sa te ascunzi cu bine.\n";
				return;
			}
			if (f == 0)
			{
				cout << " Nu te-ai putut ascunde.\n";
				atacaJucator(m, j);
				continue;
			}
		}
		if (c == 'W' || c == 'w')
		{
			atacaMonstru(j, m, c);
		}
		if (c == '3')
		{
			atacaMonstru(j, m, c);
		}
	}
}



void lupta4(Jucator &j)
{
	Monstru	m = Monstru::necunoscut();

	while (!m.eMort() && !j.eMort())
	{
		cout << "\n (A),(S),(D),(L),(3): ";
		char c;
		cin >> c;
		if (c == 'A' || c == 'a')
		{
			atacaMonstru(j, m, c);
			atacaJucator(m, j);
		}
		if (c == 'S' || c == 's')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'D' || c == 'd')
		{
			int f = getRandomNumber(0, 1);
			if (f == 1)
			{
				cout << " Ai reusit sa te ascunzi cu bine.\n";
				return;
			}
			if (f == 0)
			{
				cout << " Nu te-ai putut ascunde.\n";
				atacaJucator(m, j);
				continue;
			}
		}
		if (c == 'W' || c == 'w')
		{
			atacaMonstru(j, m, c);
		}
		if (c == 'l' || c == 'L')
		{
			atacaMonstru(j, m, c);
		}
		if (c == '3')
		{
			atacaMonstru(j, m, c);
		}
	}
}





int main()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();
	cout << "\n\n\n\t\t\t\t BLIND GAME \n\n\n";
	cout << "\t\t\t  Introduceti-va numele:\n\t\t\t\t";
	string nume;
	cin >> nume;

	cout << "\n\t\t\t  Bun venit, " << nume << ".\n\n\n";
	Jucator j(nume); 

	cout << " Regulile jocului sunt urmatoarele: \n\n";
	cout << " 1. Cand te intalnesti cu un monstru poti sa te lupti, sa tragi cu arcul sau\n    sa te ascunzi pana trece.\n";
	cout << " 2. Daca alegi sa te ascunzi, ai 50% sanse sa scapi.\n";
	cout << " 3. Daca alegi sa tragi cu arcul, monstrul nu te va ataca atata timp cat\n    tragi cu sageti.\n";
	cout << " 4. Cand omori un dragon, acesta mai are in plus si o potiune, pe care o poti\n    folosi sa ia 10 din viata adversarului.\n";
	cout << " 5. Creaturile puse in ordinea pericolului sunt: Salbatic, Varcolac, Zmeu,\n    Capcaun, Dragon.\n";
	cout << " 6. De asemenea, distrugerea provocata de lovitura ta creste cu fiecare monstru\n    pe care il omori.\n";

	cout << "\n Instructiunile jocului sunt urmatoarele: \n\n"
		<< " 1. A = Ataca\n"
		<< " 2. S = Trage cu arcul\n"
		<< " 3. D = Ascunde-te\n"
		<< " 4. W = Arunca o potiune\n"
		<< " 5. 3 = Trage cu 3 sageti o data.\n\n";
	/*
	<< "\n\n Cum vrei sa joci jocul?\n"
	<< "\n Usor(U)"
	<< "\n Mediu(M)"
	<< "\n Greu(G)"
	<< "\n Imposibil(I)\n";
	char c;
	cout << " ";
	cin >> c;
	// poate o sa main revin aici
*/
	cout << "\n\n Capitolul I. Padurea\n\n\n";
	cout << " Cari cu tine o sabie, 0 monezi, 5 sageti, 0 potiuni, si ai 10 la viata.\n\n\n";

	while (!j.eMort() && !j.aCastigat() && j.getNivel() < 10)
	{
		lupta(j);

		if (j.getNivel() == 10)
		{
			cout << "\n\n Ai gasit pe jos o potiune!";
			cout << " Aceasta ti-a crescut viata cu 3";
			j.plusViata(3);
			cout << " Acum ai " << j.getViata() << " la viata, " << " " << j.getSageti() << " sageti si " << j.getPotiune() << " potiuni.\n";
			cout << "\n\n Capitolul II. Muntii\n\n\n";
			cout << " Ai iesit cu bine din padure si acum ai de urcat pe muntii vineti.\n\n\n\n";
		}
	}

	while (!j.eMort() && !j.aCastigat() && j.getNivel() < 20 && j.getNivel() >= 10)
	{
		lupta1(j);
	}

	while (!j.eMort() && !j.aCastigat() && j.getNivel() < 30 && j.getNivel() >= 20)
	{
		if (j.getNivel() == 20)
		{
			cout << " \n Ai gasit pe jos o potiune!";
			cout << " Aceasta ti-a crescut viata cu 5.";
			j.plusViata(5);
			cout << " Acum ai " << j.getViata() << " la viata, " << j.getSageti() << " sageti si " << j.getPotiune() << " potiuni.\n";
			cout << " Ai ajuns la nivelul 20.\n\n\n";
			cout << " Capitolul III. Grota...\n\n\n";
			cout << " Ai gasit o grota mare si adanca din care iese fum si ai coborat in ea.\n\n\n\n\n";
		}
		lupta2(j);
	}

	if (!j.eMort() && !j.aCastigat() && j.getNivel() >= 30)
	{
		cout << "\n Te-ai intalnit cu Sfanta Vineri. Aceasta ti-a aparut sa te\n";
		cout << " ajute sa il omori pe cel rau si te-a binecuvantat\n";
		cout << " capatand astfel 10 la viata!\n";
		j.plusViata(10);
		cout << "\n Acum ai " << j.getViata() << " la viata, " << j.getSageti() << " sageti si " << j.getPotiune() << " potiuni.\n\n\n\n\n";
		cout << " Ai ajuns la finalul jocului, nivelul 30.\n\n";
		cout << " Ai intrat intr-o imensa sala sapata in munte si vezi un contur negru in\n";
		cout << " departare. Ai strigat si urmeaza sa va luptati. Acesta este monstrul\n";
		cout << " cel mai periculos. Dar este atat de intuneric in acest loc,\n";
		cout << " incat nu ii vezi decat conturul...\n";
		cout << "\n Te-ai intalnit cu un MONSTRU NEVAZUT!" << endl;
	}
	while (!j.eMort() && !j.aCastigat() && j.getNivel() >= 30)
	{
		lupta3(j);
	}

	if (j.eMort() && !j.aCastigat() && j.getNivel() >= 30)
	{
		cout << "\n\n Ai murit.\n\n Dar pentru ca ai ajuns asa de departe mai ai o sansa.\n";
		cout << " Insa item-urile tale vor reveni la setarile de la inceputul\n";
		cout << " jocului. Ca sa te ajute, Sfanta Vineri ti-a trimis 7 sageti de foc!\n";
		cout << " Acestea iau cate 7 puncte din viata adversarului!\n";
		cout << " Poti sa tragi cu ele pasand pe tasta 'L'.\n";
		j.resetPotiune(); j.resetSageti(); //j.resetViata();
		lupta4(j);
		if (j.eMort())
			goto Aici;
	}
Aici:
	if (j.eMort())
	{
		cout << "\n\n\n Ai murit la nivelul " << j.getNivel() << ", cu " << j.getAur() << " monede." << endl;
		cout << " Pacat ca nu poti sa le iei cu tine.\n\n\n";
	}

	if (j.aCastigat())
	{
		cout << "\n\n\n\n AI CASTIGAT!!!\n\n";
		cout << " Si ai acumulat " << j.getAur() << " monede. Acum te poti intoarce in satul tau si sa \n";
		cout << " iti ajuti satul cu monezile gasite!\n\n\n FELICITARI!\n\n\n\n";
	}
	system("pause");
	return 0;
}





