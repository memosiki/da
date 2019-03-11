#ifndef TPATRICIATRIE_H_
#define TPATRICIATRIE_H_

#include <iostream>
#include <cstring>
#include <fstream>

const int MAX_SIZE = 257;
typedef unsigned long long TPatriciaData;
typedef char TKey;

class TPatriciaTrieNode {

public:

    TPatriciaData GetValue();

    virtual ~TPatriciaTrieNode();

private:
	int id; //unic number of node
	//used for serialization
	//calculated at Index

	TKey* key;
	TPatriciaData value;
	int skip;
	TPatriciaTrieNode* links[2];
	//links to subtries
	//0-left 1-rigth
	//links to the nodes and up markers more useful to keep separated

	friend class TPatriciaTrie;
	TPatriciaTrieNode();
	TPatriciaTrieNode(TKey*, TPatriciaData, int sk);
	void Init(TKey*, TPatriciaData, int sk, TPatriciaTrieNode* left,
			TPatriciaTrieNode* rigth);

};

class TPatriciaTrie {
private:
	TPatriciaTrieNode* head;
	int size;

	static bool KeyCompare(TKey*, TKey*);
	static int BitLen(TKey*);
	static int ByteLen(TKey*);
	static int BitGet(TKey*, int);
	static int FirstDifferentBit(TKey*, TKey*);
	void SwapKeys(TPatriciaTrieNode* nodea, TPatriciaTrieNode* nodeb);
	void SetKey(TPatriciaTrieNode* nodea, TPatriciaTrieNode* nodeb);

	void DestructRecursive(TPatriciaTrieNode* n);
//	void WriteRecursive(std::ofstream* file, TPatriciaTrieNode* node);
//	TPatriciaTrieNode* ReadRecursive(std::ifstream* file, TPatriciaTrieNode** nodes, int* depth);
	void Index(TPatriciaTrieNode* node, TPatriciaTrieNode** nodes, int* depth);
	void PrintNode(TPatriciaTrieNode* node, bool leaf, int lvl);

public:
	TPatriciaTrie();
	bool Insert(TKey*, TPatriciaData);
	TPatriciaTrieNode* Find(TKey*);
	bool Delete(TKey* k);
	bool IsEmpty();
	void Print();
	bool Save(std::ofstream*);
	bool Load(std::ifstream*);

	virtual ~TPatriciaTrie();
}
;

#endif /* TTPATRICIATRIE_H_ */
