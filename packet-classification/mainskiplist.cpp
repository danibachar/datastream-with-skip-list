%%writefile mainskiplist.cpp

#include <iostream>
#include <time.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <bitset>
#include <curses.h>
#include <bitset>
#include <limits>
#include <list>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <bitset>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>

inline bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

using namespace std;
using std::list;
using std::vector;
using std::ofstream;


struct Rule
{
    string perfix;
    unsigned int low;
    unsigned int high;
    int src_perfix_length;
    unsigned int src_port_low;
    unsigned int src_port_high;

    unsigned int dest_port_low;
    unsigned int dest_port_high;

    unsigned int protocol;
};

struct packetHeader
{
    unsigned int srcAddress;
    unsigned int destAddress;
    unsigned int srcPort;
    unsigned int destPort;
    unsigned int protocol;

    unsigned int unknown_field;
    unsigned int rule_number;
    vector <unsigned int> rulesMatched;
    long double 	stamptime;
};
struct packetHeaderbystamp
{
    unsigned int srcAddress;
    unsigned int destAddress;
    unsigned int srcPort;
    unsigned int destPort;
    unsigned int protocol;
    unsigned int unknown_field;
    unsigned int rule_number;


    vector <unsigned int> rulesMatched;
};

vector <Rule> srcip;
vector<Rule> desip;

Rule tem;
long long totallMemorryAccess;
long double Timetotallclassification;
int node;
int MemAccess;
int MemAccesssrc;
int MemAccessdes;
int MemAccesssp;
int MemAccessdp;
int MemAccesspro;
int totallMemorryAccessp;
long int skiplistMemorySpacesrc;
long int	skiplisMemorySpacedes;
long int	skiplisMemorySpacesp;
long int	skiplisMemorySpacedp;
long int	skiplisMemorySpacepro;

long double finallcreattime;
using namespace std;

// # Please Note Rule set is sizeable by the actual rule set! need to update per Test
#define  sizeOfRuleSet 500

#pragma region Definition Of Skip_Node structure & Skip_List class

struct Skip_Node {
    unsigned long  key;
    string value;
    vector<Skip_Node*> forward;
    bitset<sizeOfRuleSet> Rules;
    // Constructor
    Skip_Node(unsigned long k, string v, int level);
};

class Skip_list {
public:
    string str;
    Skip_list();
    ~Skip_list();

    void rule_bubbleSort1();
    unsigned int HextoInt1(string protocol);
    string  Convert_Dec_Binary1(string temp);
    void decimal_to_binary1(unsigned int decimal);
    unsigned int binary_to_decimal1(string tempBound);
    int get_perfix_lenght1(string perfix);
    void getLowerUpperBound1();

    void print();

    Skip_Node* findips(unsigned long searchKey);
    Skip_Node* findipd(unsigned long searchKey);
    Skip_Node* findprts(unsigned long searchKey);
    Skip_Node* findprtd(unsigned long searchKey);
    Skip_Node* findp(unsigned long searchKey);

    void insertips(unsigned long searchKey, string newValue);
    void insertipd(unsigned long searchKey, string newValue);
    void insertprts(unsigned long searchKey, string newValue);
    void insertprtd(unsigned long searchKey, string newValue);
    void insertp(unsigned long searchKey, string newValue);

    void erase(int searchKey);

private:
    Skip_Node* headips;
    Skip_Node* NILips;
    Skip_Node* headprts;
    Skip_Node* NILprts;
    Skip_Node* headipd;
    Skip_Node* NILipd;
    Skip_Node* headprtd;
    Skip_Node* NILprtd;
    Skip_Node* headp;
    Skip_Node* NILp;
    int randomLevel();
    int nodeLevel(const vector<Skip_Node*>& v);
    Skip_Node* makeNode(unsigned long key, string val, int level);
    // data members  
    float probability;
    int maxLevel;
};

#pragma endregion

#pragma region Implemention member function of Skip_List calss and Skip_Node struct

Skip_Node::Skip_Node(unsigned long k, string v, int level)
    : key(k), value(v)
{
    for (int i = 0; i < level; ++i) {
        forward.emplace_back(nullptr);
    }
}

