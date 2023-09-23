#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
#include "wVector.h"
#include "wVector.cpp"

using namespace std;

class City;

class NeighbourNode {
public:
	City* from;
	int distance;
	NeighbourNode(City* a, int d) {
		from = a;
		this->distance = d;
	}
};

class City {
public:
	char name[100];
	int x, y;
	wVector<NeighbourNode*> Neighbours;
	City(char* name, int x, int y) {
		strcpy(this->name, name);
		this->x = x;
		this->y = y;
	}
	void PrintNeighbours() {
		for (int i = 0; i < Neighbours.GetSize(); i++) {
			cout << Neighbours.index(i)->from->name << " " << Neighbours.index(i)->distance << " ";
		}
	}
};

int isShorterWay(City* city, char* name, int d) {
	for (int i = 0; i < city->Neighbours.GetSize(); i++) {
		if (strcmp(city->Neighbours.index(i)->from->name, name) == 0 && city->Neighbours.index(i)->distance >= d) {
			city->Neighbours.index(i)->distance = d;
			return 1;
		}
	}
	return 0;
}

void ParseCityName(int new_x, int new_y, int BOARD_WIDTH, int BOARD_HEIGHT, char* buffer, char** Map) {
	int i = 0;
	while (new_x > 0) {
		if (Map[new_y][new_x - 1] >= 48 && Map[new_y][new_x - 1] <= 90 && new_x >= 0)new_x--;
		else break;
	}
	while (new_x < BOARD_WIDTH) {
		if (Map[new_y][new_x] < 48 || Map[new_y][new_x]>90)break;
		buffer[i] = Map[new_y][new_x];
		i++;
		new_x++;
	}
	buffer[i] = '\0';
}

