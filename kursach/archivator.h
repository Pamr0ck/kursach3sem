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


//#define QChar wchar_t
#define QCHARLEN 16   // utf-8
typedef QVector<wchar_t> code;
typedef std::vector<wchar_t> decode;

struct Node{
    QChar letter = 0;
    code letter_bin;
    int w = 0;
//	int num = 256;

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
};

#endif // ARCHIVATOR_H