Skip_list::Skip_list()
    : probability(0.5), maxLevel(15)
{
    // smallest possible key
    int headKeyips = numeric_limits<unsigned int>::min();
    headips = new Skip_Node(headKeyips, "headips", maxLevel);

    // largest possible key
    int nilKeyips = numeric_limits<int>::max();
    NILips = new Skip_Node(nilKeyips, "NILips", maxLevel);

    for (size_t i = 0; i < headips->forward.size(); ++i) {
        headips->forward[i] = NILips;
    }

    // smallest possible key
    int headKeyipd = numeric_limits<unsigned int>::min();
    headipd = new Skip_Node(headKeyipd, "headipd", maxLevel);

    // largest possible key
    int nilKeyipd = numeric_limits< unsigned int>::max();
    NILipd = new Skip_Node(nilKeyipd, "NILipd", maxLevel);

    for (size_t i = 0; i < headipd->forward.size(); ++i) {
        headipd->forward[i] = NILipd;
    }

    // smallest possible key
    int headKeysp = numeric_limits<unsigned int>::min();
    headprts = new Skip_Node(headKeysp, "headprts", maxLevel);

    // largest possible key
    int nilKeysp = numeric_limits<int>::max();
    NILprts = new Skip_Node(nilKeysp, "NILprts", maxLevel);

    for (size_t i = 0; i < headprts->forward.size(); ++i) {
        headprts->forward[i] = NILprts;
    }

    // smallest possible key
    int headKeydp = numeric_limits< unsigned int>::min();
    headprtd = new Skip_Node(headKeydp, "headprtd", maxLevel);

    // largest possible key
    int nilKeydp = numeric_limits<int>::max();
    NILprtd = new Skip_Node(nilKeydp, "NILprtd", maxLevel);

    for (size_t i = 0; i < headprtd->forward.size(); ++i) {
        headprtd->forward[i] = NILprtd;
    }

    // smallest possible key
    int headKeyp = numeric_limits< unsigned int>::min();
    headp = new Skip_Node(headKeyp, "headp", maxLevel);

    // largest possible key
    int nilKeyp = numeric_limits<int>::max();
    NILp = new Skip_Node(nilKeyp, "NILp", maxLevel);

    for (size_t i = 0; i < headp->forward.size(); ++i) {
        headp->forward[i] = NILp;
    }
}

// destructor
Skip_list::~Skip_list() {
    delete headp;
    delete NILp;
    delete headprts;
    delete NILprts;
    delete headprtd;
    delete NILprtd;
    delete headips;
    delete NILips;
    delete headipd;
    delete NILipd;
}

// Helper functions
int Skip_list::randomLevel() {
    int v = 1;

    while ((((double)std::rand() / RAND_MAX)) < probability &&
        std::abs(v) < maxLevel) {

        v += 1;
    }
    return abs(v);
}

int Skip_list::nodeLevel(const vector<Skip_Node*>& v) {
    int currentLevel = 1;

    int nilKey = std::numeric_limits<int>::max();

    if (v.size() > 0 && v[0]->key == nilKey) {
        return currentLevel;
    }

    for (size_t i = 0; i < v.size(); ++i) {

        if (v[i] != nullptr && v[i]->key != nilKey) {
            ++currentLevel;
        }
        else {
            break;
        }
    }
    return currentLevel;
}

void Skip_list::print() {
    Skip_Node* list = headp;
    int lineLenght = 1;

    cout << "{" << endl;

    while (list->forward[0] != nullptr) {
        cout << ", key: " << list->forward[0]->key
            << ", level: " << nodeLevel(list->forward);

        list = list->forward[0];

        if (list->forward[0] != nullptr) cout << " : ";

        if (++lineLenght % 2 == 0) cout << "\n";
    }
    cout << "}" << endl;
}

Skip_Node* Skip_list::findp(unsigned long searchKey) {
    Skip_Node* x = headp;
    unsigned int currentMaximum = nodeLevel(headp->forward);
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    for (unsigned int i = currentMaximum; i-- > 0;) {
        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILp") && x->forward[i]->key < searchKey) {

            x = x->forward[i];
            MemAccess++;
        }

    }
    if (x->forward.size() > 0)
    {
        x = x->forward[0];
    }

    if (x->key == searchKey) {
        return x;
    }
    else {
        return nullptr;
    }
}

Skip_Node* Skip_list::findprts(unsigned long searchKey) {
    Skip_Node* x = headprts;
    unsigned int currentMaximum = nodeLevel(headprts->forward);
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    for (unsigned int i = currentMaximum; i-- > 0;) {
        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILprts") && x->forward[i]->key < searchKey) {
            if (x->forward.size() > i)
            {
                x = x->forward[i];
            }
            MemAccess++;
        }

    }
    if (x->forward.size() > 0)
    {
        x = x->forward[0];
    }

    if (x->key == searchKey) {
        return x;
    }
    else {
        return nullptr;
    }
}

Skip_Node* Skip_list::findprtd(unsigned long searchKey) {
    Skip_Node* x = headprtd;
    unsigned int currentMaximum = nodeLevel(headprtd->forward);
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    for (unsigned int i = currentMaximum; i-- > 0;) {
        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILprtd") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i)
            {
                x = x->forward[i];
            }
            MemAccess++;
        }
    }
    if (x->forward.size() > 0)
    {
        x = x->forward[0];
    }

    if (x->key == searchKey) {
        return x;
    }
    else {
        return nullptr;
    }
}

Skip_Node* Skip_list::findips(unsigned long searchKey) {
    Skip_Node* x = headips;
    unsigned int currentMaximum = nodeLevel(headips->forward);
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    for (unsigned int i = currentMaximum; i-- > 0;) {
        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILips") && x->forward[i]->key < searchKey) {
            if (x->forward.size() > i)
            {
                x = x->forward[i];
            }
            MemAccess++;
        }

    }
    if (x->forward.size() > 0)
    {
        x = x->forward[0];
    }

    if (x->key == searchKey) {
        return x;
    }
    else {
        return nullptr;
    }
}

