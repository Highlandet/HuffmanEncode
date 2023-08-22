#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Node
{
public:
    int parent; //Parent of Node
    int left; //Left child of Node, for leaves - =-1
    int right; //Right child of Node, for leaves - =-1
    int probability; //Node's value, for leaves - probability of appearing in the text
    char symbol;
    string code;
    bool merged; //If the node has parent it's "true"

    Node(int probability, char symbol = '.')
    {
        this->parent = -1; //default
        this->right = -1; //default
        this->left = -1; //default
        this->probability = probability;
        this->symbol = symbol;
        this->code = ""; //default empty code
        this->merged = false; //default - isn't merged
    }
    void SetChildren(int left, int right)
    {
        this->left = left;
        this->right = right;
    }
    void SetParent(int parent)
    {
        this->parent = parent;
    }
    void Merging()
    {
        this->merged = true;
    }
    bool IsMerged()
    {
        return this->merged;
    }
};

string Creating()
{
    string str;
    getline(cin, str);
    return str;
}

map<char, int> Writing(string str)
{
    map<char, int> Dictio; //Table of symbols and their probabilities
    for (auto& sym : str)
        if (sym != ' ')
        {
            if (Dictio.count(sym) != 0) //If symbol has been already added into the table
                Dictio[sym]++;
            else Dictio.insert(make_pair(sym, 1)); //If symbol isn't added into the table
        }
    return Dictio;
}

vector<Node> Noding(map<char, int> Dictio)
{
    vector<Node> NodeList;
    for (auto& item : Dictio)
        NodeList.push_back(Node(item.second, item.first));
    return NodeList;
}

map<char, string> HuffmanTree(vector<Node> NodeList)
{
    int NotSeen = NodeList.size(); //Not merged nodes
    for (; NotSeen != 1; NotSeen--)
    {
        //Searching nodes to merge
        int max = 0;
        for (int i = 0; i < NodeList.size(); i++)
            if (NodeList[max].probability <= NodeList[i].probability)
                max = i;
        //min1 & min2 - these nodes
        int min1 = max;
        int min2 = max;
        if (NodeList[min2].probability < NodeList[min1].probability
            && NodeList[min2].merged == false)
        {
            int tmp = min1;
            min1 = min2;
            min2 = tmp;
        }
        for (int i = 0; i < NodeList.size(); i++)
        {
            if (NodeList[i].probability <= NodeList[min1].probability
                && NodeList[i].merged == false)
            {
                min2 = min1;
                min1 = i;
            }
            else if (NodeList[i].probability <= NodeList[min2].probability
                && NodeList[i].merged == false)
                min2 = i;
        }

        //Creating parent of min1 & min2 
        NodeList.push_back(Node(NodeList[min1].probability + NodeList[min2].probability));
        //Setting children for parent
        NodeList[NodeList.size() - 1].SetChildren(min2, min1);
        //Setting parent for children
        NodeList[min1].SetParent(NodeList.size() - 1);
        NodeList[min2].SetParent(NodeList.size() - 1);
        //Changing the merged-status for min1 & min2
        NodeList[min1].Merging();
        NodeList[min2].Merging();
    }
    map<char, string> CodeTable;  //Dictionary for codes
    NodeList[NodeList.size() - 1].code = "";  //Default empty code for the root

    //Encoding process
    for (int i = NodeList.size() - 2; i >= 0; i--)
    {
        if (NodeList[NodeList[i].parent].left == i)
        {
            /*cout << "Node *" << NodeList[i].symbol << "* is left son, ";*/
            NodeList[i].code = NodeList[NodeList[i].parent].code + "0";
        }
        if (NodeList[NodeList[i].parent].right == i)
        {
            /*cout << "Node *" << NodeList[i].symbol << "* is right son, ";*/
            NodeList[i].code = NodeList[NodeList[i].parent].code + "1";
        }
        //Printing info about node
        cout << "Node's symbol: " << NodeList[i].symbol << ", Node's probability: " << NodeList[i].probability
            << ", Node's code: " << NodeList[i].code << endl;
        //Adding into the Table of codes
        if (NodeList[i].symbol != '.')
            CodeTable.insert(make_pair(NodeList[i].symbol, NodeList[i].code));
    }
    cout << endl;
    return CodeTable;
}
string Encode(string Initial, map<char, string> CodeTable)
{
    //Encoding input-string
    string ReadyFor = "";
    for (auto& letter : Initial)
    {
        if (letter != ' ')
            ReadyFor += CodeTable[letter];
        else ReadyFor += " ";
    }
    return ReadyFor;
}

int32_t main(const int32_t argc, const char* const argv[])
{
    string str = Creating();
    map<char, int> Dictionary = Writing(str);
    vector<Node> NodeList = Noding(Dictionary);
    cout << Encode(str, HuffmanTree(NodeList));
    return EXIT_SUCCESS;
}