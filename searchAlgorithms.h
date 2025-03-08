#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <map> 
#include <vector>
#include <string>
#include <queue> 
#include <stack>
#include <unordered_map> 
#include <algorithm> 
#include <limits> 
#include <cmath> 
#include <chrono> 

using namespace std;

vector<string> bfs(const map<string, vector<string>>& adjacencies, const string& start, const string& goal);

vector<string> dfs(const map<string, vector<string>>& adjacencies, const string& start, const string& goal);

vector<string> iddfs(const map<string, vector<string>>& adjacencies, const string& start, const string& goal, int maxDepth);

vector<string> bestFirstSearch(const map<string, vector<string>>& adjacencies, const string& start, const string& goal);

vector<string> aStar(const map<string, vector<string>>& adjacencies, const string& start, const string& goal);