Skip_Node* Skip_list::findipd(unsigned long searchKey) {
    Skip_Node* x = headipd;
    unsigned int currentMaximum = nodeLevel(headipd->forward);
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    for (unsigned int i = currentMaximum; i-- > 0;) {

        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILipd") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i)
            {
                x = x->forward[i];
            }
            MemAccess++;
        }
    }
    if (x->forward.size() > 0)
    {
        x = x->forward[0];
    }

    if (x->key == searchKey) {
        return x;
    }
    else {
        return nullptr;
    }
}

Skip_Node* Skip_list::makeNode(unsigned long  key, string val, int level)
{
    return new Skip_Node(key, val, level);
}

void Skip_list::rule_bubbleSort1()
{
    Rule temp1, temp2;
    for (unsigned int i = 1;i < sizeOfRuleSet;++i)
        for (int j = 0;j < (sizeOfRuleSet - i);++j)
            if ((srcip[j].src_perfix_length + desip[j].src_perfix_length)
                < (srcip[j + 1].src_perfix_length + desip[j + 1].src_perfix_length))
            {
                temp1 = srcip[j];
                temp2 = desip[j];
                srcip[j] = srcip[j + 1];
                desip[j] = desip[j + 1];
                srcip[j + 1] = temp1;
                desip[j + 1] = temp2;
            }
}

int Skip_list::get_perfix_lenght1(string perfix)
{
    char* ch = new char[150];
    unsigned int  k;
    int i = perfix.find("/");
    string s = perfix.substr(i + 1, perfix.size());
    for (k = 0;k < s.size();k++)
        ch[k] = s[k];

    ch[k] = '\0';
    return (atoi(ch));
}

unsigned int Skip_list::HextoInt1(string protocol) {
    int a, b;
    if (protocol == "0x00/0x00")
        return 0;
    else {
        if (protocol.substr(2, 1) == "f")
            a = 15;
        else if (protocol.substr(2, 1) == "e")
            a = 14;
        else if (protocol.substr(2, 1) == "d")
            a = 13;
        else if (protocol.substr(2, 1) == "c")
            a = 12;
        else if (protocol.substr(2, 1) == "b")
            a = 11;
        else if (protocol.substr(2, 1) == "a")
            a = 10;
        else
            a = atoi(protocol.substr(2, 1).c_str());

        if (protocol.substr(3, 1) == "f")
            b = 15;
        else if (protocol.substr(3, 1) == "e")
            b = 14;
        else if (protocol.substr(3, 1) == "d")
            b = 13;
        else if (protocol.substr(3, 1) == "c")
            b = 12;
        else if (protocol.substr(3, 1) == "b")
            b = 11;
        else if (protocol.substr(3, 1) == "a")
            b = 10;
        else
            b = atoi(protocol.substr(3, 1).c_str());
        return ((a * 16) + (b * 1));
    }
}

string Skip_list::Convert_Dec_Binary1(string temp)
{
    string result = "";
    int size = 0;
    vector <string> binary;
    binary.clear();
    if (temp[0] == '@')
        temp.erase(0, 1);
    char* ch = new char[150];
    unsigned int k;
    for (k = 0;k < temp.size();k++)
        ch[k] = temp[k];
    ch[k] = '\0';
    char* next_token1 = NULL;
    char* p = strtok_r(ch, ".", &next_token1);
    do
    {
        str = "";
        int n = atoi(p);

        decimal_to_binary1(n);
        if (str.size() == 0)
            binary.push_back("0");
        else
            binary.push_back(str);

    } while ((p = strtok_r(NULL, ".", &next_token1)));

    int i = temp.find("/");
    string s = temp.substr(i + 1, temp.size());
    for (k = 0;k < s.size();k++)
        ch[k] = s[k];
    ch[k] = '\0';
    size = atoi(ch);

    int d = size / 8;
    unsigned int r = size % 8;

    if (d == 0)
    {
        //start of extended
        k = 8 - binary[0].size();
        for (unsigned int i = 0;i < k;i++)
            binary[0].insert(0, "0");
        binary[0] = binary[0].substr(0, r);
        //end of extended
        result = binary[0];
    }
    else if (d == 1)
    {
        int k = 8 - binary[0].size();
        for (int i = 0;i < k;i++)
            binary[0].insert(0, "0");
        //start of extended
        k = 8 - binary[1].size();
        for (int i = 0;i < k;i++)
            binary[1].insert(0, "0");
        binary[1] = binary[1].substr(0, r);
        //end of extended
        result = binary[0];
        result += binary[1];
    }
    else if (d == 2)
    {
        int k = 8 - binary[0].size();
        for (int i = 0;i < k;i++)
            binary[0].insert(0, "0");
        k = 8 - binary[1].size();
        for (int i = 0;i < k;i++)
            binary[1].insert(0, "0");
        //start of extended
        k = 8 - binary[2].size();
        for (int i = 0;i < k;i++)
            binary[2].insert(0, "0");
        binary[2] = binary[2].substr(0, r);
        //end of extended

        result = binary[0];
        result += binary[1];
        result += binary[2];
    }
    else if (d == 3)
    {
        int k = 8 - binary[0].size();
        for (int i = 0;i < k;i++)
            binary[0].insert(0, "0");
        k = 8 - binary[1].size();
        for (int i = 0;i < k;i++)
            binary[1].insert(0, "0");
        k = 8 - binary[2].size();
        for (int i = 0;i < k;i++)
            binary[2].insert(0, "0");
        //start of extended
        k = 8 - binary[3].size();
        for (int i = 0;i < k;i++)
            binary[3].insert(0, "0");
        binary[3] = binary[3].substr(0, r);
        //end of extended
        result = binary[0];
        result += binary[1];
        result += binary[2];
        result += binary[3];
    }
    else if (d == 4)
    {
        int k = 8 - binary[0].size();
        for (int i = 0;i < k;i++)
            binary[0].insert(0, "0");
        k = 8 - binary[1].size();
        for (int i = 0;i < k;i++)
            binary[1].insert(0, "0");
        k = 8 - binary[2].size();
        for (int i = 0;i < k;i++)
            binary[2].insert(0, "0");
        k = 8 - binary[3].size();
        for (int i = 0;i < k;i++)
            binary[3].insert(0, "0");
        result = binary[0];
        result += binary[1];
        result += binary[2];
        result += binary[3];
    }
    return result;


}

