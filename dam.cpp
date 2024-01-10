#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <map>
#include <thread>
#include <cstdlib>
#include <set>
#include <fstream>
#include <chrono>   
#include <atomic>

using namespace std;
using namespace chrono;

/*----------------------------------------------------------CONCURRENT QUEUE----------------------------------------------------------------------------------------------------------------------------------*/

class Concurrent_PQ
{
public:
    mutex mtx;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    void Clear()
    {
        while (!pq.empty())
        {
            pq.pop();
        }
    }
    int getSize()
    {
        mtx.lock();
        int ret = pq.size();
        mtx.unlock();
        return ret;
    }
};

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
map<int, int> nodes;

// Adjacency list
vector<vector<int>> neig(1e6 + 10, vector<int>(0));

// Final result of K-NN
// Clear these after you are done with one insertion
Concurrent_PQ pq;
atomic<int> visited[100000];
// mutex locks[100000];

double total_time_insertion = 0;

atomic_flag locking = ATOMIC_FLAG_INIT;
int n;

/*----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------------------------------------------------------------------------------*/

int distance(int a, int b)
{
    return a > b ? a - b : b - a;
}

bool check(int n)
{

    if (n <= 1)
        return false;

    for (int i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return false;

    return true;
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

class Counter
{
private:
    int value;

public:
    Counter(int i)
    {
        value = i;
    }
    int getAndIncrement()
    {

        while (locking.test_and_set())
            ;

        int curr_val = value;
        value++;

        locking.clear();

        return curr_val;
    }
};

/*----------------------------------------------------------THREADING FUNCTION----------------------------------------------------------------------------------------------------------------------------------*/

void eachEntry(int query_value, int K, vector<int> arr, Counter &counter)
{

    int var;

    while (true)
    {

        var = counter.getAndIncrement();

        if (var >= n)
            break;

        int entryIndex = arr[var];

        // Priority Queue(Min-Heap) of potential neigbors
        // a.first -> distance from query
        // a.second -> unique id of the node
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> candidates;
        // Temp array used for every iteration in finding K-NN for finding Nearest Neighbour
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> temp_result;
        // cout << "In thread number: "<<threadID<<", entry index value is: "<<entryIndex<<endl;

        int expected = 0;
        bool checker = visited[entryIndex].compare_exchange_strong(expected, 1);

        if (checker)
        {
            candidates.push(make_pair(distance(query_value, nodes[entryIndex]), entryIndex));
            temp_result.push(make_pair(distance(query_value, nodes[entryIndex]), entryIndex));
        }

        while (!candidates.empty())
        {
            int temp_dist;
            int id;

            pair<int, int> tmp = candidates.top();
            candidates.pop();

            temp_dist = tmp.first;
            id = tmp.second;
            int sz = pq.getSize();

            if (sz >= K)
            {
                pq.mtx.lock();
                std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> temp = pq.pq;
                pq.mtx.unlock();
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
                  int expected2 = 0;
                  bool checker2 = visited[i].compare_exchange_strong(expected2, 1);
            
                  if (checker2)
                  {
                    int friend_dist = distance(nodes[i], query_value);
                    temp_result.push(make_pair(friend_dist, i));
                    candidates.push(make_pair(friend_dist, i));
                  }
                }
        }

        pq.mtx.lock();
        while (!temp_result.empty())
        {
            pq.pq.push(temp_result.top());
            temp_result.pop();
        }
        pq.mtx.unlock();
    }
}

/*----------------------------------------------------------SEARCH FUNCTION----------------------------------------------------------------------------------------------------------------------------------*/

set<int> KNN_Search(int query_value, int entries, int K)
{

    set<int> ret;
    if (nodes.size() == 1)
    {
        return ret;
    }

    memset(visited, 0, sizeof(visited));

    // memset(visited, 0 , sizeof(visited));

    while (!pq.pq.empty())
    {
        pq.pq.pop();
    }

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, nodes.size() - 1);

    vector<int> arr(entries, 0);

    for (int i = 0; i < entries; i++)
    {
        int entryIndex = distrib(gen);
        arr[i] = entryIndex;
    }

    int no_of_threads = 8 ;

    Counter counter(0);
    n = entries;

    thread threads[no_of_threads];
    for (int i = 0; i < no_of_threads; i++)
    {
        threads[i] = thread(eachEntry, query_value, K, arr, ref(counter));
    }

    for (int i = 0; i < no_of_threads; ++i)
    {
        threads[i].join();
    }

    int max_possible_size = min(K, (int)pq.pq.size());

    for (int i = 0; i < max_possible_size; i++)
    {
        if (pq.pq.size() == 0)
            break;
        pair<int, int> a = pq.pq.top();

        pq.pq.pop();
        ret.insert(a.second);
    }

    return ret;
}

void build_nsw(pair<int, int> new_node)
{

    Timer timer;

    timer.begin();
    int new_id = new_node.first;
    int new_value = new_node.second;
    set<int> knn_neig = KNN_Search(new_value, 32, 5);

    for (auto i : knn_neig)
    {
        neig[i].push_back(new_id);
        neig[new_id].push_back(i);
    }
    timer.end();

    pq.Clear();

    total_time_insertion += timer.gettime();
    // cout << "The total time required for Insertion of "<<new_value<<" is: "<<timer.gettime()<<endl;
}

int main(int argc, char *argv[])
{
// #ifndef ONLINE_JUDGE
//     freopen("out_parallel.txt", "a", stdout);
// #endif

string str(argv[1]);
str = "./Dataset/"+str+".txt" ;
ifstream ip(str);
int number;
while (ip >> number)
{
    // cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    // cout << "Read number: " << number << endl;
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
    //     int number;
    //     cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    //     cin >> number;
    //     if (number == -1)
    //         break;
    //     Timer timer;
    //     timer.begin();
    //     set<int> knn_neig = KNN_Search(number, 64, 6);
    //     timer.end();
    //     cout << "The Nearest Neighbhours for given query point are: ";
    //     for (auto i : knn_neig)
    //     {
    //         cout << nodes[i] << " ";
    //     }
    //     cout << endl;
    //     cout << "The total time required for KNN Search of " << number << " is: " << timer.gettime() << endl;
    // }

    return 0;
}
