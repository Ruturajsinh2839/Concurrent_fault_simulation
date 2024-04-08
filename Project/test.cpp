#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct Gate // the structure to store the data of gate
{
    string type;
    string g_name;
    vector<string> inputs;
    vector<string> outputs;
};

struct Module // to store the whole circuit's basic information
{
    vector<string> inputs;
    vector<string> outputs;
    vector<Gate> gates;
};

struct Node // to store the node (primary, gate)
{
    string name;
    string type;
    Node *next = NULL;
};

Module parseVerilog(const string &filename) // function to fatch Verilog file and return the Module
{
    ifstream file(filename);
    string line;
    regex inputRegex(R"(input\s+([a-zA-Z0-9_,\s]+);)");
    regex wireRegex(R"(wire\s+([a-zA-Z0-9_,\s]+);)");
    regex outputRegex(R"(output\s+([a-zA-Z0-9_,\s]+);)");
    regex nandRegex(R"(nand\s+([()a-zA-Z0-9_,\s]+);)");
    regex andRegex(R"(and\s+([()a-zA-Z0-9_,\s]+);)");
    regex orRegex(R"(or\s+([()a-zA-Z0-9_,\s]+);)");
    regex xorRegex(R"(xor\s+([()a-zA-Z0-9_,\s]+);)");
    regex xnorRegex(R"(xnor\s+([()a-zA-Z0-9_,\s]+);)");
    regex norRegex(R"(nor\s+([()a-zA-Z0-9_,\s]+);)");
    regex notRegex(R"(not\s+([()a-zA-Z0-9_,\s]+);)");
    regex bufRegex(R"(buf\s+([()a-zA-Z0-9_,\s]+);)");
    
    Module module;

    while (getline(file, line))
    {
        smatch match;

        if (regex_search(line, match, inputRegex))
        {
            string inputsStr = match[1].str();

            stringstream inputsStream(inputsStr);
            string input;
            while (getline(inputsStream, input, ','))
            {
                module.inputs.push_back(input);
            }
        }
        else if (regex_search(line, match, wireRegex))
        {
            string inputsStr = match[1].str();
            stringstream inputsStream(inputsStr);
            string input;
            while (getline(inputsStream, input, ','))
            {
                module.outputs.push_back(input);
            }
        }
        else if (regex_search(line, match, outputRegex))
        {
            string outputsStr = match[1].str();
            stringstream outputsStream(outputsStr);
            string output;
            while (getline(outputsStream, output, ','))
            {
                module.outputs.push_back(output);
            }
        }
        else if (regex_search(line, match, nandRegex))
        {
            Gate gate;
            gate.type = "nand";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, andRegex))
        {
            Gate gate;
            gate.type = "and";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, xnorRegex))
        {
            Gate gate;
            gate.type = "xnor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, xorRegex))
        {
            Gate gate;
            gate.type = "xor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, norRegex))
        {
            Gate gate;
            gate.type = "nor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, orRegex))
        {
            Gate gate;
            gate.type = "or";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, notRegex))
        {
            Gate gate;
            gate.type = "not";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, bufRegex))
        {
            Gate gate;
            gate.type = "buf";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
    }

    return module;
}

void connectNodes(vector<Node *> &nodes, const Module &module) // to make connections between all nodes
{
    unordered_map<string, int> mp;
    int ind = 0;
    for (auto it : module.inputs)
    {
        Node *ne = new Node;
        ne->name = it;
        ne->type = "INPUT";
        ne->next = NULL;
        nodes.push_back(ne);
        mp[it] = ind++;
    }
    for (auto it : module.outputs)
    {
        Node *ne = new Node;
        ne->name = it;
        ne->next = NULL;
        nodes.push_back(ne);
        mp[it] = ind++;
    }
    for (auto it : module.gates)
    {
        string type_g = it.type;
        vector<string> ip = it.inputs;
        string op = it.outputs[0];
        for (auto it1 : ip)
        {
            Node *node = nodes[mp[it1]];
            Node *ne = new Node();
            ne->name = op;
            ne->type = type_g;
            nodes[mp[op]]->type = type_g;
            ne->next = NULL;
            while (node->next != NULL)
                node = node->next;
            node->next = ne;
        }
    }
}

void displaycircuit(vector<Node *> &nodes) // to display the circuit
{

    for (auto it : nodes)
    {
        Node *node = it;
        while (node)
        {
            cout << node->type << " ";
            cout << node->name << " -> ";
            node = node->next;
        }
        cout << "NULL" << endl;
    }
}

