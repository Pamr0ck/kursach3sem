#ifndef ARCHIVATOR_H
#define ARCHIVATOR_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <QString>
#include <QVector>
#include <QQueue>
#include <QMap>
#include <wchar.h>


#define QCHARLEN 16   // wchar_t
typedef QVector<wchar_t> code;
typedef std::vector<wchar_t> decode;

struct Node{
    QChar letter = 0;
    code letter_bin;
    int w = 0;

    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
};


class archivator
{
public:
    Node* ROOT = nullptr;

    archivator();
    QString DoCode (QString stream);
    QString DoDeCode (QString stream);
    QString to_binar(wchar_t buf);
    QChar to_letter(QVector<wchar_t> input);
    void inc_weight(Node* n);
    void switch_nodes(Node* a, Node* b);
    QVector<Node*> bfs(Node* root);
    void recount_weights(QVector<Node*> &tree);
    void balance(Node* root);
    QString pref(Node* esc);
    void readletter(QString stream, code &letter, unsigned long long &i, const int &letterlen);
    int is_leaf(Node* n);
    Node* find(Node *root, code path);

};

#endif // ARCHIVATOR_H
