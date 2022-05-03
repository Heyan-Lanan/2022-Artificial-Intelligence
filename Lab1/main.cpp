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
    char *state{};
    std::pair<int, char> from_parent_movement;
    int depth{};
    int h{};
    struct Node *parent{};
};

void print_movement(Node *n, ofstream &resultFile) {
    if (n->parent != nullptr) {
        print_movement(n->parent, resultFile);
        resultFile << n->from_parent_movement.second;
    }
}

char *exchange_state(const char *origin_state, int zero_loc, int exchange_location) {
    char *next_state = new char[DIM * DIM + 1];
    for (auto i = 0; i < DIM * DIM; i++)
        next_state[i] = origin_state[i];
    next_state[DIM * DIM] = '\0';
    next_state[zero_loc] = origin_state[exchange_location];
    next_state[exchange_location] = origin_state[zero_loc];
    return next_state;
}


class state_manage {
public:
    char *dest_state;
    explicit state_manage(const char *dest_state);
    static vector<pair<pair<int, char>, char *> > get_succeed_states(const char *state);
    int h_function(const char *state) const;
    int h_function2(const char *state) const;
private:
};

class AStar {
public:
    AStar(const char *init_state, const char *dest_state, string h_func);
    ~AStar();
    Node *graph_search();

private:
    priority_queue<pair<pair<int, int>, Node *>, vector<pair<pair<int, int>, Node *> >, greater<> > fringe;
    std::vector<Node *> nodes;
    char init_state[DIM * DIM + 1]{};
    char dest_state[DIM * DIM + 1]{};
    static bool check(const char *p1, const char *p2);
    state_manage *state_manager;
    std::unordered_set<std::string> close_set;
    bool insert_closeset(const char *state);
    string h_func;
};

struct Info {
    bool in_stack;
    Node *n_his;
};

class IDAStar {

public:
    IDAStar(const char *init_state, const char *dest_state, string h_func, ofstream &resultFile);
    ~IDAStar();
    void graph_search();
    int cnt_node;
    ofstream resultFile;
private:
    std::vector<Node *> fringe;
    std::vector<Node *> nodes;
    char init_state[DIM * DIM + 1]{};
    char dest_state[DIM * DIM + 1]{};
    static bool check(const char *p1, const char *p2);
    int DFS(Node *n, int threshold);
    state_manage *state_manager;
    const int INF = 100000;
    std::unordered_map<std::string, Info> visited;
    string h_func;

};

IDAStar::~IDAStar() {
    for (auto &n: this->nodes) {
        delete[] n->state;
        delete n;
    }
    delete this->state_manager;
}

IDAStar::IDAStar(const char *init_state, const char *dest_state, string h_func, ofstream &resultFile) {

    //this->resultFile = std::move(resultFile);//todo
    //this->resultFile.copyfmt(resultFile);
    //this->resultFile.clear(resultFile.rdstate());
    this->resultFile.basic_ios<char>::rdbuf(resultFile.rdbuf());

    this->h_func = std::move(h_func);
    this->state_manager = new state_manage(dest_state);

    this->cnt_node = 0;
    Node *n = new Node;
    n->state = new char[DIM * DIM + 1];
    for (int i = 0; i < DIM * DIM + 1; ++i) {
        n->state[i] = init_state[i];
        this->init_state[i] = init_state[i];
        this->dest_state[i] = dest_state[i];
    }
    n->from_parent_movement.first = 0;
    n->from_parent_movement.second = '\0';
    n->depth = 0;
    if (this->h_func == "h1")
        n->h = this->state_manager->h_function(n->state);//TODO
    else
        n->h = this->state_manager->h_function2(n->state);
    n->parent = nullptr;
    this->nodes.emplace_back(n);

}

void IDAStar::graph_search() {
    Node *n = this->nodes[0];
    int threshold;          //TODO
    if (this->h_func == "h1")
        threshold = this->state_manager->h_function(n->state);
    else
        threshold = this->state_manager->h_function2(n->state);

    this->visited.emplace(n->state, Info{true, n});
    while (true) {
        int temp = DFS(n, threshold);
        if (temp == -1) {
            return;
        }
        if (temp == this->INF) {
            return;
        }
        threshold = temp;
    }
}

