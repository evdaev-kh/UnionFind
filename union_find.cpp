#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

class UnionFind {
public:
    struct Node {
        std::string val;
        uint32_t size;
        Node * next;
        Node() {};
        Node(std::string value, int size=1, Node *next=nullptr): val(value), size(size), next(next) {};
    };
    UnionFind() {};                                                     //default constructor
    UnionFind(std::vector<std::string> S);                                     //constructor with nodes
    ~UnionFind();                                                       //destructor
    Node * InsertNode(std::string u);
    Node * Find(std::string v);
    uint32_t Union(std::string v, std::string u);
    bool debug{true};
private:
    std::unordered_map<std::string, Node *> components;                        //ds containing all the nodes of the graph
};
/**
 * @brief Construct a new Union Find object by populating the components of the hash map
 */
UnionFind::UnionFind(std::vector<std::string> S) {
    for (auto node : S) {
        Node * node_pointer = new Node(node);
        if (debug) std::cout << __func__<< ": node " << node_pointer ->val << " size " << node_pointer->size << "\n";
        if (components.find(node) == components.end()) components[node] = node_pointer;
    }
}

UnionFind::~UnionFind() {
    printf("[%s]: Destructor\n", __func__);
    for (auto node : this ->components) {
        if(debug) std::cout <<  __func__ << ": node " << node.first << "\n";
        delete node.second;
    }
}

UnionFind::Node * UnionFind::InsertNode(std::string u) {
    if(components.find(u) != components.end()) return components[u];
    Node* node = new Node(u);
    components[u] = node;
    return node;
}

/**
 * @brief Given a node v find the set that it belongs to
 */
UnionFind::Node * UnionFind::Find(std::string v) {

    if (this -> components.find(v) == this -> components.end()) {
        std::cout <<  __func__ << ": Node " << v << " is not part of the graph\n";
        return nullptr;
    }
    auto node = this -> components[v];
    while (node -> next != nullptr) {
        if (this ->debug) {
            std::cout <<  __func__ << ": Node " << node->val << " Size " << node->size << "\n";
        }
        node = node -> next;
    }
    return node;
}

/**
 * @brief Given two sets, represented by u and v, make a union of them
 */
uint32_t UnionFind::Union(std::string v, std::string u) {
    //First find the sets to which v and u belong
    auto set1 = Find(v);
    auto set2 = Find(u);

    if (set1 == nullptr || set2 == nullptr) {
        std::cout <<  __func__ << ": Nodes u or v are nullpointers, inserting them into components\n";
        set1 = InsertNode(v);
        set2 = InsertNode(u);
    }
    if (set1 == set2) {
        std::cout <<  __func__ << ": Nodes u or v are from the same set\n";
        return -1;
    }

    Node * larger_set(nullptr), *smaller_set(nullptr);
    if (set1 -> size < set2 -> size) {
        larger_set = set2;
        smaller_set = set1;
    } else {
        larger_set = set1;
        smaller_set = set2;
    }
    
    if (this -> debug) {
        std::cout <<  __func__ << ": Larger set: " << larger_set ->val << "\n";
        std::cout <<  __func__ << ": Smaller set: " << smaller_set ->val << "\n";
        std::cout <<  __func__ << ": Larger set size: " << larger_set ->size << "\n";
        std::cout <<  __func__ << ": Smaller set size: " << smaller_set ->size << "\n";
    }
    //Update the pointers of smaller and larger sets and their sizes
    smaller_set -> next = larger_set;
    larger_set -> size += smaller_set -> size;
    smaller_set -> size = larger_set -> size;

    return larger_set -> size;
}