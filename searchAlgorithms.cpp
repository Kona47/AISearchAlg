#include "searchAlgorithms.h"

// Heuristic function return estimate of the distance from city1 to city2. Just going to use 1
double heuristic(const string& city, const string& city2) {
    return 1.0; 
}

// Breadth-First Search to find the shortest path
vector<string> bfs(const map<string, vector<string>>& adjacentList, const string& startCity, const string& goal) {
    queue<string> toExplore;
    unordered_map<string, string> cameFrom;

    toExplore.push(startCity);
    cameFrom[startCity] = "";

    while (!toExplore.empty()) {
        string current = toExplore.front();
        toExplore.pop();

        // Check if we've reached the goal
        if (current == goal) {
            vector<string> path;
    
            for (string at = goal; !at.empty(); at = cameFrom[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end()); 
            return path;
        }

        // Explore neighbors
        for (const string& neighbor : adjacentList
        .at(current)) {
            if (cameFrom.find(neighbor) == cameFrom.end()) { 
                toExplore.push(neighbor);
                cameFrom[neighbor] = current; // track the path
            }
        }
    }

    // If no path is found, return
    return {};
}

// Depth-First Search to find a path between two cities
vector<string> dfs(const map<string, vector<string>>& adjacencies, const string& startCity, const string& goal) {
    stack<string> toExplore;  // Stack for DFS
    unordered_map<string, string> previous; 
    toExplore.push(startCity);
    previous[startCity] = "";  

    while (!toExplore.empty()) {
        string current = toExplore.top();
        toExplore.pop();

        // If we reached the goal, reconstruct the path
        if (current == goal) {
            vector<string> path;
            for (string at = goal; !at.empty(); at = previous[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (const string& neighbor : adjacencies.at(current)) {
            if (previous.find(neighbor) == previous.end()) {  
                toExplore.push(neighbor);
                previous[neighbor] = current; 
            }
        }
    }

    // If no path is found, return
    return {};
}

// Iterative Deepening Depth-First Search to find a path between two cities
vector<string> iddfs(const map<string, vector<string>>& adjacencies, const string& startCity, const string& goal, int maxDepth) {
    for (int depth = 0; depth <= maxDepth; ++depth) {
        stack<pair<string, int>> toExplore;  
        unordered_map<string, string> previous; 
        toExplore.push({startCity, 0});
        previous[startCity] = ""; 

        while (!toExplore.empty()) {
            pair<string, int> topElement = toExplore.top(); 
            string currentCity = topElement.first; 
            int currentDepth = topElement.second;
            toExplore.pop();
            // If we reached the goal
            if (currentCity == goal) {
                vector<string> path;
                for (string at = goal; !at.empty(); at = previous[at]) {
                    path.push_back(at);
                }
                reverse(path.begin(), path.end());
                return path;
            }
            //Explore neighbors if currentDepth is less than maxDepth
            if (currentDepth < depth) {
                for (const string& neighbor : adjacencies.at(currentCity)) {
                    if (previous.find(neighbor) == previous.end()) { 
                        toExplore.push({neighbor, currentDepth + 1});
                        previous[neighbor] = currentCity; 
                    }
                }
            }
        }
    }

    // If no path, return
    return {};
}

vector<string> bestFirstSearch(const map<string, vector<string>>& adjacentList, const string& startCity, const string& goal) {
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> toExplore;
    unordered_map<string, string> cameFrom;
    unordered_map<string, double> gScore;
    gScore[startCity] = 0;

    toExplore.push({heuristic(startCity, goal), startCity});
    cameFrom[startCity] = "";

    while (!toExplore.empty()) {
        string current = toExplore.top().second;
        toExplore.pop();

        // Check if we've reached the goal
        if (current == goal) {
            vector<string> path;
            // Reconstruct the path
            for (string at = goal; !at.empty(); at = cameFrom[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (const string& neighbor : adjacentList
        .at(current)) {
            double tentative_gScore = gScore[current] + 1; // Assume cost to reach a neighbor is 1
            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                double fScore = gScore[neighbor] + heuristic(neighbor, goal);
                toExplore.push({fScore, neighbor});
            }
        }
    }

    // If no path is found, return
    return {};
}

// A* Search to find a path between two cities
vector<string> aStar(const map<string, vector<string>>& adjacentList, const string& startCity, const string& goal) {
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> toExplore;
    unordered_map<string, string> cameFrom;
    unordered_map<string, double> gScore; 
    gScore[startCity] = 0;

    toExplore.push({heuristic(startCity, goal), startCity});
    cameFrom[startCity] = ""; 

    while (!toExplore.empty()) {
        string current = toExplore.top().second;
        toExplore.pop();

        // Check if we've reached the goal
        if (current == goal) {
            vector<string> path;
            
            for (string at = goal; !at.empty(); at = cameFrom[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (const string& neighbor : adjacentList
        .at(current)) {
            double tentative_gScore = gScore[current] + 1; // Assume cost to reach a neighbor is 1
            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                double fScore = gScore[neighbor] + heuristic(neighbor, goal);
                toExplore.push({fScore, neighbor});
            }
        }
    }

    // If no path is found, return
    return {};
}
