#include <bits/stdc++.h>
using namespace std;
int No_Output = 0;

struct hash_pair
{

    template <class T1, class T2>

    size_t operator()(const pair<T1, T2> &p) const

    {

        auto hash1 = hash<T1>{}(p.first);

        auto hash2 = hash<T2>{}(p.second);

        if (hash1 != hash2)
        {

            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.

        return hash1;
    }
};

class Gate
{
public:
    string g_type;
    string g_name;
    vector<string> input;
    vector<string> output;
};
class Verilog_File
{
public:
    vector<string> input;
    vector<string> output;
    vector<Gate> gate;
};

class Fault_gate_list
{
public:
    string g_name;
    vector<pair<pair<string, bool>, pair<string, bool>>> fault;
};

void Convert_string_into_word(string input, char del, vector<string> &words)
{
    stringstream ss(input);
    string word;

    while (getline(ss, word, del))
    {
        words.push_back(word);
    }
}

bool Convert_string_present(string input, char del, vector<string> types)
{
    stringstream ss(input);
    string word;

    while (getline(ss, word, del))
    {
        if (find(types.begin(), types.end(), word) != types.end())
            return 1;
        else
            return 0;
    }
}
Verilog_File Convert_file(string file_name)
{
    ifstream file(file_name);
    string line;
    Verilog_File f;
    vector<string> types = {"input", "wire", "output"};
    vector<string> types_gate = {"nand", "and", "or", "xor", "buf", "not", "nor", "xnor"};
    vector<string> words;
    while (getline(file, line, ';'))
    {
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        Convert_string_into_word(line, ' ', words);
        if (words[0] == "//")
        {
            words.clear();
            continue;
        }
        words.clear();

        if (Convert_string_present(line, ' ', types) == 1)
        {
            line.erase(remove(line.begin() + 8, line.end(), ' '), line.end());
            Convert_string_into_word(line, ' ', words);

            if (words[0] == "input")
            {
                string input = words[1];
                words.clear();

                Convert_string_into_word(input, ',', words);

                f.input.insert(f.input.end(), words.begin(), words.end());
            }
            else
            {
                string out = words[0];
                string output = words[1];
                words.clear();
                Convert_string_into_word(output, ',', words);
                if (out == "output")
                {
                    No_Output = words.size();
                }
                f.output.insert(f.output.end(), words.begin(), words.end());
            }
        }
        else if (Convert_string_present(line, ' ', types_gate) == 1)
        {
            Convert_string_into_word(line, '(', words);
            //  cout<<words.size()<<endl;
            string temp = words[0];
            string temp1 = words[1];
            words.clear();
            temp1.erase(remove(temp1.begin(), temp1.end(), ' '), temp1.end());
            temp1.erase(remove(temp1.begin(), temp1.end(), ')'), temp1.end());

            Convert_string_into_word(temp, ' ', words);

            Gate gate;
            gate.g_name = words[1];
            gate.g_type = words[0];

            words.clear();

            Convert_string_into_word(temp1, ',', words);
            gate.input.insert(gate.input.end(), words.begin() + 1, words.end());
            gate.output.push_back(words[0]);

            words.clear();
            f.gate.push_back(gate);
        }

        words.clear();
    }

    file.close();
    return f;
}

// bool compute_nand(vector<string> input, unordered_map<string, int> &mp_in)
// {
//     bool res = false;
//     for (auto i : input)
//     {
//         if (!mp_in[i])
//         {
//             res = !res;
//         }
//     }
//     return !res;
// }

bool compute_and(vector<string> input, unordered_map<string, int> &mp_in)
{
    bool res = true;
    for (auto i : input)
    {
        if (!mp_in[i])
        {
            return false;
        }
        else
        {
            res &= mp_in[i];
        }
    }
    return res;
}

bool compute_or(vector<string> input, unordered_map<string, int> &mp_in)
{
    bool res = false;
    for (auto i : input)
    {
        res |= mp_in[i];
    }
    return res;
}

bool compute_xor(vector<string> input, unordered_map<string, int> &mp_in)
{
    bool res = false;
    for (auto i : input)
    {
        res ^= mp_in[i];
    }
    return res;
}

void compute_CUT(unordered_map<string, int> &mp_in, vector<Gate> gate)
{
    for (auto i : gate)
    {
        if (i.g_type == "nand")
        {
            bool temp = compute_and(i.input, mp_in);
            mp_in[i.output[0]] = !temp;
        }
        else if (i.g_type == "nor")
        {
            bool temp = compute_or(i.input, mp_in);
            mp_in[i.output[0]] = !temp;
        }
        else if (i.g_type == "and")
        {
            bool temp = compute_and(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type == "or")
        {
            bool temp = compute_or(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type == "not")
        {
            bool temp = !mp_in[i.input[0]];
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type == "buf")
        {
            bool temp = mp_in[i.input[0]];
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type == "xor")
        {
            bool temp = compute_xor(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type == "xnFor")
        {
            bool temp = compute_xor(i.input, mp_in);
            mp_in[i.output[0]] = !temp;
        }
    }
}

void concurrent_fault_simulation(unordered_map<string, int> &mp_in, vector<Gate> gate, vector<Fault_gate_list> &fault)
{
    for (auto i : gate)
    {
        Fault_gate_list fault_gate;
        fault_gate.g_name = i.g_name;
        pair<pair<string, bool>, pair<string, bool>> f1;

        if (i.g_type == "nand")
        {

            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_and(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = !temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
            // bool temp = compute_and(i.input, mp_in);
            // mp_in[i.output[0]] = !temp;
        }
        else if (i.g_type == "nor")
        {
            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_or(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = !temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
        }
        else if (i.g_type == "and")
        {
            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_and(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
        }
        else if (i.g_type == "or")
        {
            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_or(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
            // mp_in[i.output[0]] = !mp_in[i.output[0]];
        }
        else if (i.g_type == "not")
        {

            f1.first.first = i.input[0];
            f1.first.second = !mp_in[i.input[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);

            // mp_in[i.output[0]] = !temp;

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
        }
        else if (i.g_type == "buf")
        {
            f1.first.first = i.input[0];
            f1.first.second = !mp_in[i.input[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);

            // mp_in[i.output[0]] = !temp;

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
            
        }
        else if (i.g_type == "xor")
        {
            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_xor(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);

            
        }
        else if (i.g_type == "xnFor")
        {
            for (auto j : i.input)
            {

                f1.first.first = j;
                mp_in[j] = !mp_in[j];
                f1.first.second = mp_in[j];
                bool temp = compute_xor(i.input, mp_in);
                f1.second.first = i.output[0];
                f1.second.second = !temp;
                fault_gate.fault.push_back(f1);
                mp_in[j] = !mp_in[j];
                // mp_in[i.output[0]] = !temp;
            }

            f1.first.first = i.output[0];
            f1.first.second = !mp_in[i.output[0]];
            f1.second.first = i.output[0];
            f1.second.second = !mp_in[i.output[0]];
            fault_gate.fault.push_back(f1);
        }

        fault.push_back(fault_gate);
    }
}

void Find_Fault_List(unordered_map<pair<string, bool>, bool, hash_pair> &mp_fault_list, vector<Fault_gate_list> fault)
{
    for (auto i : fault)
    {
        for (auto j : i.fault)
        {

            if (mp_fault_list[j.second] == true)
            {
                mp_fault_list[j.first] = true;
                continue;
            }
            // mp_fault_list[j.first] = false;
        }
    }
}

int main()
{

    string file_name;

    cout << "Enter File Name: ";
    cin >> file_name;

    Verilog_File File = Convert_file(file_name);
    unordered_map<string, int> mp_in;

    // int index = 0;

    for (auto i : File.input)
    {
        cout << "Input : " << i << endl;
        int num;
        cin >> num;
        mp_in[i] = num;
    }

    compute_CUT(mp_in, File.gate);
    vector<Fault_gate_list> fault;

    concurrent_fault_simulation(mp_in, File.gate, fault);

    for (auto i : fault)
    {
        cout << i.g_name << endl;
        for (auto j : i.fault)
        {
            cout << "(" << j.first.first << "," << j.first.second << ")"
                 << " " << j.second.first << endl;
        }

        cout << endl
             << endl;
    }

    unordered_map<pair<string, bool>, bool, hash_pair> mp_fault_list;
    reverse(fault.begin(), fault.end());
    //   cout<<File.output[0]<<File.output[1];
    for (int i = 0; i < No_Output; i++)
    {
        pair<string, bool> f1;
        f1.first = File.output[i];
        f1.second = !mp_in[File.output[i]];
        // cout<<"12"<<f1.first<<","<<f1.second<<endl;
        mp_fault_list[f1] = true;
    }

    Find_Fault_List(mp_fault_list, fault);
    cout << "{ ";
    for (auto j : mp_fault_list)
    {
        if (j.second == true)
            cout << "(" << (j.first).first << "," << (j.first).second << ")"
                 << " ,";
    }
    cout << " }";
    // for(auto j : mp_in){
    //     cout<<j.first<<"= "<<j.second<<endl;
    // }

    // for (auto it : File.input)
    // {
    //     cout << it << ",";
    // }

    // cout << "output" << endl;

    // for (auto it : File.output)
    // {
    //     cout << it << ",";
    // }

    // for (Gate g : File.gate)
    // {
    //     cout << g.g_name << endl;
    //     cout << g.g_type << endl;
    //     for (auto it : g.input)
    //     {
    //         cout << it << " ";
    //     }
    //     cout << endl;
    //     cout << g.output[0] << endl
    //          << endl;
    // }

    return 0;
}