void Skip_list::decimal_to_binary1(unsigned int decimal)
{
    if (decimal == 0) //ground  state
    {
        return;
    }
    else
    {
        decimal_to_binary1(decimal / 2);
        string buff;
        //itoa(decimal%2,buff,10);
        ostringstream temp;
        temp << decimal % 2;
        buff = temp.str();
        str += buff;
    }
}

unsigned int  Skip_list::binary_to_decimal1(string tempBound)
{
    unsigned int boundDecimal = 0;
    for (int i = 1; i <= 32; i++)
    {
        if (tempBound[32 - i] == '1')
            boundDecimal += (unsigned int)(pow(2, i - 1));
    }
    return boundDecimal;
}

void Skip_list::getLowerUpperBound1()
{
    string tempLowerBound, tempUpperBound;
    for (unsigned int i = 0; i < srcip.size(); i++)
    {
        tempLowerBound = srcip[i].perfix;
        tempUpperBound = srcip[i].perfix;
        for (unsigned int j = 0; j < (32 - srcip[i].perfix.size()); j++)
        {
            tempLowerBound += "0";
            tempUpperBound += "1";
        }
        srcip[i].low = binary_to_decimal1(tempLowerBound);
        srcip[i].high = binary_to_decimal1(tempUpperBound);

        tempLowerBound = desip[i].perfix;
        tempUpperBound = desip[i].perfix;
        for (unsigned int j = 0; j < (32 - desip[i].perfix.size()); j++)
        {
            tempLowerBound += "0";
            tempUpperBound += "1";
        }
        desip[i].low = binary_to_decimal1(tempLowerBound);
        desip[i].high = binary_to_decimal1(tempUpperBound);
    }
}

void Skip_list::insertips(unsigned long searchKey, string newValue) {
    // reassign if node exists 
    Skip_Node* x = nullptr;
    x = findips(searchKey);
    if (x) {
        x->value = newValue;
        return;
    }

    // vector of pointers that needs to be updated to account for the new node
    vector<Skip_Node*> update(headips->forward);
    unsigned int currentMaximum = nodeLevel(headips->forward);
    x = headips;
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    // search the list 
    for (unsigned int i = currentMaximum; i-- > 0;) {

        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILips") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i) {
                x = x->forward[i];
            }
            else {
                cout << "insertips skipping x " << i << endl;
            }
        }
        if (update.size() > i) {
            update[i] = x;
        }
        else {
            cout << "insertips skipping x " << i << endl;
        }
    }
    if (x->forward.size() > 0) {
        x = x->forward[0];
    }

    // create new node
    int newNodeLevel = 1;
    if (x->key != searchKey) {

        newNodeLevel = randomLevel();
        int currentLevel = nodeLevel(update);

        if (newNodeLevel > currentLevel) {

            for (int i = currentLevel + 1; i < newNodeLevel; ++i) {

                update[i] = headips;
            }
        }
        x = makeNode(searchKey, newValue, newNodeLevel);
        for (unsigned int i = 0; i < srcip.size();i++) {
            if ((searchKey >= srcip[i].low) && (searchKey <= srcip[i].high))
                x->Rules[i] = 1;
        }

        // connect pointers of predecessors and new node to successors
        for (int i = 0; i < newNodeLevel; ++i) {

            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
        skiplistMemorySpacesrc += sizeof(x->key) + sizeof(x->Rules) + sizeof(x->forward);
        node++;
    }
}

