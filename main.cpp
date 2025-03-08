#include "searchAlgorithms.h"

using namespace std;



double earthRadius = 6371.0;
// Function to add an adjacency between two cities
void addAdjacency(map<string, vector<string>>& adjacencies, const string& city1, const string& city2) {
    adjacencies[city1].push_back(city2);
    adjacencies[city2].push_back(city1); 
}

// Function to parse adjacencies from a file
map<string, vector<string>> parseAdjacenciesFromFile(const string& filename) {
    ifstream file(filename);
    map<string, vector<string>> adjacencies;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return adjacencies;
    }

    string line, city1, city2;

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> city1 >> city2;
        addAdjacency(adjacencies, city1, city2);
    }

    file.close();
    return adjacencies;
}
// Function to read city coordinates from a CSV file
map<string, pair<double, double>> readCoordinatesFromCSV(const string& filename) {
    ifstream file(filename);
    map<string, pair<double, double>> coordinates;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return coordinates;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string city;
        double lat, lon;
        getline(ss, city, ',');
        ss >> lat;
        ss.ignore(1); // ignore the comma
        ss >> lon;
        coordinates[city] = {lat, lon};
    }

    file.close();
    return coordinates;
}

//The haversine formula, calculates distance between 2 latitude and longitude locations, From ChatGPT.
double haversine(const pair<double, double>& coord1, const pair<double, double>& coord2) {
    double latitude1 = coord1.first * M_PI / 180.0;
    double longitude1 = coord1.second * M_PI / 180.0;
    double latitude2 = coord2.first * M_PI / 180.0;
    double longitude2 = coord2.second * M_PI / 180.0;

    double dlat = latitude2 - latitude1;
    double dlon = longitude2 - longitude1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(latitude1) * cos(latitude2) * 
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earthRadius * c;
}

// Function to calculate total distance for a given path
double calculateTotalDistance(const vector<string>& path, const map<string, pair<double, double>>& coordinates) {
    double totalDistance = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const string& city1 = path[i];
        const string& city2 = path[i + 1];

        if (coordinates.find(city1) != coordinates.end() && coordinates.find(city2) != coordinates.end()) {
            totalDistance += haversine(coordinates.at(city1), coordinates.at(city2));
        } else {
            cerr << "Coordinates not found for city: " << city1 << " or " << city2 << endl;
        }
    }
    return totalDistance;
}

// Function to display the path and its total distance
void displayPathAndDistance(const vector<string>& path, const map<string, pair<double, double>>& coordinates) {
    if (path.empty()) {
        cout << "\nNo path found!" << endl;
        return;
    }
    else{
      cout << "\nBest Path: ";
      for (const string& city : path) {
          cout << city << " -> ";
      }
      cout << "Done!\n";

      double totalDistance = calculateTotalDistance(path, coordinates);
      cout << "\nTotal distance: " << totalDistance << " kilometers" << endl;
    }
}

template<typename Func>
auto measureSearchTime(Func searchFunction, const map<string, vector<string>>& adjacencies, const string& startCity, const string& endCity) {
    auto start = chrono::high_resolution_clock::now();
    auto path = searchFunction(adjacencies, startCity, endCity);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    return make_pair(path, elapsed.count()); // Return both the path and elapsed time
}
// Function to get the estimated memory used
size_t memoryUsageOfVector(const vector<string>& vec) {
    size_t totalMemory = 0;
    for (const string& s : vec) {
        totalMemory += sizeof(s) + s.capacity();  // memory for string object and its allocated buffer
    }
    totalMemory += sizeof(vec);  // memory for the vector object itself
    return totalMemory;
}

// Function to estimate memory used by an adjacency list
size_t memoryUsageOfadjacencies(const map<string, vector<string>>& adjacencies) {
    size_t totalMemory = 0;
    for (const auto& pair : adjacencies) {
        totalMemory += sizeof(pair.first) + pair.first.capacity();  // memory for the city string
        totalMemory += memoryUsageOfVector(pair.second);  // memory for the vector of adjacent cities
    }
    totalMemory += sizeof(adjacencies);  // memory for the map itself
    return totalMemory;
}

// Function to estimate memory used by the coordinates map
size_t memoryUsageOfCoordinates(const map<string, pair<double, double>>& coordinates) {
    size_t totalMemory = 0;
    for (const auto& pair : coordinates) {
        totalMemory += sizeof(pair.first) + pair.first.capacity();  // memory for the city string
        totalMemory += sizeof(pair.second);  // memory for the coordinate pair
    }
    totalMemory += sizeof(coordinates);  // memory for the map itself
    return totalMemory;
}



int main() {
    //file names
    string adjFilename = "adjacencies.txt"; 
    string coordFilename = "coordinates.csv"; 

    map<string, vector<string>> adjacencies = parseAdjacenciesFromFile(adjFilename);
    map<string, pair<double, double>> coordinates = readCoordinatesFromCSV(coordFilename);

    // Input cities to search the path between
    string startCity, endCity;
    cout << "Start city: ";
    cin >> startCity;
    cout << "End City: ";
    cin >> endCity;
    char exit = 'Y';
    while (exit == 'Y' || exit == 'y'){
        // Choose the search method: BFS, DFS, ID-DFS, Best-First Search, or A*
        int choice;
        cout << "Enter number for desired method:\n\t1 : BFS \n\t2 : DFS \n\t3 : IDDFS \n\t4 : Best-First Search \n\t5 : A*\nNumber: ";
        cin >> choice;

        vector<string> path;
        double elapsedTime = 0.0;

        if (choice == 1) {
            // Perform BFS
            auto result = measureSearchTime(bfs, adjacencies, startCity, endCity);
            path = result.first;
            elapsedTime = result.second;
        }
        else if (choice == 2) {
            // Perform DFS
            auto result = measureSearchTime(dfs, adjacencies, startCity, endCity);
            path = result.first;
            elapsedTime = result.second;
        } 
        else if (choice == 3) {
            // Perform ID-DFS
            int maxDepth;
            cout << "Enter maximum depth for ID-DFS: ";
            cin >> maxDepth;
            auto result = measureSearchTime([&](const auto& adj, const auto& start, const auto& goal) {
                return iddfs(adj, start, goal, maxDepth);
            }, adjacencies, startCity, endCity);
            path = result.first;
            elapsedTime = result.second;
        } 
        else if (choice == 4) {
            // Perform Best-First Search
            auto result = measureSearchTime(bestFirstSearch, adjacencies, startCity, endCity);
            path = result.first;
            elapsedTime = result.second;
        } 
        else if (choice == 5) {
            // Perform A* Search
            auto result = measureSearchTime(aStar, adjacencies, startCity, endCity);
            path = result.first;
            elapsedTime = result.second;
        } 
        else {
            cout << "Invalid choice!" << endl;
            return 1;
        }

        displayPathAndDistance(path, coordinates);

        //Memory
        size_t adjacenciesMemory = memoryUsageOfadjacencies(adjacencies);
        size_t coordMemory = memoryUsageOfCoordinates(coordinates);

        cout << "\nMemory used by adjacency list: " << adjacenciesMemory << " bytes\n";
        cout << "\nMemory used by coordinates map: " << coordMemory << " bytes\n";
        cout << "\nTime: " << elapsedTime << " seconds.\n";
        cout << "\nSearch another route? ('Y' or 'N'): ";
        cin >> exit;
        cout << endl << endl << endl;
    }

    return 0;
}