#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "wVector.h"
#include "wVector.cpp"

using namespace std;

class City;

class NeighbourNode {
public:
        City* toCity;
        int distance;
        NeighbourNode(City* toCity, int distance);
};

class City {
public:
        char* name;
        City* prev;
        wVector<NeighbourNode*> Neighbours;
        int x, y, dist;
        bool visited;
        City(char* buffer, int x, int y);
        ~City();
};

NeighbourNode::NeighbourNode(City* toCity, int distance) {
        this->toCity = toCity;
        this->distance = distance;
}

City::City(char* buffer, int x, int y) {
        name = new char[strlen(buffer)];
        strcpy(this->name, buffer);
        this->x = x;
        this->y = y;
        this->dist = INT_MAX;
        this->prev = nullptr;
        this->visited = false;
}

City::~City() {
        delete[] name;
        Neighbours.delete_wVector();
}

void ParseCityName(int new_x, int new_y, int BOARD_WIDTH, char* buffer, char** Map);

void FindCityName(char** Map, char* buffer, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT);

bool isRoad(char** Map, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT);

bool isOnMap(int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT);

void FindPathBFS(City* city, wVector<City*> Graph, char** Map, bool** Visited, int** Distance, int BOARD_WIDTH, int BOARD_HEIGHT);

City* GetMinNeighbour(wVector<City*> Graph);

void Dijkstra(wVector<City*> Graph, City* start, City* destination, bool withPath);

void LoadFlights(wVector<City*> Graph, char* buffer, char* buffer2, int flight_distance);

int main() {
        int BOARD_WIDTH, BOARD_HEIGHT, k, q, flight_distance;
        bool withPath = false;
        char buffer[100];
        char buffer2[100];
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
                FindPathBFS(Graph[i], Graph, Map, Visited, Distance, BOARD_WIDTH, BOARD_HEIGHT);
                for (int y = 0; y < BOARD_HEIGHT; y++) {
                        for (int x = 0; x < BOARD_WIDTH; x++) {
                                Visited[y][x] = false;
                                Distance[y][x] = 0;
                        }
                }
        }

        cin >> k;

        for (int i = 0; i < k; i++) {
                cin >> buffer >> buffer2 >> flight_distance;
                LoadFlights(Graph, buffer, buffer2, flight_distance);
        }

        cin >> q;

        for (int i = 0; i < q; i++) {
                cin >> buffer >> buffer2 >> withPath;
                City* source = nullptr;
                City* destination = nullptr;
                for (int j = 0; j < Graph.GetSize(); j++) {
                        if (strcmp(buffer, Graph[j]->name) == 0)source = Graph[j];
                        if (strcmp(buffer2, Graph[j]->name) == 0)destination = Graph[j];
                }
                if (source != nullptr && destination != nullptr)Dijkstra(Graph, source, destination, withPath);
        }

        for (int i = 0; i < BOARD_HEIGHT; i++) {
                delete[] Distance[i];
                delete[] Map[i];
                delete[] Visited[i];
        }

        delete[] Map;
        delete[] Visited;
        delete[] Distance;
        Graph.delete_wVector();
        return 0;
}

