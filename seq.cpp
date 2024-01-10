#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <random>
#include <map>
#include <cstdlib>
#include <set>
#include <chrono>

using namespace std;
using namespace chrono;

/*----------------------------------------------------------TIMER CLASS----------------------------------------------------------------------------------------------------------------------------------*/

class Timer
{
public:
  high_resolution_clock::time_point start;
  high_resolution_clock::time_point stop;
  double total_time = 0;
  void begin()
  {
    start = high_resolution_clock::now();
  }
  void end()
  {
    stop = high_resolution_clock::now();
    auto time = duration_cast<microseconds>(stop - start);
    total_time = (double)(time.count());
  }
  double gettime()
  {
    return total_time;
  }
};

/*----------------------------------------------------------GLOBAL VARIABLES----------------------------------------------------------------------------------------------------------------------------------*/

// vector which stores node unique id and its value
// unqiue id, value
map<int, int> nodes;

// Adjacency list
vector<vector<int>> neig(1e6 + 10, vector<int>(0));

double total_time_insertion = 0;
/*----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------------------------------------------------------------------------------*/

int distance(int a, int b)
{
  return a > b ? a - b : b - a;
}

void printing(map<int, int> nodes, vector<vector<int>> neig)
{
  for (auto i : nodes)
  {
    cout << i.second << " : ";
    for (auto j : neig[i.first])
    {
      cout << nodes[j] << " ";
    }
    cout << endl;
  }
}

/*----------------------------------------------------------SEARCH FUNCTION----------------------------------------------------------------------------------------------------------------------------------*/

// Function which returns K nearest neighbors for given node in the current graph
set<int> KNN_Search(int query_value, int entries, int K)
{

  if (nodes.size() == 1)
  {
    set<int> ret;
    return ret;
  }

  // Final result of K-NN
  // a.first -> distance from query
  // a.second -> unique id of the node
  int *visited = (int *)calloc(1000010, sizeof(int));
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> result;
  set<int> ret;

  // Random number generator setup
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, nodes.size() - 1);

  map<int, int> entry_indexes;

  // Used for choosing "entries" number of entry points
  for (int i = 0; i < entries; i++)
  {
    // Priority Queue(Min-Heap) of potential neigbors
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> candidates;
    // Temp array used for every iteration in finding K-NN for finding Nearest Neighbour
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> temp_result;

    int entryIndex = distrib(gen);

    if (visited[entryIndex] == 0)
    {
      temp_result.push(make_pair(distance(query_value, nodes[entryIndex]), entryIndex));
      candidates.push(make_pair(distance(query_value, nodes[entryIndex]), entryIndex));
      visited[entryIndex] = 1;
    }

    while (!candidates.empty())
    {
      int temp_dist;
      int id;

      pair<int, int> tmp = candidates.top();
      candidates.pop();

      temp_dist = tmp.first;
      id = tmp.second;

      if (result.size() >= K)
      {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> temp = result;
        int a;
        int tmp2 = K;
        while (tmp2--)
        {
          a = temp.top().first;
          temp.pop();
        }
        // Breaking the while loop for traversing of potential neighbhours as reached a sitation where from now on the elements
        // in the Priority Queue are farther than the Kth element in the till formed in the result set
        if (temp_dist > a)
        {
          break;
        }
      }

      for (auto i : neig[id])
      {
        if (visited[i] == 0)
        {
          visited[i] = 1;
          int friend_dist = distance(nodes[i], query_value);
          temp_result.push(make_pair(friend_dist, i));
          candidates.push(make_pair(friend_dist, i));
        }
      }
    }

    while (!temp_result.empty())
    {
      result.push(temp_result.top());
      temp_result.pop();
    }
  }

  int max_possible_size = min(K, (int)result.size());

  for (int i = 0; i < max_possible_size; i++)
  {
    if (result.size() == 0)
      break;
    pair<int, int> a = result.top();
    result.pop();
    ret.insert(a.second);
  }

  free(visited);
  return ret;
}

void build_nsw(pair<int, int> new_node)
{

  Timer timer;
  timer.begin();
  int new_id = new_node.first;
  int new_value = new_node.second;

  set<int> knn_neig = KNN_Search(new_value, 32, 6);

  for (auto i : knn_neig)
  {
    neig[i].push_back(new_id);
    neig[new_id].push_back(i);
  }
  timer.end();

  total_time_insertion += timer.gettime();
  // cout << "The total time required for Insertion of " << new_value << " is: " << timer.gettime() << endl;
}

int main(int argc, char *argv[])
{

// #ifndef ONLINE_JUDGE
//   freopen("out_sequential.txt", "a", stdout);
// #endif

  string str(argv[1]);
  str = "./Dataset/"+str+".txt" ;
  // cout << "hi" << endl ;
  ifstream ip(str);
  int number;
  while (ip >> number)
  {
    // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    // cout << "Number Read: " << number << endl;

    if (number == -1)
    {
      break;
    }
    int cnt = nodes.size();
    cnt++;
    nodes[cnt] = number;

    build_nsw(make_pair(nodes.size(), number));

    // printing(nodes, neig);
  }
  // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  // cout << "The Total time taken for all the insertions is: "
  cout << (total_time_insertion*0.000001) << endl;
  
  // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;

  // while (true)
  // {
  //   int number;
  //   cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  //   cin >> number;
  //   if (number == -1)
  //     break;
  //   Timer timer;
  //   timer.begin();
  //   set<int> knn_neig = KNN_Search(number, 64, 6);
  //   timer.end();
  //   cout << "The Nearest Neighbhours for given query point are: ";
  //   for (auto i : knn_neig)
  //   {
  //     cout << nodes[i] << " ";
  //   }
  //   cout << endl;
  //   cout << "The total time required for KNN Search of " << number << " is: " << timer.gettime() << endl;
  // }

  return 0;
}
