#include "TPatriciaTrie.h"
TPatriciaTrieNode::TPatriciaTrieNode() :
        id(0), key(nullptr), value(0), skip(0) {
    links[0] = this;
    links[1] = this;

}
TPatriciaTrieNode::TPatriciaTrieNode(TKey* k, TPatriciaData v, int sk) :
        id(0), key(k), value(v), skip(sk) {
    links[0] = this;
    links[1] = this;
}

TPatriciaData TPatriciaTrieNode::GetValue() {
    return this->value;
}
void TPatriciaTrieNode::Init(TKey* k, TPatriciaData v, int sk,
        TPatriciaTrieNode* left, TPatriciaTrieNode* rigth) {
    id = 0;
    key = k;
    value = v;
    skip = sk;
    links[0] = left;
    links[1] = rigth;
}
TPatriciaTrieNode::~TPatriciaTrieNode() {
    if (this->key != nullptr)
        delete[] this->key;
}

////////////////////////////////////////////////////////////////////////

TPatriciaTrie::TPatriciaTrie() :
        size(0) {
    //head will not moves around the trie during 'Delete'
    head = new TPatriciaTrieNode(nullptr, 0, -1);

}

TPatriciaTrie::~TPatriciaTrie() {
    DestructRecursive(head);
}

void TPatriciaTrie::DestructRecursive(TPatriciaTrieNode* node) {

    if (node->links[0]->skip > node->skip)
        DestructRecursive(node->links[0]);
    if (node->links[1]->skip > node->skip)
        DestructRecursive(node->links[1]);
    delete node;

}

TPatriciaTrieNode* TPatriciaTrie::Find(TKey* k) {

    if (this->IsEmpty())
        return nullptr;

    TPatriciaTrieNode* pref = head; //previous checked node (so it's PREFix)
    TPatriciaTrieNode* ref = head->links[0]; //currently checking node

    while (pref->skip < ref->skip) {
        pref = ref;
        ref = pref->links[BitGet(k, pref->skip)];
    }
    if (!KeyCompare(k, ref->key))
        return nullptr;

    return ref;
}

bool TPatriciaTrie::Insert(TKey* k, TPatriciaData d) {
//returns:
//true  - inserted
//false - exists
//
//do nothing to TKey *k

    TPatriciaTrieNode* prev = head; //previous checked node
//this will go before node that will be inserted

    TPatriciaTrieNode* nxt = head->links[0]; //current checked node
//that will go after node that will be inserted
    while (prev->skip < nxt->skip) {
        prev = nxt;
        nxt = prev->links[BitGet(k, nxt->skip)];
    }

    if (KeyCompare(k, nxt->key))
        //already in trie
        return false;

    int bitPrefix = FirstDifferentBit(k, nxt->key);
    prev = head;
    nxt = head->links[0];
    while (prev->skip < nxt->skip && nxt->skip < bitPrefix) {
        prev = nxt;
        nxt = prev->links[BitGet(k, nxt->skip)];
    }
//no changes to trie were made so far
//found very place for key
//need to split and insert new node

    //compact memory resolution for keys
    TKey* compactKey;
    try {
        compactKey = new TKey[ByteLen(k) + 1];
    } catch (const std::bad_alloc &) {
        std::cout << "ERROR: not enough memory" << std::endl;
        return false;
    }
    std::strcpy(compactKey, k);
    TPatriciaTrieNode* newNode = new TPatriciaTrieNode(compactKey, d,
            bitPrefix);
    prev->links[BitGet(compactKey, prev->skip)] = newNode;
    newNode->links[BitGet(compactKey, bitPrefix)] = newNode;
    newNode->links[1 - BitGet(compactKey, bitPrefix)] = nxt;
    this->size++;
    return true;
}
bool TPatriciaTrie::Delete(TKey* k) {
// deletes node with 'k' if it is exist
//
//ORDER:
//
//grandParent -> parent -> (link up)del
//                 |       			 ^
//             move here(if differ)--^
//
// and delete 'parent'
//
    TPatriciaTrieNode *grandParent = nullptr; //grand parent  lies before parent
    TPatriciaTrieNode *parent = head; //link from parent leads to node with key we need
    TPatriciaTrieNode *del = head->links[0]; // node with key 'k' (deleting this)

    while (parent->skip < del->skip) {
        grandParent = parent;
        parent = del;
        del = del->links[BitGet(k, del->skip)];
    }

    if (!KeyCompare(k, del->key))
        return false;    //no such key in trie

    if (del != parent) {
        SetKey(del, parent);    //sets key of 'parent' to 'del'
        del->value = parent->value;
        //so now deleting 'parent'
    }

    if (parent->links[0]->skip > parent->skip
            || parent->links[1]->skip > parent->skip) {

        //'parent' is not a leaf
        //so at least link leads downward
        if (parent != del) {
            //if link upward lies deeper than nearest child
            //we have to find exact node with this link
            TPatriciaTrieNode* parentOfParent = parent;
            TPatriciaTrieNode* tmp = parent->links[BitGet(parent->key,
                    parent->skip)];
            TKey* key = parent->key;
            //exactParent will be new parent to del
            while (parentOfParent->skip < tmp->skip) {
                parentOfParent = tmp;
                tmp = parentOfParent->links[BitGet(key, parentOfParent->skip)];
            }

            if (!KeyCompare(key, tmp->key)) {
                std::cout
                        << "ERROR: logical error during Delete (incorrect generated trie?)";
                std::cout << std::endl;
                return false;
            }

            parentOfParent->links[BitGet(key, parentOfParent->skip)] = del;

        }

        if (grandParent != parent)
            //link that leads downward
            grandParent->links[BitGet(k, grandParent->skip)] = parent->links[1
                    - BitGet(k, parent->skip)];

    } else {

        //if it is leaf
        //(so at least one link leads to itself
        //and all of them leads upward)

        //if links of parent leads somewhere else
        //have to wire it to grandParent

        //if all links from parent leads to 'parent'
        //should point freed link to grandParent

        if (grandParent != parent) {
            //if parent has link not on him
            //rewire grandpa to it
            //else wire grandpa to himself
//            if (parent->links[0] == parent) {
//                if (parent->links[1] == parent)
//                    grandParent->links[BitGet(k, grandParent->skip)] =
//                            grandParent;
//                else
//                    grandParent->links[BitGet(k, grandParent->skip)] =
//                            parent->links[1];
//            } else
//                grandParent->links[BitGet(k, grandParent->skip)] =
//                        parent->links[0];

//	trying to make it more readable
//
			grandParent->links[BitGet(k, grandParent->skip)] =
					(parent->links[0] == parent) ?
							(parent->links[1] == parent) ?
									grandParent : parent->links[1]
							:parent->links[0];
        }
    }
    this->size--;
    delete parent;
    return true;
}