// fuctions to compute the values of logic gate
/--------------------------------------------------------------/

bool computeXORGate(const vector<string> &inputs, unordered_map<string, int> &mp)
{
    bool output = false;
    for (const auto &input : inputs)
    {
        output = output ^ mp[input];
    }
    return output;
}
bool computeANDGate(const vector<string> &inputs, unordered_map<string, int> &mp)
{
    bool output = true;
    for (const auto &input : inputs)
    {
        output = output && mp[input];
    }
    return output;
}
bool computeORGate(const vector<string> &inputs, unordered_map<string, int> &mp)
{
    bool output = false;
    for (const auto &input : inputs)
    {
        output = output || mp[input];
    }
    return output;
}
bool computeNOTGate(bool inputs)
{
    return !inputs;
}
bool computebufGate(bool inputs)
{
    return inputs;
}
/--------------------------------------------------------------/

void compute(unordered_map<string, int> &mp, vector<Gate> &ge) // will compute the outputs of the circuit
{
    for (auto it : ge)
    {
        vector<string> inp = it.inputs;
        string typ = it.type;
        string st = it.outputs[0];

        if (typ == "or")
        {
            bool ans = computeORGate(inp, mp);
            mp[st] = ans;
        }
        if (typ == "nor")
        {
            bool ans = computeORGate(inp, mp);
            mp[st] = computeNOTGate(ans);
        }
        if (typ == "and")
        {
            bool ans = computeANDGate(inp, mp);
            mp[st] = ans;
        }
        if (typ == "nand")
        {
            bool ans = computeANDGate(inp, mp);
            mp[st] = computeNOTGate(ans);
        }
        if (typ == "xor")
        {
            bool ans = computeXORGate(inp, mp);
            mp[st] = ans;
        }
        if (typ == "xnor")
        {
            bool ans = computeXORGate(inp, mp);
            mp[st] = computeNOTGate(ans);
        }
        if (typ == "not")
            mp[st] = computeNOTGate(mp[inp[0]]);

        if (typ == "buf")
            mp[st] = computebufGate(mp[inp[0]]);
    }
}

void generateAllBinaryStrings(int n, vector<int> &arr, int i,Module &module,unordered_map<string,int>&mp) // to generate the permutations for input
{
    if (i == n)
    {
        cout << "Inputs: ";
        for (int j = 0; j < n; j++)
            {
                cout << module.inputs[j] << " : " << arr[j] << ", ";
                mp[module.inputs[j]] = arr[j];
            }
            cout << endl;
        compute(mp,module.gates);
            cout << "Outputs: ";
            for (auto it : mp)
                cout << it.first << " : " << it.second << ", ";
            cout << endl;
            cout << endl;
        return;
    }
    arr[i] = 0;
    generateAllBinaryStrings(n, arr, i + 1,module,mp);

    arr[i] = 1;
    generateAllBinaryStrings(n, arr, i + 1,module,mp);
}

/-------------------------------: Driver Function :-----------------------------------/
int main()
{
    string file_name;
    cout << "Enter the file name you want to make circuit: ";
    cin >> file_name;

    Module module = parseVerilog(file_name); // fatching the verilog file
    vector<Node *> nodes;
    connectNodes(nodes, module);
    unordered_map<string, int> mp;

    cout << endl;
    cout << "-------------Circuit Representation-------------" << endl;
    displaycircuit(nodes);
    cout << endl;

    cout << "----------------Input Section------------------" << endl;
    cout << "WHat do you want to do? (1)for manual inputs (2)for permutation inputs (3)Exit :";
    int choise;
    cin >> choise;
    cout << endl;

    if (choise == 2)
    {
        int n = module.inputs.size();
        vector<int> arr(n, 0);
        generateAllBinaryStrings(n, arr, 0,module,mp);
    }
    else if (choise == 1)
    {
        bool want_exit = false;

        while (!want_exit)
        {
            for (auto it : module.inputs)
            {
                cout << "Enter input for " << it << " :";
                int num;
                cin >> num;
                mp[it] = num;
            }
            compute(mp, module.gates);

            cout << "Outputs: ";
            for (auto it : mp)
                cout << it.first << " : " << it.second << ", ";
            cout << endl;

            cout << endl;
            cout << "Do you want to: Continue(Enter 0) or Exit(Enter 1): ";
            cin >> want_exit;
        }
    }
    cout << endl;
    cout << "----------- (: Thank YOU :) ----------------" << endl;
    cout << endl;

    return 0;
}