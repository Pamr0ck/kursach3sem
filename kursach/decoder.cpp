#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>

#define CHARLEN 7   // utf-8

typedef std::vector<char> code;

struct Node{
    code letter;
    int w = 0;

    Node* parent;
    Node* left;
    Node* right;
};

char to_letter(std::vector<char> input){
    int i=0;
    static char mask[] = { 64, 32, 16, 8 , 4, 2, 1};
    int d_letter = 0;
    for (char it:input){
        d_letter += (it-'0' )*mask[i];
        i++;
    }
    return (char)d_letter;
}

void inc_weight(Node *n){
    n->w++;
    if (n->parent) inc_weight(n->parent);
}


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


std::vector<Node*> bfs(Node* root){
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


std::string pref(Node* esc){
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


void readletter(std::stringstream &stream, code &letter, const int &letterlen){
    char buf;
    for(int i = 0; i < letterlen; i++){
        stream >> buf;
        letter.push_back(buf);
    }
}

int is_leaf(Node* n){
    if(n->left || n->right)
        return 0;
    else if (n->w)
        return 1;
    return -1;
}

Node* find(Node *root, code path){
    Node* current = root;
    while(!path.empty()){
        if(path[0] == '0' && current->left)
            current = current->left;
        else if (current->right)
            current = current->right;
        else return 0;
        path.erase(path.begin());
    }
    return current;
}

int main(){
    char buf;
    code letter;
    std::map<code, Node*> leafs;
    std::vector<code> message;

    std::string text;

    Node* root = new Node;
    Node* esc = root;

    std::string str;

    std::cout << "Print a message needs to be decrypted:" << std::endl;
    std::getline(std::cin, str);

    std::stringstream stream(str);

    while(stream.get(buf)){
        if(esc == root){
            readletter(stream, letter, CHARLEN - 1);
            letter.emplace(letter.begin(), buf);

            message.push_back(letter);
            //
            text.push_back(to_letter(letter));
            //

            leafs[letter] = new Node;
            leafs[letter]->letter = letter;
            root = new Node;
            root->left = esc;
            esc->parent = root;

            root->right = leafs[letter];
            leafs[letter]->parent = root;
            inc_weight(leafs[letter]);

            letter.clear();
        } else {
            letter.push_back(buf);
            Node *intree = find(root, letter);

            if(intree){
                switch(is_leaf(intree)){
                case  1:{
                    letter.clear();

                    message.push_back(intree->letter);
                    //
                    text.push_back(to_letter(intree->letter));
                    //
                    inc_weight(intree);
                    break;
                }
                case -1:{
                    letter.clear();
                    readletter(stream, letter, CHARLEN);
                    message.push_back(letter);
                    //
                    text.push_back(to_letter(letter));
                    //
                    leafs[letter] = new Node;
                    leafs[letter]->letter = letter;
                    Node *n = new Node;
                    n->parent = esc->parent;
                    n->parent->left = n;

                    n->left = esc;
                    esc->parent = n;

                    n->right = leafs[letter];
                    leafs[letter]->parent = n;
                    inc_weight(leafs[letter]);
                    letter.clear();
                    break;
                }
                default:  continue;
                }
            } else {
                std::cout << "corrupted data" << std::endl;
                return 0;
            }
        }
        balance(root);
    }

    for(auto letter: message){
        for(auto b: letter){
            std::cout << b;
        }
        std::cout << std::endl;
    }
    for(auto let:text){
        std::cout<<let;
    }
    std::cout<<std::endl;

    return 0;
}