void TPatriciaTrie::SwapKeys(TPatriciaTrieNode* nodea,
        TPatriciaTrieNode* nodeb) {
    TKey* tmp = nodea->key;
    TPatriciaData dat = nodea->value;
    nodea->key = nodeb->key;
    nodea->value = nodeb->value;
    nodeb->key = tmp;
    nodeb->value = dat;
}

void TPatriciaTrie::SetKey(TPatriciaTrieNode* to, TPatriciaTrieNode* from) {

    if (from->key == nullptr)
        to->key = nullptr;

    else
        std::strcpy(to->key, from->key);
}
bool TPatriciaTrie::IsEmpty() {
//tree is empty
//if all links from head leads to head(up)
    return (head->links[0] == head) && (head->links[1] == head);
}
int TPatriciaTrie::BitLen(TKey* k) {
    if (k == nullptr)
        return 0;
    return std::strlen(k) * 8;
}
int TPatriciaTrie::ByteLen(TKey* k) {
    if (k == nullptr)
        return 0;
    return std::strlen(k);
}
int TPatriciaTrie::BitGet(TKey* k, int bit) {
    //optimized version
    if (bit < 0)
        bit = 0;
    //sizeof(TKey)==8
    //which bit needed relative to 1 character
    //               bit % 8
    int count = 7 - (bit & 7);

    // k[bit >> 3]  <==>  k[bit/8]
    //needed bit from this character
    //
    //   >> count
    //moving needed bit to far rigth
    //and get it
    return ((k[bit >> 3] >> count) & 1U);
}
bool TPatriciaTrie::KeyCompare(TKey* key1, TKey* key2) {
    if (key1 == key2)     //same links (most cases)
        return true;
    int len = BitLen(key1);
    if (len != BitLen(key2)) //different lengths
        return false;

    if (FirstDifferentBit(key1, key2) != len)
        return false;
    return true;

}
int TPatriciaTrie::FirstDifferentBit(TKey* keya, TKey* keyb) {
    if (!keya)
        return 0;
    if (!keyb)
        return 0;

    size_t differ = 0;
    size_t lena = ByteLen(keya);
    size_t lenb = ByteLen(keyb);
    size_t minlen = lenb > lena ? lena : lenb;
    size_t maxlen = lenb < lena ? lena : lenb;
    //finding first different character
    while ((keya[differ] == keyb[differ]) && differ < minlen)
        differ++;
    differ *= 8;
    maxlen *= 8;
    while (BitGet(keyb, differ) == BitGet(keya, differ) && differ < maxlen)
        //finding first different bit
        differ++;
    return differ;

}