void Skip_list::insertipd(unsigned long  searchKey, string newValue) {
    // reassign if node exists 
    Skip_Node* x = nullptr;
    x = findipd(searchKey);
    if (x) {
        x->value = newValue;
        return;
    }

    // vector of pointers that needs to be updated to account for the new node
    vector<Skip_Node*> update(headipd->forward);
    unsigned int currentMaximum = nodeLevel(headipd->forward);
    x = headipd;
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    // search the list 
    for (unsigned int i = currentMaximum; i-- > 0;) {

        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILipd") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i) {
                x = x->forward[i];
            }
        }
        update[i] = x;
    }
    if (x->forward.size() > 0) {
        x = x->forward[0];
    }

    // create new node
    int newNodeLevel = 1;
    if (x->key != searchKey) {

        newNodeLevel = randomLevel();
        int currentLevel = nodeLevel(update);
        // 
        if (newNodeLevel > currentLevel) {

            for (int i = currentLevel + 1; i < newNodeLevel; ++i) {

                update[i] = headipd;
            }

            currentLevel = newNodeLevel;
        }
        x = makeNode(searchKey, newValue, newNodeLevel);
        for (unsigned int i = 0; i < srcip.size();i++) {
            if ((searchKey >= desip[i].low) && (searchKey <= desip[i].high))
                x->Rules[i] = 1;
        }
        skiplisMemorySpacedes += sizeof(x->key) + sizeof(x->Rules) + sizeof(x->forward);
        node++;

        // connect pointers of predecessors and new node to successors
        for (int i = 0; i < newNodeLevel; ++i) {

            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

void Skip_list::insertprts(unsigned long searchKey, string newValue) {
    // reassign if node exists 
    Skip_Node* x = nullptr;
    x = findprts(searchKey);
    if (x) {
        x->value = newValue;
        return;
    }

    // vector of pointers that needs to be updated to account for the new node
    vector<Skip_Node*> update(headprts->forward);
    unsigned int currentMaximum = nodeLevel(headprts->forward);
    x = headprts;
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    // search the list 

    for (unsigned int i = currentMaximum; i-- > 0;) {
        while ((x->forward.size() > i && x->forward[i] != nullptr && x->forward[i]->value != "NILprts") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i) {
                x = x->forward[i];
            }
        }
        if (update.size() > i) {
            update[i] = x;
        }
        else {
            cout << "insertprts skipping x " << i << endl;
        }
    }

    if (x->forward.size() > 0) {
        x = x->forward[0];
    }

    // create new node
    int newNodeLevel = 1;

    if (x->key != searchKey) {

        newNodeLevel = randomLevel();
        int currentLevel = nodeLevel(update);

        if (newNodeLevel > currentLevel) {

            for (int i = currentLevel + 1; i < newNodeLevel; ++i) {
                if (update.size() > i) {
                    update[i] = headprts;
                }
                else {
                    cout << "insertprts skipping headprts " << i << endl;
                }
            }
        }


        x = makeNode(searchKey, newValue, newNodeLevel);

        for (unsigned int i = 0; i < srcip.size();i++) {

            if ((searchKey >= srcip[i].src_port_low) && (searchKey <= srcip[i].src_port_high)) {

                if (x->Rules.size() > i) {
                    x->Rules[i] = 1;
                }
                else {
                    cout << "Trying to update rule " << i << ", while rules count = " << x->Rules.size() << endl;
                }
            }
        }

        skiplisMemorySpacesp += sizeof(x->key) + sizeof(x->Rules) + sizeof(x->forward);
        node++;

        // connect pointers of predecessors and new node to successors
        for (int i = 0; i < newNodeLevel; ++i) {

            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

void Skip_list::insertprtd(unsigned long searchKey, string newValue) {
    // reassign if node exists 
    Skip_Node* x = nullptr;
    x = findprtd(searchKey);
    if (x) {
        x->value = newValue;
        return;
    }

    // vector of pointers that needs to be updated to account for the new node
    vector<Skip_Node*> update(headprtd->forward);
    unsigned int currentMaximum = nodeLevel(headprtd->forward);
    x = headprtd;
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    // search the list 
    for (unsigned int i = currentMaximum; i-- > 0;) {

        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILprtd") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i) {
                x = x->forward[i];
            }
        }
        update[i] = x;
    }
    if (x->forward.size() > 0) {
        x = x->forward[0];
    }

    // create new node
    int newNodeLevel = 1;
    if (x->key != searchKey) {

        newNodeLevel = randomLevel();
        int currentLevel = nodeLevel(update);

        if (newNodeLevel > currentLevel) {

            for (int i = currentLevel + 1; i < newNodeLevel; ++i) {

                update[i] = headprtd;
            }
        }
        x = makeNode(searchKey, newValue, newNodeLevel);
        for (unsigned int i = 0; i < srcip.size();i++) {
            if ((searchKey >= desip[i].src_port_low) && (searchKey <= desip[i].src_port_high))
                x->Rules[i] = 1;
        }
        skiplisMemorySpacedp += sizeof(x->key) + sizeof(x->Rules) + sizeof(x->forward);
        node++;

        // connect pointers of predecessors and new node to successors
        for (int i = 0; i < newNodeLevel; ++i) {

            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

void Skip_list::insertp(unsigned long searchKey, string newValue) {
    // reassign if node exists 
    Skip_Node* x = nullptr;
    x = findp(searchKey);
    if (x) {
        x->value = newValue;
        return;
    }

    // vector of pointers that needs to be updated to account for the new node
    vector<Skip_Node*> update(headp->forward);
    unsigned int currentMaximum = nodeLevel(headp->forward);
    x = headp;
    if (currentMaximum > maxLevel)
        currentMaximum = maxLevel;
    // search the list 
    for (unsigned int i = currentMaximum; i-- > 0;) {

        while ((x->forward[i] != nullptr && x->forward[i]->value != "NILp") && x->forward[i]->key < searchKey) {

            if (x->forward.size() > i) {
                x = x->forward[i];
            }
        }
        update[i] = x;
    }
    if (x->forward.size() > 0) {
        x = x->forward[0];
    }

    // create new node
    int newNodeLevel = 1;
    if (x->key != searchKey) {

        newNodeLevel = randomLevel();
        int currentLevel = nodeLevel(update);

        if (newNodeLevel > currentLevel) {

            for (int i = currentLevel + 1; i < newNodeLevel; ++i) {

                update[i] = headp;
            }
        }
        x = makeNode(searchKey, newValue, newNodeLevel);
        for (unsigned int i = 0; i < srcip.size();i++) {
            if (searchKey == srcip[i].protocol)
                x->Rules[i] = 1;
        }
        skiplisMemorySpacepro += sizeof(x->key) + sizeof(x->Rules) + sizeof(x->forward);
        node++;

        // connect pointers of predecessors and new node to successors
        for (int i = 0; i < newNodeLevel; ++i) {

            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

#pragma endregion

#pragma region main function

void handler(int sig) {
    void* array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

Skip_list s, v;
// New - "/content" "dataset/acl2/acl2_8k" "acl2-8k.txt" "acl8k_header128"
int main(int argc, char* argv[])
{
    signal(SIGSEGV, handler);   // install our handler
    // Arguments handling
    std::string baseFolder(argv[1]);
    std::string datasetFolder(argv[2]);

    std::string fullDatasetFolder = baseFolder + "/" + datasetFolder;

    std::string filterName(argv[3]);
    std::string traceName(argv[4]);

    std::string outputsFolder = fullDatasetFolder + "/" + traceName;

    // Inputs
    ifstream MyFilters;
    ifstream MyFilters_trace;

    std::string filterFileName = fullDatasetFolder + "/" + filterName;
    std::string traceFileName = fullDatasetFolder + "/" + traceName + "/skewness 0.txt";

    // Output
    ofstream finallmaching;
    ofstream searchFile;
    ofstream timeFile;
    ofstream memorry;

    ofstream analysisCsv;
    std::string outotputFileName = baseFolder + "/" + "analysis.csv";
    bool fileExisits = file_exists(outotputFileName);

    analysisCsv.open(outotputFileName, std::ios::app);
    // colums / heaedr - if fiel is not existing
    if (!fileExisits) {
        cout << "File does not exists creating header" << endl;
        analysisCsv << "filter" << "," << "dataset" << "," << "clasification_time" << "," << "memory_access" << "," << "memory_consumption" << endl;
    }

    vector<packetHeaderbystamp> vectorPacketstamp;
    vector<Skip_Node*> l;
    vector<int> f;

    totallMemorryAccess = 0;
    skiplistMemorySpacesrc = 0;
    skiplisMemorySpacedes = 0;
    skiplisMemorySpacesp = 0;
    skiplisMemorySpacedp = 0;
    skiplisMemorySpacepro = 0;
    node = 0;
    MemAccess = 0;
    finallcreattime = 0.0;
    int totallMemorryAccessp = 0;
    void printSkipList();

    clock_t start;
    clock_t end;

    clock_t startrulesrc;
    clock_t endrulesrc;

    clock_t startruledes;
    clock_t endruledes;

    clock_t startrulesp;
    clock_t endrulesp;

    clock_t startruledp;
    clock_t endruledp;

    clock_t startrulepro;
    clock_t endrulepro;
    // ************
    clock_t startsearchsrc;
    clock_t endrsearchsrc;

    clock_t startsearchdes;
    clock_t endrsearchdes;

    clock_t startsearchsp;
    clock_t endrsearchsp;

    clock_t startsearchdp;
    clock_t endrsearchdp;

    clock_t startsearchpro;
    clock_t endrsearchpro;
    //**************
    long double sumtimrulesp = 0.0;
    long double sumtimruledp = 0.0;
    long double sumtimrulesrc = 0.0;
    long double sumtimruledes = 0.0;
    long double sumtimrulepro = 0.0;
    MemAccesssrc = 0;
    MemAccessdes = 0;
    MemAccesssp = 0;
    MemAccessdp = 0;
    MemAccesspro = 0;

    unsigned long b = 0;

    cout << "Reading Rules From file: ";
    cout << filterFileName << endl;

    MyFilters.open(filterFileName, ios::in);
    if (!MyFilters)
    {
        cerr << "can not open the input File .....";
        exit(0);
    }
    // read rules az file classbench va tabdile anha be formate monaseb
    while (!MyFilters.eof())
    {
        string a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 = "";
        MyFilters >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10;
        Rule tempRules;
        Rule tempRuled;
        if (!a1.empty())
        {
            tempRules.perfix = v.Convert_Dec_Binary1(a1);
            tempRules.src_port_low = stoi(a3);
            tempRules.src_port_high = stoi(a5);
            tempRules.src_perfix_length = v.get_perfix_lenght1(a1);

            tempRuled.perfix = v.Convert_Dec_Binary1(a2);
            tempRuled.dest_port_low = stoi(a6);
            tempRuled.dest_port_high = stoi(a8);
            tempRuled.src_perfix_length = v.get_perfix_lenght1(a2);

            tempRules.protocol = v.HextoInt1(a9);

            srcip.push_back(tempRules);
            desip.push_back(tempRuled);
        }
    }
    MyFilters.close();
    //**************************************          be dast avardane  upper va lower bound      *************************************************************************************************

    v.getLowerUpperBound1();
    //*****************************************************************************************************************************************
    cout << "Adding " << srcip.size() << " rules to skip lists" << endl;
    //*************************************************** reordering *************************************************************************
    v.rule_bubbleSort1();
    for (int i = 0; i < srcip.size();i++)
    {
        startrulesrc = clock();
        v.insertips(srcip[i].low, "srcip");
        v.insertips(srcip[i].high, "srcip");
        endrulesrc = clock();
        long double function_timerule = 1000 * (long double)(endrulesrc - startrulesrc) / (long double)CLOCKS_PER_SEC;
        sumtimrulesrc = function_timerule;

        startruledes = clock();
        v.insertipd(desip[i].low, "desip");
        v.insertipd(desip[i].high, "desip");
        endruledes = clock();
        long double function_timeruledes = 1000 * (long double)(endruledes - startruledes) / (long double)CLOCKS_PER_SEC;
        sumtimruledes = function_timeruledes;

        startrulesp = clock();
        v.insertprts(srcip[i].src_port_low, "srcport");
        v.insertprts(srcip[i].src_port_high, "srcport");
        endrulesp = clock();

        long double function_timerulesp = 1000 * (long double)(endrulesp - startrulesp) / (long double)CLOCKS_PER_SEC;
        sumtimrulesp = function_timerulesp;

        startruledp = clock();
        v.insertprtd(desip[i].dest_port_low, "destport");
        v.insertprtd(desip[i].dest_port_high, "destport");

        endruledp = clock();
        long double function_timeruledp = 1000 * (long double)(endruledp - startruledp) / (long double)CLOCKS_PER_SEC;
        sumtimruledp = function_timeruledp;

        startrulepro = clock();
        v.insertp(srcip[i].protocol, "protocol");
        endrulepro = clock();

        long double function_timerulep = 1000 * (long double)(endrulepro - startrulepro) / (long double)CLOCKS_PER_SEC;
        sumtimrulepro = function_timerulep;
        long double	 sumtimcreat = max(sumtimrulesrc, max(sumtimruledes, max(sumtimrulesp, max(sumtimruledp, sumtimrulepro))));
        finallcreattime += sumtimcreat;
        sumtimcreat = 0.0;
    }

    cout << "Rules added." << endl;
    MemAccess = 0;

    //***********************************************************************************************************************************
    // memorry << "\n total memorry space:" << (skiplistMemorySpacesrc + skiplisMemorySpacedes + skiplisMemorySpacesp + skiplisMemorySpacedp + skiplisMemorySpacepro);
    // cout << "\n total memorry space:" << (skiplistMemorySpacesrc + skiplisMemorySpacedes + skiplisMemorySpacesp + skiplisMemorySpacedp + skiplisMemorySpacepro) << endl;

    int q = 0;
    unsigned long long Sum = 0, Min = 999999999999, Max = 0, Mintotal = 999999999999, Maxtotal = 0;
    long double function_timemax = 0.0;
    long double function_timemin = 9999999999.0;

    unsigned long long sumclasssplay = 0;

    long double searchTimeSum = 0;
    int totallaccessclass = 0;
    Timetotallclassification = 0;
    long double timeEshterakgiri = 0.0;


    //****************************************************************** cllassifay packetha dar barabare rules**************************************		
    ifstream classbenchHeaders;
    cout << "Reading traces from " << traceFileName << endl;

    classbenchHeaders.open(traceFileName, ios::in);
    if (!classbenchHeaders)
    {
        cerr << "can not open the input File .....";
        exit(0);
    }

    while (!classbenchHeaders.eof())
    {
        unsigned long n = 0, m = 0, w = 0, e = 0, r = 0, s = 0, t = 0, j = 0, k = 0;
        long double zq = 0.0;
        classbenchHeaders >> w >> e >> r >> s >> t >> j >> k;
        if (classbenchHeaders.eof())
            break;
        packetHeaderbystamp packetstamp;
        packetstamp.srcAddress = w;
        packetstamp.destAddress = e;
        packetstamp.srcPort = r;
        packetstamp.destPort = s;
        packetstamp.protocol = t;
        packetstamp.unknown_field = j;
        packetstamp.rule_number = k;

        packetstamp.rulesMatched.clear();
        vectorPacketstamp.push_back(packetstamp);
    }

    cout << "Classifying..." << endl;
    classbenchHeaders.close();
    long double sumtimesearchsrc = 0.0;
    long double sumtimesearchdes = 0.0;
    long double sumtimesearchsp = 0.0;
    long double sumtimesearchdp = 0.0;
    long double sumtimesearchpro = 0.0;

    bitset<sizeOfRuleSet> Listsrc, Listdes, listsp, Listdp, Listpro, Listfinall;
    bool flag = false;
    MemAccess = 0;

    for (int i = 0; i < vectorPacketstamp.size();i++)
    {
        long double sumt = 0.0;
        startsearchsrc = clock();
        Skip_Node* root_S = v.findips(vectorPacketstamp[i].srcAddress);
        endrsearchsrc = clock();
        MemAccesssrc = MemAccess;
        //Sum+=MemAccess;
        MemAccess = 0;

        long double sumtimcllass = 1000 * (long double)(endrsearchsrc - startsearchsrc) / (long double)CLOCKS_PER_SEC;

        sumtimesearchsrc = sumtimcllass;
        sumtimcllass = 0.0;

        //*************************************************
        startsearchdes = clock();
        Skip_Node* root_D = v.findipd(vectorPacketstamp[i].destAddress);
        endrsearchdes = clock();
        MemAccessdes = MemAccess;
        // Sum=MemAccess;
        MemAccess = 0;
        long double sumtimcllass2 = 1000 * (long double)(endrsearchdes - startsearchdes) / (long double)CLOCKS_PER_SEC;
        sumtimesearchdes = sumtimcllass2;
        sumtimcllass2 = 0.0;

        ///*******************************************************************
        startsearchsp = clock();
        Skip_Node* root_sport = v.findprts(vectorPacketstamp[i].srcPort);
        endrsearchsp = clock();

        MemAccesssp = MemAccess;
        //Sum=MemAccess;
        MemAccess = 0;
        long double sumtimcllass3 = 1000 * (long double)(endrsearchsp - startsearchsp) / (long double)CLOCKS_PER_SEC;
        sumtimesearchsp = sumtimcllass3;
        sumtimcllass3 = 0.0;

        //**************************************************************************
        startsearchdp = clock();
        Skip_Node* root_dport = v.findprtd(vectorPacketstamp[i].destPort);
        endrsearchdp = clock();
        MemAccessdp = MemAccess;
        // Sum=MemAccess;
        MemAccess = 0;

        long double sumtimcllass4 = 1000 * (long double)(endrsearchdp - startsearchdp) / (long double)CLOCKS_PER_SEC;

        sumtimesearchdp = sumtimcllass4;
        sumtimcllass4 = 0.0;

        //****************************************************
        startsearchpro = clock();
        Skip_Node* root_protocol = v.findp(vectorPacketstamp[i].protocol);
        endrsearchpro = clock();
        MemAccesspro = MemAccess;
        // Sum=MemAccess;
        MemAccess = 0;

        long double sumtimcllass5 = 1000 * (long double)(endrsearchpro - startsearchpro) / (long double)CLOCKS_PER_SEC;
        sumtimesearchpro = sumtimcllass5;
        sumtimcllass5 = 0.0;

        // eshterak giri byne rulehaye bedast amade az tree haye mokhtalef		 
        clock_t startpacketesh = clock();
        Listfinall = Listsrc & Listdes & listsp & Listdp & Listpro;

        for (int p = 0; p < Listfinall.size();p++)
        {
            if (Listfinall.test(p)) {
                flag = true;
                break;
            }
        }
        clock_t endpacketesh = clock();
        long double function_timepacket = 1000 * (long double)(endpacketesh - startpacketesh) / (long double)CLOCKS_PER_SEC;
        timeEshterakgiri += function_timepacket;
        function_timepacket = 0;
        sumt = max(sumtimesearchsrc, max(sumtimesearchdes, max(sumtimesearchsp, max(sumtimesearchdp, sumtimesearchpro))));
        Timetotallclassification += sumt;
        sumtimcllass = 0.0;

        // set kardan time be 0 baraye packet badi.
        if (sumt > function_timemax)
            function_timemax = sumt;
        if (sumt < function_timemin)
            function_timemin = sumt;

        sumt = 0.0;
        sumtimesearchsrc = 0.0;
        sumtimesearchdes = 0.0;
        sumtimesearchsp = 0.0;
        sumtimesearchdp = 0.0;
        sumtimesearchpro = 0.0;

        q++;

        int totalclasification = max(MemAccesssrc, max(MemAccessdes, max(MemAccesssp, max(MemAccessdp, MemAccesspro))));
        Sum = totalclasification;

        totallMemorryAccess += totalclasification;
        if (Sum > Max)
            Max = Sum;
        if (Sum < Min)
            Min = Sum;


        MemAccess = 0;

        Sum = 0;

        flag = false;
    }

    double totalMemoryConsumption = (skiplistMemorySpacesrc + skiplisMemorySpacedes + skiplisMemorySpacesp + skiplisMemorySpacedp + skiplisMemorySpacepro);
    analysisCsv << filterName << "," << traceName << "," << Timetotallclassification << "," << totallMemorryAccess << "," << totalMemoryConsumption << endl;
    analysisCsv.close();

    cout << "Finish!" << endl;
    cout << "total nodes: " << node << endl;
    cout << endl << endl;

    return 0;
}

#pragma endregion