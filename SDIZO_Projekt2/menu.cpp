#include "stdafx.h"
#include "menu.h"
#include "Graph.h"
using namespace std;
menu::menu()
{
}

menu::~menu()
{
}

int menu::ShowMenu() //Menu g³ówne programu
{
	char select = 'x';
	while (select != '0')
	{
		std::cout << "SDIZO Projekt 2 - Mikolaj Baran nr. 241128" << endl;
		cout << "Wybierz opcje: " << endl;
		cout << "1.Wczytaj graf z pliku" << endl;
		cout << "2.Wygeneruj losowy graf" << endl;
		cout << "3.Testowanie" << endl;
		cout << "0.Wyjscie" << endl;
		select = _getch();

		switch (select)
		{
		case '1':
		{
			Graph *graph;
			VertexList *e1;
			std::string s, filename;
			int a = 0;
			int edgesLoad, verticesLoad, vertexFirst, vertexLast;

			std::cout << "Podaj nazwe pliku grafu (txt)" << std::endl;
			std::cin >> filename;
			if (filename.substr(filename.find_last_of(".") + 1) != "txt") //dodanie do nazwy pliku rozszerzenia .txt gdy go nie ma
			{
				filename += ".txt";
			}

			std::fstream file;
			file.open(filename.c_str(), std::ios::in);
			if (!file.is_open())
			{
				std::cout << "Nie mozna otworzyc pliku" << std::endl;
				break;
			}
			else
			{
				int tab[4];
				if (!file_read_line(file, tab, 4))
				{
					std::cout << "Blad! Niepoprawdne wartosci w pierwszej lini pliku!" << std::endl;
					break;
				}
				else
					edgesLoad = tab[0];
				verticesLoad = tab[1];
				vertexFirst = tab[2];
				vertexLast = tab[3];
				if (!file || edgesLoad < verticesLoad - 1 || verticesLoad <= 1 || edgesLoad >(verticesLoad*(verticesLoad - 1)) || vertexFirst<0 || vertexFirst>verticesLoad || vertexLast<0 || vertexLast>verticesLoad)
				{
					std::cout << "Blad! Niepoprawdne wartosci w pierwszej lini pliku!" << std::endl;
					break;
				}
				else
				{
					graph = new Graph(verticesLoad, edgesLoad, vertexFirst, vertexLast);
					for (int i = 0; i < edgesLoad; i++)
					{
						if (!file_read_line(file, tab, 3))
						{
							std::cout << "Blad! Zle podane krawedzie!" << std::endl;
							break;
						}

						graph->E[a].vf = tab[0];
						graph->E[a].vl = tab[1];
						graph->E[a].weight = tab[2];

						if (graph->E[a].vf >= verticesLoad || graph->E[a].vl >= verticesLoad)
						{
							std::cout << "Blad! Zle podane krawedzie!" << std::endl;
							break;
						}
						if (file)
							a++;
						else
						{
							std::cout << "Blad! Zle podane krawedzie!" << std::endl;
							break;
						}
					}
					if (a == edgesLoad)
					{
						for (int i = 0; i < verticesLoad; i++)
							graph->incidentMatrix[i] = new int[edgesLoad];

						for (int i = 0; i < verticesLoad; i++)
							for (int j = 0; j < edgesLoad; j++)
							{
								graph->incidentMatrix[i][j] = 0;
							}

						for (int i = 0; i < verticesLoad; i++)
							graph->NHoodList[i] = NULL;

						for (int i = 0; i < edgesLoad; i++)
						{
							int wp = graph->E[i].vf;
							int wk = graph->E[i].vl;
							e1 = new VertexList;
							e1->v = wk;
							e1->weight = graph->E[i].weight;
							e1->next = graph->NHoodList[wp];
							graph->NHoodList[wp] = e1;
							graph->incidentMatrix[wp][i] = 1;
							graph->incidentMatrix[wk][i] = -1;
						}
						file.close();
						graph->NotDirectedGraph();

						char select2 = 'y';
						while (select2 != '0') {

							std::cout << "Wybierz opcje:"<<endl<<endl;
							cout << "------------------------------------" << endl;
							cout << "-- - Najkrotsza drogra w grafie-- - " << endl;
							cout << "------------------------------------" << endl;
							cout << "1.Algorytm Dijkstry Lista" << endl;
							cout << "2.Algorytm Dijkstry Macierz" << endl << endl;
							cout << "------------------------------------" << endl;
							cout << "-- - Minimalne drzewo rozpinajace-- - " << endl;
							cout << "------------------------------------" << endl;
							cout << "3.Algorytm Prima Lista" << endl;
							cout << "4.Algorytm Prima Macierz" << endl << endl;
							cout << "5.Pokaz graf(skierowany)" << endl;
							cout << "6.Pokaz graf(nieskierowany)" << endl;
							cout << "0.Wyjscie" << endl;
							select2 = _getch();
							switch (select2)
							{
							case '1':
							{
								graph->DijkstraList(graph->GetFirstVertex());
							} break;
							case '2':
							{
								graph->DijkstraMatrix(graph->GetFirstVertex());
							} break;

							case '3':
							{
								graph->PrimList();
							} break;

							case '4':
							{
								graph->PrimMatrix();
							} break;

							case '5':
							{
								graph->Show();
							} break;

							case '6':
							{
								graph->ShowND();
							} break;

							}
						}

					}
				}
			}

		} break;

		case '2':
		{
			int w, g;
			Graph *graph;
			while (true)
			{
				std::cout << "Podaj liczbe wierzcholkow" << std::endl;
				std::cin >> w;
				if (w > 1) break;
				else std::cout << "Liczba wierzcholkow musi byc wieksza od 1!" << std::endl;
			}

			int maxK = w*(w - 1);
			double minG = ceil((((double)w - 1) * 100) / (double)maxK);

			while (true)
			{
				std::cout << "Podaj gestosc - co najmniej " << minG << " %" << std::endl;
				std::cin >> g;
				if (g < minG || g > 100) std::cout << "Podaj prawidlowa wartosc w %!"
					<< std::endl << "Aby graf byl spojny, gestosc musi wynosic przynajmniej " << minG << " %" << std::endl;
				else
					break;
			}
			double krawedzie = ceil(((double)maxK*g) / 100);
			graph = new Graph(w, krawedzie);
			graph->RandomGraph();

			char select2 = 'y';
			while (select2 != '0') {

				std::cout << "Wybierz opcje:" << endl << endl;
				cout << "------------------------------------" << endl;
				cout << "-- - Najkrotsza drogra w grafie-- - " << endl;
				cout << "------------------------------------" << endl;
				cout << "1.Algorytm Dijkstry Lista" << endl;
				cout << "2.Algorytm Dijkstry Macierz" << endl << endl;
				cout << "------------------------------------" << endl;
				cout << "-- - Minimalne drzewo rozpinajace-- - " << endl;
				cout << "------------------------------------" << endl;
				cout << "3.Algorytm Prima Lista" << endl;
				cout << "4.Algorytm Prima Macierz" << endl << endl;
				cout << "5.Pokaz graf(skierowany)" << endl;
				cout << "6.Pokaz graf(nieskierowany)" << endl;
				cout << "0.Wyjscie" << endl;;
				select2 = _getch();
				switch (select2)
				{
				case '1':
				{
					std::cout << "\nPodaj wierzcholek poczatkowy:";
					int wp;
					std::cin >> wp;
					graph->DijkstraList(wp);
				} break;
				case '2':
				{
					std::cout << "\nPodaj wierzcholek poczatkowy:";
					int wp;
					std::cin >> wp;
					graph->DijkstraMatrix(wp);
				} break;

				case '3':
				{
					graph->PrimList();
				} break;

				case '4':
				{
					graph->PrimMatrix();
				} break;

				case '5':
				{
					graph->Show();
				} break;

				case '6':
				{
					graph->ShowND();
				} break;
				case '7':
				{
					std::cout << "\nPodaj wierzcholek poczatkowy:";
					int wp;
					std::cin >> wp;
					graph->bellman_fordM(wp);
				} break;

				}
			}
		} break;

		case '3':
		{
			char select2 = 'x';
			while (select2 != '0') {

				std::cout << "Wybierz test:\n\n1. DijkstraMacierz\n2. Dijkstra Lista\n3. Prim Macierz\n4. Prim Lista\n0. Wyjscie\n";
				select2 = _getch();
				switch (select2)
				{
				case '1':
				{
					int w, g, r;
					Graph *graph;
					while (true)
					{
						std::cout << "Podaj liczbe wierzcholkow" << std::endl;
						std::cin >> w;
						if (w > 1) break;
						else std::cout << "Liczba wierzcholkow musi byc wieksza od 1!" << std::endl;
					}

					int maxK = w*(w - 1);
					double minG = ceil((((double)w - 1) * 100) / (double)maxK);

					while (true)
					{
						std::cout << "Podaj gestosc - co najmniej " << minG << " %" << std::endl;
						std::cin >> g;
						if (g < minG || g > 100) std::cout << "Podaj prawidlowa wartosc w %!"
							<< std::endl << "Aby graf byl spojny, gestosc musi wynosic przynajmniej " << minG << " %" << std::endl;
						else
							break;
					}
					double krawedzie = ceil(((double)maxK*g) / 100);
					
					std::cout << "Podaj liczbe powtorzen" << std::endl;
					std::cin >> r;

					double temp, timeMid = 0; //sredni czas

					for (int i = 0; i < r; i++) {
						graph = new Graph(w, krawedzie);
						graph->RandomGraph();

						temp = graph->DijkstraMatrixTest(rand() % w);
						timeMid += (temp / r);
						if (i % 10 == 0)
							std::cout << "Petla " << i + 1 << std::endl;
					}

					timeMid *= 1000;
					std::cout << "Sredni czas wykonywania: " << timeMid << " [ms]" << std::endl;
					std::fstream file;
					file.open("wyniki.txt", std::ios::out);
					file << timeMid;
					file.close();

				} break;

				case '2':
				{
					int w, g, r;
					Graph *graph;
					while (true)
					{
						std::cout << "Podaj liczbe wierzcholkow" << std::endl;
						std::cin >> w;
						if (w > 1) break;
						else std::cout << "Liczba wierzcholkow musi byc wieksza od 1!" << std::endl;
					}

					int maxK = w*(w - 1);
					double minG = ceil((((double)w - 1) * 100) / (double)maxK);

					while (true)
					{
						std::cout << "Podaj gestosc - co najmniej " << minG << " %" << std::endl;
						std::cin >> g;
						if (g < minG || g > 100) std::cout << "Podaj prawidlowa wartosc w %!"
							<< std::endl << "Aby graf byl spojny, gestosc musi wynosic przynajmniej " << minG << " %" << std::endl;
						else
							break;
					}
					double krawedzie = ceil(((double)maxK*g) / 100);

					std::cout << "Podaj liczbe powtorzen" << std::endl;
					std::cin >> r;

					double temp, timeMid = 0; //sredni czas

					for (int i = 0; i < r; i++) {
						graph = new Graph(w, krawedzie);
						graph->RandomGraph();

						temp = graph->DijkstraListTest(rand() % w);
						timeMid += (temp / r);
						if (i % 10 == 0)
							std::cout << "Petla " << i + 1 << std::endl;
					}
					timeMid *= 1000;
					std::cout << "Sredni czas wykonywania: " << timeMid << " [ms]"<<std::endl;
					std::fstream file;
					file.open("wyniki.txt", std::ios::out);
					file << timeMid;
					file.close();

				} break;

				case '3':
				{
					int w, g, r;
					Graph *graph;
					while (true)
					{
						std::cout << "Podaj liczbe wierzcholkow" << std::endl;
						std::cin >> w;
						if (w > 1) break;
						else std::cout << "Liczba wierzcholkow musi byc wieksza od 1!" << std::endl;
					}

					int maxK = w*(w - 1);
					double minG = ceil((((double)w - 1) * 100) / (double)maxK);

					while (true)
					{
						std::cout << "Podaj gestosc - co najmniej " << minG << " %" << std::endl;
						std::cin >> g;
						if (g < minG || g > 100) std::cout << "Podaj prawidlowa wartosc w %!"
							<< std::endl << "Aby graf byl spojny, gestosc musi wynosic przynajmniej " << minG << " %" << std::endl;
						else
							break;
					}
					double krawedzie = ceil(((double)maxK*g) / 100);

					std::cout << "Podaj liczbe powtorzen" << std::endl;
					std::cin >> r;

					double temp, timeMid = 0; //sredni czas

					for (int i = 0; i < r; i++) {
						graph = new Graph(w, krawedzie);
						graph->RandomGraph();

						temp = graph->PrimMatrixTest();
						timeMid += (temp / r);
						if (i % 10 == 0)
							std::cout << "Petla " << i + 1 << std::endl;
					}

					timeMid *= 1000;
					std::cout << "Sredni czas wykonywania: " << timeMid << " [ms]" << std::endl;
					std::fstream file;
					file.open("wyniki.txt", std::ios::out);
					file << timeMid;
					file.close();

				} break;

				case '4':
				{
					int w, g, r;
					Graph *graph;
					while (true)
					{
						std::cout << "Podaj liczbe wierzcholkow" << std::endl;
						std::cin >> w;
						if (w > 1) break;
						else std::cout << "Liczba wierzcholkow musi byc wieksza od 1!" << std::endl;
					}

					int maxK = w*(w - 1);
					double minG = ceil((((double)w - 1) * 100) / (double)maxK);

					while (true)
					{
						std::cout << "Podaj gestosc - co najmniej " << minG << " %" << std::endl;
						std::cin >> g;
						if (g < minG || g > 100) std::cout << "Podaj prawidlowa wartosc w %!"
							<< std::endl << "Aby graf byl spojny, gestosc musi wynosic przynajmniej " << minG << " %" << std::endl;
						else
							break;
					}
					double krawedzie = ceil(((double)maxK*g) / 100);

					std::cout << "Podaj liczbe powtorzen" << std::endl;
					std::cin >> r;

					double temp, timeMid = 0; //sredni czas

					for (int i = 0; i < r; i++) {
						graph = new Graph(w, krawedzie);
						graph->RandomGraph();

						temp = graph->PrimListTest();
						timeMid += (temp / r);
						if (i % 10 == 0)
							std::cout << "Petla " << i + 1 << std::endl;
					}

					timeMid *= 1000;
					std::cout << "Sredni czas wykonywania: " << timeMid << " [ms]" << std::endl;
					std::fstream file;
					file.open("wyniki.txt", std::ios::out);
					file << timeMid;
					file.close();
				} break;
				}
			}
		} break;

		}
		std::cout << std::endl;
	}
	return 0;
}

bool menu::file_read_line(std::fstream &file, int tab[], int size)
{
	std::string s;
	getline(file, s);
	if (file.fail() || s.empty())
		return(false);

	std::istringstream in_ss(s);

	for (int i = 0; i < size; i++)
	{
		in_ss >> tab[i];
		if (in_ss.fail())
			return(false);
	}
	return(true);
}