bool TPatriciaTrie::Save(std::ofstream* file) {
    file->write((char*) &(this->size), sizeof(int));
    int index = 0;
    TPatriciaTrieNode** nodes = new TPatriciaTrieNode*[this->size + 1];
    TPatriciaTrieNode* node;
    Index(head, nodes, &index);
    for (int i = 0; i < (size + 1); ++i) {
        node = nodes[i];
        file->write((char*) &(node->value), sizeof(TPatriciaData));
        file->write((char*) &(node->skip), sizeof(int));
        int len = ByteLen(node->key);
        file->write((char*) &(len), sizeof(int));
        file->write(node->key, len);
        file->write((char*) &(node->links[0]->id), sizeof(int));
        file->write((char*) &(node->links[1]->id), sizeof(int));
    }
    delete[] nodes;
    if (file->fail())
        return false;
    return true;
//old recursive version
//
//	file->write((char*) &(this->size), sizeof(int));
//	if (IsEmpty()) {
//		int downward = 0;
//		int link = 0;
//		file->write((char*) &downward, sizeof(downward));
//		file->write((char*) &link, sizeof(link));
//	} else {
//		int index = -1;
//		Index(head, &index);
//		WriteRecursive(file, this->head->links[0]);
//	}
}
void TPatriciaTrie::Index(TPatriciaTrieNode* node, TPatriciaTrieNode** nodes,
        int* index) {
    node->id = *index;
    nodes[*index] = node;
    (*index)++;
    if (node->links[0]->skip > node->skip)
        Index(node->links[0], nodes, index);
    if (node->links[1]->skip > node->skip)
        Index(node->links[1], nodes, index);

}
//void TPatriciaTrie::WriteRecursive(std::ofstream* file,
//		TPatriciaTrieNode* node) {
//	int downward = 1;
//	file->write((char*) &downward, sizeof(downward));
//	file->write((char*) &(node->value), sizeof(TPatriciaData));
//	file->write((char*) &(node->skip), sizeof(int));
//	int len = std::strlen(node->key);
//	file->write((char*) &(len), sizeof(int));
//	file->write(node->key, len);
//	downward = 0;
//	if (node->links[0]->skip > node->skip)
//		WriteRecursive(file, node->links[0]);
//	else {
//		file->write((char*) &downward, sizeof(downward));
//		file->write((char*) &(node->links[0]->id), sizeof(int));
//	}
//	if (node->links[1]->skip > node->skip)
//		WriteRecursive(file, node->links[1]);
//	else {
//		file->write((char*) &downward, sizeof(downward));
//		file->write((char*) &(node->links[1]->id), sizeof(int));
//	}
//}
bool TPatriciaTrie::Load(std::ifstream* file) {
    int size;
    TPatriciaTrieNode** nodes;
    file->read((char*) &size, sizeof(int));
    this->size = size;
    if (!size)
        return true;
    try {
        nodes = new TPatriciaTrieNode*[size + 1];
    } catch (const std::bad_alloc &) {
        return false;
    }
    nodes[0] = this->head;

    //making nodes
    int var;
    try {
        for (var = 1; var < (size + 1); ++var)
            nodes[var] = new TPatriciaTrieNode();
    } catch (const std::bad_alloc &) {
        std::cout << "ERROR: not enough memory" << std::endl;
        for (int i = 0; i < var; ++i)
            delete nodes[i];
        delete[] nodes;
        return false;
    }

    TPatriciaData value;
    int skip;
    int len;
    TKey* key;
    int indLeft, indRight; //childs
    try {
        for (int i = 0; i < (size + 1); ++i) {
            file->read((char*) &(value), sizeof(TPatriciaData));
            file->read((char*) &(skip), sizeof(int));
            file->read((char*) &(len), sizeof(int));
            key = new char[len + 1];
            key[len] = 0;
            file->read(key, len);
            file->read((char*) &(indLeft), sizeof(int));
            file->read((char*) &(indRight), sizeof(int));
            nodes[i]->Init(key, value, skip, nodes[indLeft], nodes[indRight]);
        }
    } catch (const std::bad_alloc &) {
        std::cout << "ERROR: not enough memory" << std::endl;
        for (int i = 0; i < this->size; i++)
            delete nodes[i];
        delete[] nodes;
        return false;
    }
    file->peek();
    if (file->fail() || !file->eof()) {
        //wrong format
        for (int i = 0; i < this->size; i++)
            delete nodes[i];
        delete[] nodes;
        return false;
    }
    delete[] nodes;
    return true;
    //old recursive version
    //	int index = 0;
    //	head->links[0] = ReadRecursive(file, nodes, &index);
}

//TPatriciaTrieNode* TPatriciaTrie::ReadRecursive(std::ifstream* file,
//		TPatriciaTrieNode** nodes, int* depth) {
//	TPatriciaTrieNode* node = nullptr;
//	int downward;
//	file->read((char*) &downward, sizeof(downward));
//
//	if (downward) {
//		TPatriciaData value;
//		int skip;
//		int len;
//		TKey* word;
//		file->read((char*) &value, sizeof(TPatriciaData));
//		file->read((char*) &skip, sizeof(int));
//		file->read((char*) &len, sizeof(int));
//		try {
//			word = new char[len + 1];
//			node = new TPatriciaTrieNode(word, value, skip);
//		} catch (const std::bad_alloc &) {
//			std::cout << "ERROR: not enough memory" << std::endl;
//			return nullptr;
//		}
//		word[len] = 0;
//		file->read(word, len);
//		(*depth)++;
//		nodes[*depth] = node;
//		node->links[0] = ReadRecursive(file, nodes, depth);
//		node->links[1] = ReadRecursive(file, nodes, depth);
//	} else {
//		int id;
//		file->read((char*) &id, sizeof(int));
//		node = nodes[id];
//	}
//	return node;
//}

