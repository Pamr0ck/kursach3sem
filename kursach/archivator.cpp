#include "archivator.h"



archivator::archivator()
{

}

QString archivator::to_binar(wchar_t buf){
//    unsigned short mask[] = { 1, 2, 4, 8, 16, 32, 64, 128};

    unsigned short mask[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    QString result;
    int i = QCHARLEN;

    while( i-- )
    {
        result += (buf & mask[i]) ? L'1' : L'0';
    }
    return result;
}

QChar archivator::to_letter(QVector<wchar_t> input){
    int i=0;
    unsigned short mask[] = {32768,16384,8192,4096,2048,1024,512,256,128, 64, 32, 16, 8 , 4, 2, 1};
//    unsigned short mask[] = {128, 64, 32, 16, 8 , 4, 2, 1};
    unsigned long long d_letter = 0;
    for (wchar_t it:input){
        d_letter += (it-L'0' )*mask[i];
        i++;
    }
    wchar_t tmp = (wchar_t)d_letter;
    return (QChar)tmp;
}

void archivator::inc_weight(Node* n){
    n->w++;
    if(n->parent) inc_weight(n->parent);
}

void archivator::archivator::switch_nodes(Node* a, Node* b){
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

QVector<Node*> archivator::bfs(Node* root){    // список элементов наоборот
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

void archivator::recount_weights(QVector<Node*> &tree){
    for(auto i = 1; i < tree.size(); i+=2){
        tree[i]->parent->w = tree[i]->w + tree[i]->parent->left->w;
    }
}

void archivator::balance(Node* root){
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


QString archivator::pref(Node* esc){   //prefix of letter
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

void archivator::readletter(QString stream, code &letter, unsigned long long &i, const int &letterlen){
    wchar_t buf;
    const unsigned long long tmp = i;
    for(i; i < letterlen+tmp; i++){
        buf = stream.at(i).unicode();
        letter.push_back(buf);
    }
}

int archivator::is_leaf(Node* n){
    if(n->left || n->right)
        return 0;
    else if (n->w)
        return 1;
    return -1;
}

Node* archivator::find(Node *root, code path){
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

QString archivator::DoCode (QString stream){
    QString coded;
    wchar_t buf;
    unsigned long long i = 0;
    std::map<wchar_t,Node*> leafs;

//    std::vector<std::string> code;

    Node* root = new Node;
    Node* esc = root;

//    QString str;

    std::cout << "Print a message needs to be encrypted:" << std::endl;
//    std::getline(std::cin, str);

//    std::stringstream stream(str);

//    while(stream.get(buf)){
    while(i<stream.length()){
        buf = stream.at(i).unicode();
        i++;
        if(leafs[buf]){
            inc_weight(leafs[buf]);    // + balance??
            //
            coded+=pref(leafs[buf]);
            //
//            code.push_back(pref(leafs[buf]));
        } else {

            //
            coded+=pref(esc);
            coded+=to_binar(buf);
            //
//            code.push_back(pref(esc) + "\'" + buf + "\'");

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

//    for(auto it: code)
//        std::cout << it << " ";
//    std::cout << std::endl;

//    for(auto it: coded) {

//        printf("%c", it);
//    }
//    std::cout << std::endl;
    ROOT = root;
    return coded;

}


QString archivator::DoDeCode(QString stream){
    wchar_t buf;
    code letter;
    unsigned long long i = 0;
    std::map<code, Node*> leafs;
    std::vector<code> message;

    QString text;

    Node* root = new Node;
    Node* esc = root;


    while(i<stream.length()){
        buf = stream.at(i).unicode();
        i++;
        if(esc == root){
            readletter(stream, letter,i, QCHARLEN - 1);
            letter.insert(letter.begin(), buf);
         //
            text.push_back(to_letter(letter));
            //

            leafs[letter] = new Node;
            leafs[letter]->letter_bin = letter;
            root = new Node;
            root->left = esc;
            esc->parent = root;

            root->right = leafs[letter];
            leafs[letter]->parent = root;
            inc_weight(leafs[letter]);



            leafs[letter]->letter = to_letter(leafs[letter]->letter_bin);

            letter.clear();
        } else {
            letter.push_back(buf);
            Node *intree = find(root, letter);

            if(intree){
                switch(is_leaf(intree)){
                case  1:{
                    letter.clear();

                    //
                    text.push_back(to_letter(intree->letter_bin));
                    //
                    inc_weight(intree);
                    break;
                }
                case -1:{
                    letter.clear();
                    readletter(stream, letter,i, QCHARLEN);
                    //
                    text.push_back(to_letter(letter));
                    //
                    leafs[letter] = new Node;
                    leafs[letter]->letter_bin = letter;
                    Node *n = new Node;
                    n->parent = esc->parent;
                    n->parent->left = n;

                    n->left = esc;
                    esc->parent = n;

                    n->right = leafs[letter];
                    leafs[letter]->parent = n;
                    inc_weight(leafs[letter]);
                    leafs[letter]->letter = to_letter(leafs[letter]->letter_bin);
                    letter.clear();
                    break;
                }
                default:  continue;
                }
            } else {
                std::cout << "corrupted data" << std::endl;
                return nullptr ;
            }
        }
        balance(root);
    }

    ROOT = root;
    return text;
}