void ParseCityName(int new_x, int new_y, int BOARD_WIDTH, char* buffer, char** Map) {
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

void FindCityName(char** Map, char* buffer, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT) {
        for (int d_y = -1; d_y < 2; d_y++) {
                for (int d_x = -1; d_x < 2; d_x++) {
                        if (x + d_x >= 0 && x + d_x < BOARD_WIDTH && y + d_y >= 0 && y + d_y < BOARD_HEIGHT
                                && Map[y + d_y][x + d_x] >= 48 && Map[y + d_y][x + d_x] <= 90)
                                ParseCityName(x + d_x, y + d_y, BOARD_WIDTH, buffer, Map);
                }
        }
}

bool isRoad(char** Map, int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT) {
        return x >= 0 && x < BOARD_WIDTH&& y >= 0 && y < BOARD_HEIGHT && (Map[y][x] == '#' || Map[y][x] == '*');
}

bool isOnMap(int x, int y, int BOARD_WIDTH, int BOARD_HEIGHT) {
        return x >= 0 && x < BOARD_WIDTH&& y >= 0 && y < BOARD_HEIGHT;
}

void FindPathBFS(City* city, wVector<City*> Graph, char** Map, bool** Visited, int** Distance, int BOARD_WIDTH, int BOARD_HEIGHT) {
        wVector<int> queue_x;
        wVector<int> queue_y;
        queue_x.push_back(city->x);
        queue_y.push_back(city->y);
        Distance[city->y][city->x] = 0;
        while (queue_x.GetSize() > 0 || queue_y.GetSize() > 0) {
                int x = queue_x.pop_first();
                int y = queue_y.pop_first();
                Visited[y][x] = true;
                if (Map[y][x] == '*' && (city->y != y || city->x != x)) {
                        if (isOnMap(x, y + 1, BOARD_WIDTH, BOARD_HEIGHT))Visited[y + 1][x] = true;
                        if (isOnMap(x, y - 1, BOARD_WIDTH, BOARD_HEIGHT))Visited[y - 1][x] = true;
                        if (isOnMap(x - 1, y, BOARD_WIDTH, BOARD_HEIGHT))Visited[y][x - 1] = true;
                        if (isOnMap(x + 1, y, BOARD_WIDTH, BOARD_HEIGHT))Visited[y][x + 1] = true;
                        for (int i = 0; i < Graph.GetSize(); i++) {
                                if (Graph[i]->x == x && Graph[i]->y == y) {
                                        NeighbourNode* temp = new NeighbourNode(city, Distance[y][x]);
                                        Graph[i]->Neighbours.push_back(temp);
                                }
                        }
                }

                int d[2] = { -1, 1 };

                for (int i = 0; i < 2; i++) {
                        if (isRoad(Map, x + d[i], y, BOARD_WIDTH, BOARD_HEIGHT) && !Visited[y][x + d[i]]) {
                                queue_x.push_back(x + d[i]);
                                queue_y.push_back(y);
                                Distance[y][x + d[i]] = Distance[y][x] + 1;
                                Visited[y][x + d[i]] = true;
                        }

                        if (isRoad(Map, x, y + d[i], BOARD_WIDTH, BOARD_HEIGHT) && !Visited[y + d[i]][x]) {
                                queue_x.push_back(x);
                                queue_y.push_back(y + d[i]);
                                Distance[y + d[i]][x] = Distance[y][x] + 1;
                                Visited[y + d[i]][x] = true;
                        }
                }
        }
        queue_x.delete_wVector();
        queue_y.delete_wVector();
}

City* GetMinNeighbour(wVector<City*> Graph) {
        City* neighbour = nullptr;
        int minimumDistance = INT_MAX;
        for (int i = 0; i < Graph.GetSize(); i++) {
                if (Graph[i]->dist < minimumDistance && Graph[i]->visited == false) {
                        neighbour = Graph[i];
                        minimumDistance = neighbour->dist;
                }
        }
        return neighbour;
}

void Dijkstra(wVector<City*> Graph, City* start, City* destination, bool withPath) {
        if (start != nullptr)start->dist = 0;
        for (;;) {
                City* temp = GetMinNeighbour(Graph);
                if (temp == nullptr) {
                        break;
                }

                temp->visited = true;

                for (int i = 0; i < temp->Neighbours.GetSize(); i++) {
                        City* toCity = temp->Neighbours[i]->toCity;
                        int distance = temp->Neighbours[i]->distance;

                        if (toCity->visited == false && temp->dist + distance < toCity->dist) {
                                toCity->dist = temp->dist + distance;
                                toCity->prev = temp;
                        }
                }
        }

        cout << destination->dist << " ";

        if (withPath == true) {
                wVector<City*> path;
                City* prev_temp = destination;
                while (prev_temp != nullptr) {
                        path.push_back(prev_temp);
                        prev_temp = prev_temp->prev;
                }
                for (int i = path.GetSize() - 2; i >= 1; i--) {
                        cout << path[i]->name << " ";
                }
                path.delete_wVector();
        }
        cout << endl;

        for (int i = 0; i < Graph.GetSize(); i++) {
                Graph[i]->dist = INT_MAX;
                Graph[i]->visited = false;
                Graph[i]->prev = nullptr;
        }
}

void LoadFlights(wVector<City*> Graph, char* buffer, char* buffer2, int flight_distance) {
        for (int i = 0; i < Graph.GetSize(); i++) {
                if (strcmp(Graph[i]->name, buffer) == 0) {
                        City* temp2 = nullptr;
                        for (int j = 0; j < Graph.GetSize(); j++) {
                                if (strcmp(Graph[j]->name, buffer2) == 0)temp2 = Graph[j];
                        }
                        NeighbourNode* temp = new NeighbourNode(temp2, flight_distance);
                        Graph[i]->Neighbours.push_back(temp);
                }
        }
}