bool IDAStar::check(const char *p1, const char *p2) {
    for (int i = 0; i < DIM * DIM + 1; ++i) {
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
        print_movement(n, this->resultFile);//TODO
        return -1;
    }
    int min_val = this->INF;
    auto succeeds = state_manage::get_succeed_states(n->state);
    for (auto &s: succeeds) {
        auto n_find = this->visited.find(s.second);
        if (n_find == this->visited.end()) {
            Node *n_temp = new Node;
            n_temp->state = s.second;
            n_temp->from_parent_movement = s.first;
            n_temp->depth = n->depth + 1;
            n_temp->parent = n;
            if(this->h_func == "h1")
                n_temp->h = this->state_manager->h_function(n_temp->state);//TODO
            else
                n_temp->h = this->state_manager->h_function2(n_temp->state);

            this->nodes.emplace_back(n_temp);

            this->visited.emplace(n_temp->state, Info{true, n_temp});

            int temp = DFS(n_temp, threshold);
            this->visited[n_temp->state].in_stack = false;
            if (temp == -1) {
                return -1;
            }
            if (temp < min_val) {
                min_val = temp;
            }
        } else {
            if (!n_find->second.in_stack) {
                if (n->depth + 1 <= n_find->second.n_his->depth) {
                    Node *n_temp = n_find->second.n_his;
                    n_temp->depth = n->depth + 1;
                    n_temp->parent = n;
                    n_temp->from_parent_movement = s.first;
                    n_find->second.in_stack = true;
                    int temp = DFS(n_temp, threshold);
                    n_find->second.in_stack = false;

                    if (temp == -1) {
                        return -1;
                    }
                    if (temp < min_val) {
                        min_val = temp;
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
    for (auto &n: this->nodes) {
        delete[] n->state;
        delete n;
    }
    delete this->state_manager;
}

AStar::AStar(const char *init_state, const char *dest_state, string h_func) {
    this->h_func = std::move(h_func);
    Node *n = new Node;
    n->state = new char[DIM * DIM + 1];
    for (int i = 0; i < DIM * DIM + 1; ++i) {
        n->state[i] = init_state[i];
        this->init_state[i] = init_state[i];
        this->dest_state[i] = dest_state[i];
    }
    n->from_parent_movement.first = 0;
    n->from_parent_movement.second = '\0';
    n->depth = 0;
    n->parent = nullptr;
    this->insert_closeset(n->state);
    this->nodes.emplace_back(n);
    this->state_manager = new state_manage(dest_state);
}

bool AStar::insert_closeset(const char *state) {
    std::string s(state);
    if (this->close_set.find(s) == this->close_set.end()) {
        this->close_set.insert(s);
        return true;
    }
    return false;
}

Node *AStar::graph_search() {
    Node *n = this->nodes[0];
    int h;
    if (this->h_func == "h1")
        h = this->state_manager->h_function(n->state);
    else
        h = this->state_manager->h_function2(n->state);
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
            //cout << this->fringe.size();
            return n;
        }
        auto succeeds = state_manage::get_succeed_states(n->state);
        for (auto &s: succeeds) {
            if (this->insert_closeset(s.second)) {
                Node *n_temp = new Node;
                n_temp->state = s.second;
                n_temp->from_parent_movement = s.first;
                n_temp->depth = n->depth + 1;
                n_temp->parent = n;
                int n_temp_g = n_temp->depth;
                int n_temp_h;
                if (this->h_func == "h1")
                    n_temp_h = this->state_manager->h_function(n_temp->state);
                else
                    n_temp_h = this->state_manager->h_function2(n_temp->state);
                int n_temp_f = n_temp_g + n_temp_h;
                fringe.push(std::make_pair(std::make_pair(n_temp_f, n_temp_g), n_temp));
                this->nodes.emplace_back(n_temp);
            }
        }
    }
    return nullptr;
}

bool AStar::check(const char *p1, const char *p2) {
    for (int i = 0; i < DIM * DIM; ++i) {
        if (p1[i] != p2[i]) {
            return false;
        }
    }
    return true;
}

state_manage::state_manage(const char *dest_state) {
    this->dest_state = new char[DIM * DIM + 1];
    for (auto i = 0; i < DIM * DIM; i++)
        this->dest_state[i] = dest_state[i];
    this->dest_state[DIM * DIM] = '\0';
}

std::vector<std::pair<std::pair<int, char>, char *>> state_manage::get_succeed_states(const char *state) {
    std::vector<std::pair<std::pair<int, char>, char *>> result;
    int zero_loc = 0;
    for (auto i = 0; i < DIM * DIM; i++) {
        if (state[i] == ZERO) {
            zero_loc = i;
            break;
            //printf("OK!");
        }

        //printf("%d ", state[i]);
    }
    //printf("\n");
    if ((zero_loc != 0) && (zero_loc != 5) && (zero_loc != 10) && (zero_loc != 15) && (zero_loc != 20) && (state[zero_loc - 1] != MINUS))
        result.emplace_back(make_pair(zero_loc, 'l'), exchange_state(state, zero_loc, zero_loc - 1));
    if ((zero_loc != 4) && (zero_loc != 9) && (zero_loc != 14) && (zero_loc != 19) && (zero_loc != 24) && (state[zero_loc + 1] != MINUS))
        result.emplace_back(make_pair(zero_loc, 'r'), exchange_state(state, zero_loc, zero_loc + 1));
    if ((zero_loc - DIM >= 0) && (state[zero_loc - DIM] != MINUS))
        result.emplace_back(make_pair(zero_loc, 'u'), exchange_state(state, zero_loc, zero_loc - DIM));
    if ((zero_loc + DIM <= 24) && (state[zero_loc + DIM] != MINUS))
        result.emplace_back(make_pair(zero_loc, 'd'), exchange_state(state, zero_loc, zero_loc + DIM));

    //隧道
    if (zero_loc == 2 && (state[zero_loc + 4 * DIM] != MINUS)) {
        result.emplace_back(make_pair(zero_loc, 'u'),
                            exchange_state(state, zero_loc, zero_loc + 4 * DIM));
    } else if (zero_loc == 10 && (state[zero_loc + DIM - 1] != MINUS)) {
        result.emplace_back(make_pair(zero_loc, 'l'),
                            exchange_state(state, zero_loc, zero_loc + DIM - 1));
    } else if (zero_loc == 14 && (state[zero_loc - DIM + 1] != MINUS)) {
        result.emplace_back(make_pair(zero_loc, 'r'),
                            exchange_state(state, zero_loc, zero_loc - DIM + 1));
    } else if (zero_loc == 22 && (state[zero_loc - 4 * DIM] != MINUS)) {
        result.emplace_back(make_pair(zero_loc, 'd'),
                            exchange_state(state, zero_loc, zero_loc - 4 * DIM));
    }


    return result;
}

int state_manage::h_function(const char *state) const {
    int mis_count = 0;
    for (int i = 0; i < DIM * DIM; i++) {
        if (this->dest_state[i] != state[i])
            mis_count++;
    }
    return mis_count;
}

int state_manage::h_function2(const char *state) const {
    int miss_count = 0;
    for (int i = 0; i < DIM * DIM; i++) {
        for (int j = 0; j < DIM * DIM; j++) {
            if (state[i] == this->dest_state[j]) {
                if (i == j)
                    break;
                else {
                    int xi = i % DIM, xj = j % DIM, yi = i / DIM, yj = j / DIM;
                    int x_distance = abs(xi - xj), y_distance = abs(yi - yj);
                    if (x_distance >= 3)
                        miss_count += (5 - x_distance + y_distance);
                    else if (y_distance >= 3)
                        miss_count += (5 - y_distance + x_distance);
                    else
                        miss_count += (y_distance + x_distance);
                    break;
                }
            }
        }
    }
    return miss_count;
}
//miss_count += max(abs(xi - xj), abs(yi - yj));

void A_h1(const char *init_state, const char *dest_state, ofstream &resultFile){
    string h_func = "h1";
    AStar a_star(init_state, dest_state, h_func);
    Node *n = a_star.graph_search();
    print_movement(n, resultFile);
}

void A_h2(const char *init_state, const char *dest_state, ofstream &resultFile){
    string h_func = "h2";
    AStar a_star(init_state, dest_state, h_func);
    Node *n = a_star.graph_search();
    print_movement(n, resultFile);
}

void IDA_h1(const char *init_state, const char *dest_state, ofstream &resultFile){
    string h_func = "h1";
    IDAStar ida_star(init_state, dest_state, h_func, resultFile);
    ida_star.graph_search();
}

void IDA_h2(const char *init_state, const char *dest_state, ofstream &resultFile){
    string h_func = "h2";
    IDAStar ida_star(init_state, dest_state, h_func, resultFile);
    ida_star.graph_search();
}

int main(int argc, char *argv[]) {

    std::ifstream initFile, destFile;
    ofstream resultFile;
    auto input = new string(argv[2]);
    auto output = new string(argv[3]);
    initFile.open("C://AI_Lab1//data//" + *input);
    destFile.open("C://AI_Lab1//data//" + *output);
    resultFile.open("C://AI_Lab1//result//result10.txt");
    char *init_state = new char[DIM * DIM + 1];
    char *dest_state = new char[DIM * DIM + 1];
    std::string buffer;

    for (int i = 0; i < DIM * DIM; ++i) {
        initFile >> buffer;
        char num = (char) std::stoi(buffer);
        if (num == 0) {
            init_state[i] = ZERO;
        } else if (num < 0) {
            init_state[i] = MINUS;
        } else {
            init_state[i] = num;
        }
    }
    for (int i = 0; i < DIM * DIM; ++i) {
        destFile >> buffer;
        char num = (char) std::stoi(buffer);
        if (num == 0) {
            dest_state[i] = ZERO;
        } else if (num < 0) {
            dest_state[i] = MINUS;
        } else {
            dest_state[i] = num;
        }
    }
    init_state[DIM * DIM] = '\0';
    dest_state[DIM * DIM] = '\0';

    //cout << argv[1][0] << argv[1][3];
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    if (argv[1][0] == 'A') {
        if(argv[1][3] == '1')
            A_h1(init_state, dest_state, resultFile);
        else
            A_h2(init_state, dest_state, resultFile);
    }
    else{
        if(argv[1][5] == '1')
            IDA_h1(init_state, dest_state, resultFile);
        else
            IDA_h2(init_state, dest_state, resultFile);

    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();

    resultFile << "," << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << "s";

    return 0;
}
