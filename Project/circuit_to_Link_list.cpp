#include <bits/stdc++.h>
using namespace std;

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
    vector<string> types_gate = {"nand", "and", "or", "xor", "buf","not", "nor", "xnor"};
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
                string output = words[1];
                words.clear();
                Convert_string_into_word(output, ',', words);
                f.output.insert(f.output.end(), words.begin(), words.end());
            }
        }
        else if(Convert_string_present(line, ' ', types_gate) == 1)
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
            gate.input.insert(gate.input.end(), words.begin()+1, words.end());
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
    bool res=false;
    for(auto i:input)
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
        else if (i.g_type =="and")
        {
            bool temp = compute_and(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type =="or")
        {
            bool temp = compute_or(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type =="not")
        {
            bool temp = !mp_in[i.input[0]];
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type =="buf")
        {
            bool temp = mp_in[i.input[0]];
            mp_in[i.output[0]] = temp;
        }
        else if (i.g_type =="xor")
        {
            bool temp = compute_xor(i.input, mp_in);
            mp_in[i.output[0]] = temp;
        }
         else if (i.g_type =="xnFor")
        {
            bool temp = compute_xor(i.input, mp_in);
            mp_in[i.output[0]] = !temp;
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

    for(auto j : mp_in){
        cout<<j.first<<"= "<<j.second<<endl;
    }

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