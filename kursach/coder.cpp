#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cstdio>

struct Node{
    char letter = 0;
    int w = 0;
//	int num = 256;

    Node *parent = NULL;
    Node *left = NULL;
    Node *right = NULL;
};


void inc_weight(Node* n){
    n->w++;
    if(n->parent) inc_weight(n->parent);
}

/*void add_node(Node* esc){
    Node *n = new Node;
    Node *leaf = new Node;

    n->parent = esc->parent;
    n->parent->left = n;
    n->left = esc;
    esc->parent = n;

    n->right = leaf;
    leaf->parent = n;
}*/


void switch_nodes(Node* a, Node* b){
    Node *tmp = a->parent;

    if(a->parent != b->parent){
        if(a->parent->left == a)
            a->parent->left = b;
        else
            a->parent->right = b;

        if(b->parent->left == b)
            b->parent->left = a;
        else
            b->parent->right = a;

        a->parent = b->parent;
        b->parent = tmp;
    } else {
        tmp = a->parent->left;
        a->parent->left = a->parent->right;
        a->parent->right = tmp;
    }
}

std::vector<Node*> bfs(Node* root){    // список элементов наоборот
    Node* tmp;
    std::vector<Node*> out;

    std::queue<Node*> tree;
    tree.push(root);

    while(!tree.empty()){
        tmp = tree.front();
        tree.pop();

        out.push_back(tmp);
        if(tmp->right)
            tree.push(tmp->right);
        if(tmp->left)
            tree.push(tmp->left);
    }
    std::reverse(out.begin(), out.end());
    return out;
}

void recount_weights(std::vector<Node*> &tree){
    for(int i = 1; i < tree.size(); i+=2){
        tree[i]->parent->w = tree[i]->w + tree[i]->parent->left->w;
    }
}

void balance(Node* root){
    std::vector<Node*> tree = bfs(root);

    int size = tree.size();
    for(int i = 0; i < size - 1; i++){
        if(tree[i]->w > tree[i+1]->w){
            int j;
            for(j = i+1; j < size-1; j++)
                if (tree[j]->w < tree[j+1]->w) break;
            switch_nodes(tree[i], tree[j]);
            tree = bfs(root);
            recount_weights(tree);
            i = 0;
            size = tree.size();
        }
    }
}

std::string binar(char buf){
    static char mask[] = { 1, 2, 4, 8, 16, 32, 64};
    std::string result;
    int i =7;
    while( i-- )
    {
        result += (buf & mask[i]) ? '1' : '0';
    }
    return result;
}

std::string pref(Node* esc){   //prefix of letter
    std::string out = "";

    Node* tmp = esc;
    while(tmp->parent){
        if(tmp->parent->left == tmp)
            out.insert(0, 1, '0');
        else out.insert(0, 1, '1');
        tmp = tmp->parent;
    }
    return out;
}


int main(){
    std::string coded;
    char buf;
    std::map<char,Node*> leafs;

    std::vector<std::string> code;

    Node* root = new Node;
    Node* esc = root;

    std::string str;

    std::cout << "Print a message needs to be encrypted:" << std::endl;
    std::getline(std::cin, str);

    std::stringstream stream(str);

    while(stream.get(buf)){
        if(leafs[buf]){
            inc_weight(leafs[buf]);    // + balance??
            //
            coded+=pref(leafs[buf]);
            //
            code.push_back(pref(leafs[buf]));
        } else {

            //
            coded+=pref(esc);
            coded+=binar(buf);
            //
            code.push_back(pref(esc) + "\'" + buf + "\'");

            leafs[buf] = new Node;
            leafs[buf]->letter = buf;
            if(esc == root) {
                root = new Node;
                root->left = esc;
                esc->parent = root;
                root->right = leafs[buf];
                leafs[buf]->parent = root;
            } else {
                Node* n = new Node;
                n->parent = esc->parent;
                n->parent->left = n;

                n->left = esc;
                esc->parent = n;

                n->right = leafs[buf];
                leafs[buf]->parent = n;
            }

            inc_weight(leafs[buf]);
        }
        balance(root);
    }

    for(auto it: code)
        std::cout << it << " ";
    std::cout << std::endl;

    for(char it: coded) {

        printf("%c", it);
    }
    std::cout << std::endl;

    return 0;
}
