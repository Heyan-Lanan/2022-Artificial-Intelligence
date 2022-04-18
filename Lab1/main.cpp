#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_set>
#include <set>
#include <chrono>
#include <unordered_map>

using namespace std;

#define DIM 5
#define ZERO 26
#define MINUS (-1)


struct Node {
    char *state{}; //[DIM*DIM+1]
    std::pair<int, char> from_parent_movement;
    int depth{};
    int h{};
    struct Node *parent{};
};

void printMovement(Node *n) {
    if (n->parent != nullptr) {
        printMovement(n->parent);
        std::cout << "(" << n->from_parent_movement.first << ", " << n->from_parent_movement.second << ")" << std::endl;
    }
}

char *Exchange_state(const char *origin_state, int zero_location, int exchange_location){
    char *next_state = new char[DIM*DIM+1];
    for(auto i = 0; i < DIM*DIM; i++)
        next_state[i] = origin_state[i];
    next_state[DIM*DIM] = '\0';
    next_state[zero_location] = origin_state[exchange_location];
    next_state[exchange_location] = origin_state[zero_location];
    return next_state;
}


class StateManager {
    public:
        char *dest_state;
        explicit StateManager(const char *dest_state);
        static std::vector<std::pair<std::pair<int, char>, char *> > getSucceedStates(const char *state);
        int hFunction(const char *state) const;
        int hFunction2(const char *state) const;

    private:
};

class AStar {
    public:
        AStar(const char *init_state, const char *dest_state, string h_func);
        ~AStar();
        Node *graphSearch();

    private:
        std::priority_queue<std::pair<std::pair<int ,int>, Node* > ,std::vector<std::pair<std::pair<int ,int>, Node* > >, std::greater<> > fringe;
        std::vector<Node *> nodes;
        char init_state[DIM*DIM+1]{};
        char dest_state[DIM*DIM+1]{};
        static bool check(const char* p1, const char *p2);
        StateManager *sm;
        std::unordered_set<std::string> close_set;
        bool insertIntoCloseSet(const char *state);
        string h_func;
};

struct Info{
    bool in_stack;
    Node *n_his;
};

class IDAStar {

public:
    IDAStar(const char *init_state, const char *dest_state);
    ~IDAStar();
    void graphSearch();
    int cnt_node;
private:
    std::vector<Node *> fringe;
    // std::priority_queue<std::pair<std::pair<int ,int>, Node* > ,std::vector<std::pair<std::pair<int ,int>, Node* > >, std::greater<> > fringe;
    std::vector<Node *> nodes;
    char init_state[DIM*DIM+1];
    char dest_state[DIM*DIM+1];
    bool check(const char* p1, const char *p2);
    int DFS(Node *n, int threshold);
    StateManager *sm;
    const int INF = 100000;
    std::unordered_map<std::string, Info> visited;
};

IDAStar::~IDAStar() {
    for (auto & n : this->nodes) {
        delete[] n->state;
        delete n;
    }
    delete this->sm;
}

IDAStar::IDAStar(const char *init_state, const char *dest_state) {

    this->sm = new StateManager(dest_state);

    this->cnt_node = 0;
    Node *n = new Node;
    n->state = new char[DIM*DIM+1];
    for (int i = 0; i < DIM*DIM+1; ++i) {
        n->state[i] = init_state[i];
        this->init_state[i] = init_state[i];
        this->dest_state[i] = dest_state[i];
    }
    n->from_parent_movement.first = 0;
    n->from_parent_movement.second = '\0';
    n->depth = 0;
    n->h = this->sm->hFunction2(n->state);//TODO
    n->parent = nullptr;
    this->nodes.push_back(n);

}

void IDAStar::graphSearch() {
    Node *n = this->nodes[0];
    int threshold = this->sm->hFunction2(n->state);//TODO

    this->visited.emplace(n->state, Info{true, n});
    while (true) {
        int tmp = DFS(n, threshold);
        std::cout << "current nodes: " << this->cnt_node << std::endl;
        if (tmp == -1) {
            return;
        }
        if (tmp == this->INF) {
            return;
        }
        threshold = tmp;
    }
}

bool IDAStar::check(const char *p1, const char *p2) {
    for (int i = 0; i < DIM*DIM+1; ++i) {
        if (p1[i] != p2[i]) {
            return false;
        }
    }
    return true;
}

