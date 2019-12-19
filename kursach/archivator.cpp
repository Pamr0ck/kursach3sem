#include "archivator.h"



archivator::archivator()
{

}

QString to_binar(QChar buf){
    unsigned short mask[] = { 1, 2, 4, 8, 16, 32, 64};
    QString result;
    int i = QCHARLEN;

    while( i-- )
    {
        result += (buf & mask[i]) ? L'1' : L'0';
    }
    return result;
}

char to_letter(QVector<QChar> input){
    int i=0;
    unsigned short mask[] = { 64, 32, 16, 8 , 4, 2, 1};
    int d_letter = 0;
    for (QChar it:input){
        d_letter += (it-L'0' )*mask[i];
        i++;
    }
    return (QChar)d_letter;
}


void inc_weight(Node* n){
    n->w++;
    if(n->parent) inc_weight(n->parent);
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

QVector<Node*> bfs(Node* root){    // список элементов наоборот
    Node* tmp;
    QVector<Node*> out;

    QQueue<Node*> tree;
    tree.push_back(root);

    while(!tree.empty()){
        tmp = tree.front();
        tree.pop_front();

        out.push_back(tmp);
        if(tmp->right)
            tree.push_back(tmp->right);
        if(tmp->left)
            tree.push_back(tmp->left);
    }
    std::reverse(out.begin(), out.end());
    return out;
}

void recount_weights(QVector<Node*> &tree){
    for(auto i = 1; i < tree.size(); i+=2){
        tree[i]->parent->w = tree[i]->w + tree[i]->parent->left->w;
    }
}

void balance(Node* root){
    QVector<Node*> tree = bfs(root);

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


QString pref(Node* esc){   //prefix of letter
    QString out = "";

    Node* tmp = esc;
    while(tmp->parent){
        if(tmp->parent->left == tmp)
            out.insert(0,L'0');
        else out.insert(0, L'1');
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
        else return nullptr;
        path.erase(path.begin());
    }
    return current;
}

void DoCode (QString in){
    QString coded;
    QChar buf;
    std::map<char,Node*> leafs;

    std::vector<std::string> code;

    Node* root = new Node;
    Node* esc = root;

    QString str;

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
            coded+=to_binar(buf);
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

}


void DoDeCode(){
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
                readletter(stream, letter, QCHARLEN - 1);
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
                        readletter(stream, letter, QCHARLEN);
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
                    return ;
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
}