void FindCityName(char** Map, char *buffer, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT) {
	int new_x, new_y;
	int i = 0;
	if (x + 1 < BOARD_WIDTH && Map[y][x + 1] >= 48 && Map[y][x + 1] <= 90) {
		new_y = y;
		new_x = x+1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (x - 1 >= 0 && Map[y][x - 1] >= 48 && Map[y][x - 1] <= 90) {
		new_y = y;
		new_x = x-1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (x + 1 < BOARD_WIDTH && y + 1 < BOARD_HEIGHT && Map[y + 1][x + 1] >= 48 && Map[y + 1][x + 1] <= 90) {
		new_y = y+1;
		new_x = x+1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (x - 1 >= 0 && y - 1 >= 0 && Map[y - 1][x - 1] >= 48 && Map[y - 1][x - 1] <= 90) {
		new_y = y-1;
		new_x = x-1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (x - 1 >= 0 && y + 1 < BOARD_HEIGHT && Map[y + 1][x - 1] >= 48 && Map[y + 1][x - 1] <= 90) {
		new_y = y+1;
		new_x = x-1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (y - 1 >= 0 && x + 1 < BOARD_WIDTH && Map[y - 1][x + 1] >= 48 && Map[y - 1][x + 1] <= 90) {
		new_y = y-1;
		new_x = x+1;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (y + 1 < BOARD_HEIGHT && Map[y + 1][x] >= 48 && Map[y + 1][x] <= 90) {
		new_y = y+1;
		new_x = x;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
	else if (y - 1 >= 0 && Map[y - 1][x] >= 48 && Map[y - 1][x] <= 90) {
		new_y = y-1;
		new_x = x;
		ParseCityName(new_x, new_y, BOARD_WIDTH, BOARD_HEIGHT, buffer, Map);
	}
}

bool isValid(char** Map, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT) {
	return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT && (Map[y][x] == '#' || Map[y][x]=='*');
}

// Funkcja wykorzystuj¹ca DFS do przeszukiwania grafu
void dfs(char** Map, bool **Visited, int** Distance, int x, int y, int d, int BOARD_WIDTH, int BOARD_HEIGHT,char *name, wVector<City*> Graph, City* city) {
	Visited[y][x] = true;
	Distance[y][x] = d;
	if (Map[y][x] == '*' && d!=0) {
		Visited[y][x] = false;
		char* name2=NULL;
		for (int i = 0; i < Graph.GetSize(); i++) {
			if (Graph.index(i)->x == x && Graph.index(i)->y == y) {
				name2 = Graph.index(i)->name;
				if (!isShorterWay(Graph.index(i), name, d)) {
					NeighbourNode* temp = new NeighbourNode(city, d);
					Graph.index(i)->Neighbours.push_back(temp);
				}
			}
		}
		return;
	}

	// SprawdŸ wszystkich s¹siadów wierzcho³ka (x, y)
	if (isValid(Map, x + 1, y, BOARD_WIDTH, BOARD_HEIGHT) && Visited[y][x + 1]==false) {
		//cout << "caseRIGHT" << endl;
		dfs(Map, Visited, Distance, x + 1, y, d + 1, BOARD_WIDTH, BOARD_HEIGHT, name, Graph,city);
	}
	if (isValid(Map, x - 1, y, BOARD_WIDTH, BOARD_HEIGHT) && Visited[y][x - 1] == false) {
		//cout << "caseLEFT" << endl;
		dfs(Map, Visited, Distance, x - 1, y, d + 1, BOARD_WIDTH, BOARD_HEIGHT, name, Graph,city);
	}
	if (isValid(Map, x, y + 1, BOARD_WIDTH, BOARD_HEIGHT) && Visited[y + 1][x] == false) {
		//cout << "caseDOWN" << endl;
		dfs(Map, Visited, Distance, x, y + 1, d + 1, BOARD_WIDTH, BOARD_HEIGHT, name, Graph,city);
	}
	if (isValid(Map, x, y - 1, BOARD_WIDTH, BOARD_HEIGHT) && Visited[y - 1][x] == false) {
		//cout << "caseUP" << endl;
		dfs(Map, Visited, Distance, x, y - 1, d + 1, BOARD_WIDTH, BOARD_HEIGHT, name, Graph,city);
	}
}

int main() {
	int BOARD_WIDTH, BOARD_HEIGHT, k, flight_distance;
	char buffer[100];
	char buffer2[100];
	int roadCounter = 0;
	char** Map = nullptr;
	bool** Visited = nullptr;
	int** Distance = nullptr;
	wVector<City*> Graph;
	cin >> BOARD_WIDTH >> BOARD_HEIGHT;

	Map = new char* [BOARD_HEIGHT];
	Visited = new bool* [BOARD_HEIGHT];
	Distance = new int* [BOARD_HEIGHT];

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		Map[i] = new char[BOARD_WIDTH];
		Visited[i] = new bool[BOARD_WIDTH];
		Distance[i] = new int[BOARD_WIDTH];
	}

	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			cin >> Map[y][x];
			if (Map[y][x] == '#')roadCounter++;
			Visited[y][x] = false;
		}
	}

	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (Map[y][x] == '*') {
				FindCityName(Map, buffer, x, y, BOARD_WIDTH, BOARD_HEIGHT);
				City* temp = new City(buffer, x, y);
				Graph.push_back(temp);
			}
		}
	}

	for (int i = 0; i < Graph.GetSize(); i++) {
		dfs(Map, Visited, Distance, Graph.index(i)->x, Graph.index(i)->y, 0, BOARD_WIDTH, BOARD_HEIGHT, Graph.index(i)->name, Graph,Graph.index(i));
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				Visited[y][x] = false;
			}
		}
	}
	cin >> k;
	for (int i = 0; i < k; i++) {
		cin >> buffer >> buffer2 >> flight_distance;
		for (int i = 0; i < Graph.GetSize(); i++) {
			if (strcmp(Graph.index(i)->name, buffer) == 0 && !isShorterWay(Graph.index(i), buffer2, flight_distance)) {
				City* temp2 = nullptr;
				for (int j = 0; j < Graph.GetSize(); j++) {
					if (strcmp(Graph.index(j)->name, buffer2) == 0)temp2 = Graph.index(j);
				}
				NeighbourNode* temp = new NeighbourNode(temp2, flight_distance);
				Graph.index(i)->Neighbours.push_back(temp);
			}
		}
	}
	for (int i = 0; i < Graph.GetSize(); i++) {
		cout << Graph.index(i)->name << ": ";
		Graph.index(i)->PrintNeighbours();
		cout << endl;
		
	}
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		delete[] Map[i];
		delete[] Visited[i];
	}
	delete[] Map;
	delete[] Visited;
	return 0;
}