int IDAStar::DFS(Node *n, int threshold) {
    cnt_node++;
    int f = n->depth + n->h;
    if (f > threshold) {
        return f;
    }
    if (this->check(n->state, this->dest_state)) {
        printMovement(n);
        return -1;
    }
    int min_val = this->INF;
    auto succeeds = StateManager::getSucceedStates(n->state);
    for (auto &s : succeeds) {

        auto n_find = this->visited.find(s.second);
        if (n_find == this->visited.end()) {
            // not found in visited
            Node *n_tmp = new Node;
            n_tmp->state = s.second;
            n_tmp->from_parent_movement = s.first;
            n_tmp->depth = n->depth + 1;
            n_tmp->parent = n;
            n_tmp->h = this->sm->hFunction2(n_tmp->state);//TODO
            this->nodes.push_back(n_tmp);

            this->visited.emplace(n_tmp->state, Info{true, n_tmp});

            int tmp = DFS(n_tmp, threshold);
            this->visited[n_tmp->state].in_stack = false;
            if (tmp == -1) {
                return -1;
            }
            if (tmp < min_val) {
                min_val = tmp;
            }

        } else {
            if (!n_find->second.in_stack) {
                if (n->depth+1 <= n_find->second.n_his->depth) {
                    // not in stack and current depth is lower than old depth
                    Node *n_tmp = n_find->second.n_his; // update info
                    n_tmp->depth = n->depth + 1;
                    n_tmp->parent = n;
                    n_tmp->from_parent_movement = s.first;
                    n_find->second.in_stack = true; // in stack
                    int tmp = DFS(n_tmp, threshold);
                    n_find->second.in_stack = false;

                    if (tmp == -1) {
                        return -1;
                    }
                    if (tmp < min_val) {
                        min_val = tmp;
                    }
                } else {
                    delete[] s.second;
                }
            } else {
                delete[] s.second;
            }
        }
    }
    return min_val;
}



AStar::~AStar() {
    for (auto & n : this->nodes) {
        delete[] n->state;
        delete n;
    }
    delete this->sm;
}

AStar::AStar(const char *init_state, const char *dest_state, string h_func) {
    this->h_func = std::move(h_func);
    Node *n = new Node;
    n->state = new char[DIM*DIM+1];
    for (int i = 0; i < DIM*DIM+1; ++i) {
        n->state[i] = init_state[i];
        this->init_state[i] = init_state[i];
        this->dest_state[i] = dest_state[i];
    }
    n->from_parent_movement.first = 0;
    n->from_parent_movement.second = '\0';
    n->depth = 0;
    n->parent = nullptr;
    this->insertIntoCloseSet(n->state);
    this->nodes.push_back(n);
    this->sm = new StateManager(dest_state);
}

bool AStar::insertIntoCloseSet(const char *state) {
    std::string s(state);
    if (this->close_set.find(s) == this->close_set.end()) {
        this->close_set.insert(s);
        return true;
    }
    return false;
}

Node *AStar::graphSearch() {
    // return the final node.
    Node *n = this->nodes[0];
    int h;
    if(this->h_func == "h1")
        h = this->sm->hFunction(n->state);
    else
        h = this->sm->hFunction2(n->state);
    int g = n->depth;
    int f = g + h;
    this->fringe.push(std::make_pair(std::make_pair(f, g), n));

    while (!fringe.empty()) {
        auto t = fringe.top();
        fringe.pop();
        f = t.first.first;
        g = t.first.second;
        n = t.second;
        if (AStar::check(n->state, this->dest_state)) {
            cout << this->fringe.size();
            return n;
        }

        auto succeeds = StateManager::getSucceedStates(n->state);
        for (auto &s : succeeds) {
            if(this->insertIntoCloseSet(s.second)){
                Node *n_tmp = new Node;
                n_tmp->state = s.second;
                n_tmp->from_parent_movement = s.first;
                n_tmp->depth = n->depth + 1;
                n_tmp->parent = n;
                int n_tmp_g = n_tmp->depth;
                int n_tmp_h;
                if(this->h_func == "h1")
                    n_tmp_h = this->sm->hFunction(n_tmp->state);
                else
                    n_tmp_h = this->sm->hFunction2(n_tmp->state);
                int n_tmp_f = n_tmp_g + n_tmp_h;
                fringe.push(std::make_pair(std::make_pair(n_tmp_f, n_tmp_g), n_tmp));
                this->nodes.push_back(n_tmp);
            }
        }
    }
    return nullptr;
}

bool AStar::check(const char* p1, const char *p2) {
    for (int i = 0; i < DIM*DIM; ++i) {
        if (p1[i] != p2[i]) {
            return false;
        }
    }
    return true;
}

StateManager::StateManager(const char *dest_state) {
    this->dest_state = new char[DIM*DIM+1];
    for(auto i = 0; i < DIM*DIM; i++)
        this->dest_state[i] = dest_state[i];
    this->dest_state[DIM*DIM] = '\0';
}

std::vector<std::pair<std::pair<int, char>, char *>> StateManager::getSucceedStates(const char *state) {
    std::vector<std::pair<std::pair<int, char>, char *>> result;
    int zero_location = 0;
    for(auto i = 0; i < DIM*DIM; i++){
        if(state[i] == ZERO){
            zero_location = i;
            break;
            //printf("OK!");
        }

        //printf("%d ", state[i]);
    }
    //printf("\n");
    if((zero_location - 1 >= 0) && (zero_location - 1 <= 24) && (state[zero_location - 1] != MINUS))
        result.emplace_back(make_pair(zero_location, 'l'), Exchange_state(state, zero_location, zero_location-1));
    if((zero_location + 1 >= 0) && (zero_location + 1 <= 24) && (state[zero_location + 1] != MINUS))
        result.emplace_back(make_pair(zero_location, 'r'), Exchange_state(state, zero_location, zero_location+1));
    if((zero_location - DIM >= 0) && (zero_location - DIM <= 24) && (state[zero_location - DIM] != MINUS))
        result.emplace_back(make_pair(zero_location, 'u'), Exchange_state(state, zero_location, zero_location-DIM));
    if((zero_location + DIM >= 0) && (zero_location + DIM <= 24) && (state[zero_location + DIM] != MINUS))
        result.emplace_back(make_pair(zero_location, 'd'), Exchange_state(state, zero_location, zero_location+DIM));


    if(zero_location == 2 && (state[zero_location+4*DIM] != MINUS)){
        result.emplace_back(make_pair(zero_location, 'u'), Exchange_state(state, zero_location, zero_location+4*DIM));
    }
    else if(zero_location == 10 && (state[zero_location+DIM-1] != MINUS)){
        result.emplace_back(make_pair(zero_location, 'l'), Exchange_state(state, zero_location, zero_location+DIM-1));
    }
    else if(zero_location == 14 && (state[zero_location-DIM+1] != MINUS)){
        result.emplace_back(make_pair(zero_location, 'r'), Exchange_state(state, zero_location, zero_location-DIM+1));
    }
    else if(zero_location == 22 && (state[zero_location-4*DIM] != MINUS)){
        result.emplace_back(make_pair(zero_location, 'd'), Exchange_state(state, zero_location, zero_location-4*DIM));
    }



    return result;
}

int StateManager::hFunction(const char *state) const {
    int mis_count = 0;
    for(int i = 0; i < DIM * DIM; i++){
        if(this->dest_state[i] != state[i])
            mis_count++;
    }
    return mis_count;
}

int StateManager::hFunction2(const char *state) const {
    int chebyshev_count = 0;
    for(int i = 0; i < DIM * DIM; i++){
        for(int j = 0; j < DIM * DIM; j++){
            if(state[i] == this->dest_state[j]){
                if(i == j)
                    break;
                else{
                    int xi = i % DIM, xj = j % DIM, yi = i / DIM, yj = j / DIM;
                    int x_distance = abs(xi - xj), y_distance = abs(yi - yj);
                    if(x_distance >= 3)
                        chebyshev_count += (5 - x_distance + y_distance);
                    else if(y_distance >= 3)
                        chebyshev_count += (5 - y_distance + x_distance);
                    else
                        chebyshev_count += (y_distance + x_distance);
                    break;
                }
            }
        }
    }
    return chebyshev_count;
}
//chebyshev_count += max(abs(xi - xj), abs(yi - yj));
int main(int argc, char *argv[]) {
    //if (argc == 4) {
        std::ifstream initFile, destFile;
        initFile.open("C://AI_Lab1//data//input11.txt");
        destFile.open("C://AI_Lab1//data//target11.txt");
        char *init_state = new char[DIM*DIM+1];
        char *dest_state = new char[DIM*DIM+1];
        std::string buffer;

        for (int i = 0; i < DIM * DIM; ++i) {
            initFile >> buffer;
            char num = (char )std::stoi(buffer);
            if (num == 0) {
                init_state[i] = ZERO;
            }
            else if(num < 0){
                init_state[i] = MINUS;
            }
            else {
                init_state[i] = num;
            }
        }
        for (int i = 0; i < DIM * DIM; ++i) {
            destFile >> buffer;
            char num = (char )std::stoi(buffer);
            if (num == 0) {
                dest_state[i] = ZERO;
            }
            else if(num < 0){
                dest_state[i] = MINUS;
            }
            else {
                dest_state[i] = num;
            }
        }
        init_state[DIM*DIM] = '\0';
        dest_state[DIM*DIM] = '\0';
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        //if (argv[3][0] == 'A') {
            string h_func = "h2";
            AStar a_star(init_state, dest_state, h_func);
            Node *n = a_star.graphSearch();
            printMovement(n);

        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        IDAStar ida_star(init_state, dest_state);
        ida_star.graphSearch();

        //} else {
            //IDAStar ida_star(init_state, dest_state);
            //ida_star.graphSearch();
        //}
        chrono::steady_clock::time_point t3 = chrono::steady_clock::now();
    cout << "AStar Search Timing: " << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << "s" << endl;
    cout << "IDAStar Search Timing: " << chrono::duration_cast<chrono::duration<double>>(t3 - t2).count() << "s";
    //}
    return 0;
}