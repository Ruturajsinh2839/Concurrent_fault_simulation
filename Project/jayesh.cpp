#include <bits/stdc++.h>
using namespace std;

int No_Output = 0;
int No_input = 0;
vector<string> types_wire = {"input", "wire", "output"};
vector<string> types_gate = {"nand", "and", "or", "xor", "buf", "not", "nor", "xnor","dum"};
class Gate;
class Wire
{
    public:
    bool value;
    string w_name;
    string w_type;
    vector<int> start;
    vector<int> end;
    int L;
    Wire(){
        L=-1;
    }
};

class Gate
{
public:
    string g_type;
    string g_name;
    vector<int> input_wires;
    vector<int> output_wires;
    int L;
    Gate(){
        L=-1;
    }
};



class Verilog_File
{
public:
    map<int,Gate> gate;
    map<int,Wire> wire;
};

Verilog_File File;
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
        
            
    }
    return 0;
}

Verilog_File Convert_file(string file_name)
{
    ifstream file(file_name);
    string line;
    Verilog_File f;
    int wire_id=0;
    int gate_id=0;
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
       // cout<<line<<endl;
            // cout<<"33333333333333333333333333333333333"<<endl;
        if (Convert_string_present(line,' ', types_wire) == 1)
        {
            line.erase(remove(line.begin() + 8, line.end(), ' '), line.end());
            Convert_string_into_word(line, ' ', words);
           // cout<<words[0]<<" "<<words[0].size()<<endl;
            //cout<<"3333   pandya jayesh gnahsyambhai"<<endl;
            if (words[0] == "input")//--------------------------input
            {    

                // cout<<"1111111111111111111111111111111"<<endl;
                string input = words[1];
                
                words.clear();
                //cout<<"444  pandya jayesh gnahsyambhai"<<endl;
                Convert_string_into_word(input, ',', words);
                for(long long int i=0;i<words.size();i++){
                    No_input++;
                    Wire w;
                    w.w_name=words[i];
                    w.w_type=types_wire[0];
                    w.L=0;
                    w.start.push_back(0);
                   // w.w_id=++wire_id;
                    f.wire[++wire_id]=w;
                }
                //f.input.insert(f.input.end(), words.begin(), words.end());
            }
            else if(words[0] == "output")
            {       
                // cout<<"222222222222222222222222222222222"<<endl;
                //cout<<"555   pandya jayesh gnahsyambhai"<<endl;
                string out = words[0];
                string output = words[1];
                words.clear();
                Convert_string_into_word(output, ',', words);
                if (out == "output")//-------------------------output
                {
                    No_Output = words.size();
                }
                for(long long int i=0;i<words.size();i++){
                    Wire w;
                    w.w_name=words[i];
                    w.w_type=types_wire[2];
                    w.end.push_back(-1);
                   // w.w_id=++wire_id;
                    f.wire[++wire_id]=w;
                }
                
            }
            else 
            {
                // cout<<"444444444444444444444444"<<endl;
                //cout<<"666   pandya jayesh gnahsyambhai"<<endl;
                string output = words[1];
                words.clear();
                Convert_string_into_word(output, ',', words);
                for(long long int i=0;i<words.size();i++){
                    Wire w;
                    w.w_name=words[i];
                    w.w_type=types_wire[1];
                   // w.w_id=++wire_id;
                    f.wire[++wire_id]=w;
                }
            }
        }
        else if (Convert_string_present(line, ' ', types_gate) == 1)
        {
            Convert_string_into_word(line, '(', words);
            // cout<<line<<endl;
            // cout<<"33333333333333333333333333333333333"<<endl;
            //  cout<<words.size()<<endl;
            string temp = words[0];
            string temp1 = words[1];
            words.clear();
            temp1.erase(remove(temp1.begin(), temp1.end(), ' '), temp1.end());
            temp1.erase(remove(temp1.begin(), temp1.end(), ')'), temp1.end());

            Convert_string_into_word(temp, ' ', words);
            //cout<<"777   pandya jayesh gnahsyambhai"<<endl;
            Gate gate;
            
            gate.g_name = words[1];
            gate.g_type = words[0];
            // gate.g_id = ++gate_id;
           ++gate_id;
            words.clear();

            Convert_string_into_word(temp1, ',', words);

            for(long long int i=0;i<words.size();i++){
                // Wire w;
                // w.w_name=words[i];
                // w.w_type=types_wire[0];
                // for(auto wire:f.wire){
                //     if(words[i]==wire.w_name){
                //         if(i==0){
                //             gate.output_wires.push_back(wire);
                //         }else{
                //             gate.input_wires.push_back(wire);
                //         }
                //     }
                // }
                 for(long long int j=0;j<f.wire.size();j++){
                    if(words[i]==f.wire[j+1].w_name){
                        if(i==0){
                            gate.output_wires.push_back(j+1);
                            f.wire[j+1].start.push_back(gate_id);
                        }else{
                            gate.input_wires.push_back(j+1);
                            f.wire[j+1].end.push_back(gate_id);
                        }
                    }
                }

                // if(f.wire.find(w)!=f.wire.end()){
                //     //gate.input_wires.push_back(w);
                // }else{
                //     w.w_type=types_wire[2];
                //     if(f.wire.find(w)!=f.wire.end()){
                //         //gate.input_wires.push_back(w);
                //     }else{
                //         w.w_type=types_wire[1];
                //         //gate.input_wires.push_back(w);
                //     }
                // }
                // if(i==0){
                //     gate.output_wires.push_back(w);
                // }else{
                //     gate.input_wires.push_back(w);
                // }
            }

            //gate.input_wires.insert(gate.input_wires.end(), words.begin() + 1, words.end());
            //gate.output_wires.push_back(words[0]);
            
            words.clear();
            //f.gate.insert(gate);
            f.gate[gate_id]=gate;
        }

        words.clear();
    }
//cout<<"8888   pandya jayesh gnahsyambhai"<<endl;
    file.close();
    return f;
}

bool circuit_modification(Verilog_File &File){
    long long int num=0;
    for(long long int i=0;i<File.wire.size();i++){
        
        if(File.wire[i+1].end.size()>1){
            num++;
            string temp=to_string(num);
            Gate d;
            d.g_type=types_gate[8];
            d.g_name="DUMM"+temp;
            d.input_wires.push_back(i+1);
            d.L=File.wire[i+1].L + 1;
            for(long long int k=0;k<File.wire[i+1].end.size();k++){
                Wire w;
                w.w_type=types_wire[1];
                w.w_name="Ex" + temp + to_string(k+1);
                w.start.push_back(File.gate.size()+1);
                w.end.push_back(File.wire[i+1].end[k]);
                d.output_wires.push_back(File.wire.size()+1);
                w.L=d.L;
                auto it=find(File.gate[File.wire[i+1].end[k]].input_wires.begin(),File.gate[File.wire[i+1].end[k]].input_wires.end(),i+1);
                if(it!=File.gate[File.wire[i+1].end[k]].input_wires.end()) File.gate[File.wire[i+1].end[k]].input_wires.erase(it);
                else return false;
                File.gate[File.wire[i+1].end[k]].input_wires.push_back(File.wire.size()+1);
                File.wire[File.wire.size()+1]=w;
                //i->second[k].input_wires.push_back(w);
               // i->second[k].g_id
            }
            File.wire[i+1].end.clear();
            File.wire[i+1].end.push_back(File.gate.size()+1);
            File.gate[File.gate.size()+1]=d;
        }
    }


    // map<Wire,vector<Gate>> mpp;
    // for(auto g: File.gate){
    //     for(long long int i=0;i<g.input_wires.size();i++){
    //         Wire w=g.input_wires[i];
    //         mpp[w].push_back(g);
    //     }
    // }
    // long long int num=0;
    // for(auto i=mpp.begin();i!=mpp.end();i++){
    //     if((i->second.size())>1){
    //         num++;
    //         string temp=to_string(num);
    //         Gate d;
    //         d.g_type=types_gate[8];
    //         d.g_name="DUMM"+temp;
    //         d.input_wires.push_back(i->first);
    //         d.g_id=File.gate.size();
    //         for(long long int k=0;k<i->second.size();k++){
    //             Wire w;
    //             w.w_type=types_wire[1];
    //             w.w_name="Ex" + temp + to_string(k+1);
    //             d.output_wires.push_back(w);
    //             auto it=find(i->second[k].input_wires.begin(),i->second[k].input_wires.end(),i->first);
    //             if(it!=i->second[k].input_wires.end()) i->second[k].input_wires.erase(it);
    //             else return false;
    //             //i->second[k].input_wires.push_back(w);
    //            // i->second[k].g_id
    //         }
    //         File.gate.insert(d);
           
    //     }
    // }
    return true;
}

void Logic_levelization(Verilog_File &File){
    long long int size=File.gate.size();
    for(long long int i=0;i<size;i++){
        long long int temp = File.gate[i+1].input_wires.size();
        long long int level=0;
        for(long long int j=0;j<temp;j++){
            int temp1=File.wire[File.gate[i+1].input_wires[j]].L;
            if(File.wire[File.gate[i+1].input_wires[j]].end.size()>1) temp1++;
            if(level<temp1){
                level=temp1;
            }
        }
        File.gate[i+1].L=level+1;
        long long int temp2 = File.gate[i+1].output_wires.size();
        for(long long int j=0;j<temp2;j++){
            File.wire[File.gate[i+1].output_wires[j]].L=level+1;                   
        }
    }
}

bool compute_and(long long int id)
{
    bool res = true;
    long long int  size = File.gate[id].input_wires.size();
    for (auto i=0;i<size;i++)
    {   long long int temp = File.gate[id].input_wires[i];
        if (!File.wire[temp].value)
        {   
           
            return false;
        }
    }
  
    return res;
}

bool compute_or(long long int id)
{
    bool res = false;
    long long int  size = File.gate[id].input_wires.size();
    for (auto i=0;i<size;i++)
    {   long long int temp = File.gate[id].input_wires[i];
        if (File.wire[temp].value)
        {  
            return true;
        }
    }
    return res;
}

bool compute_xor(long long int id)
{
    bool res = false;
    long long int  size = File.gate[id].input_wires.size();
    for (auto i=0;i<size;i++)
    {   long long int temp = File.gate[id].input_wires[i];
        
        res ^= File.wire[temp].value;
    }
    
    return res;
}

void compute_CUT()
{   
    multimap<int,int> mpp;
    
    for (auto i=File.gate.begin();i!=File.gate.end();i++)
    {
       // mpp[i->second.L]=i->first;
        mpp.insert(pair<int, int>(i->second.L, i->first));
    }
    long long int size1=mpp.size();
    for (auto i=mpp.begin();i!=mpp.end();i++)
    {
        long long int id=i->second;
        cout<<"computation------------------ gate name "<<File.gate[id].g_name<<endl;
        if (File.gate[id].g_type == "nand")
        {
            //cout<<"********************************8"<<endl;
            bool temp = !compute_and(id);
            //cout<<"********************************8"<<temp<<endl;
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;

        }
        else if (File.gate[id].g_type == "nor")
        {
            bool temp = compute_or(id);
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;

            
        }
        else if (File.gate[id].g_type == "and")
        {
            bool temp = compute_and(id);
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;
        }
        else if (File.gate[id].g_type == "or")
        {
            bool temp = compute_or(id);
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;
        }
        else if (File.gate[id].g_type == "not")
        {
            int temp = File.gate[id].input_wires[0];
            bool t = File.wire[temp].value;
            int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=!t;
        }
        else if (File.gate[id].g_type == "buf")
        {
            int temp = File.gate[id].input_wires[0];
            bool t = File.wire[temp].value;
            int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=t;
        }
        else if (File.gate[id].g_type == "xor")
        {
            bool temp = compute_xor(id);
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;

            
        }
        else if (File.gate[id].g_type == "xnor")
        {
            bool temp = !compute_xor(id);
            long long int temp1 = File.gate[id].output_wires[0];
            File.wire[temp1].value=temp;

            
        }else if(File.gate[id].g_type == "dum")
        {
            int temp = File.gate[id].input_wires[0];
            bool t = File.wire[temp].value;
            int size1 =  File.gate[id].output_wires.size();
            for(long long int k=0;k<size1;k++){
                int temp1 = File.gate[id].output_wires[k];
                File.wire[temp1].value=t;
            }
           
        }
    }
}

void Print(Verilog_File &File){
    long long int size_gate=File.gate.size();
    long long int size_wire=File.wire.size();
    for(long long int i=0;i<size_gate;i++){
        cout<<File.gate[i+1].g_name<<" "<<File.gate[i+1].L<<endl;
    }

    for (auto i=File.gate.begin();i!=File.gate.end();i++)
    {
        cout<<i->second.g_name<<" "<<File.wire[i->second.output_wires[0]].value<<endl;
    }
}

int main()
{

    string file_name;

    cout << "Enter File Name: ";
    cin >> file_name;

    File = Convert_file(file_name);
    Logic_levelization(File);
   // Print(File);
   // cout<<"dskrjfgi5gyebrugegryrtge"<<endl;
    if(!circuit_modification(File)){
        cout<<"ERROR!!: circuit modification error happened.."<<endl;
        // return 0;
    }
    bool arr[4] = {1,1,1,1};
    for(long long int i=0;i<No_input;i++){
        // bool t;
        // cout<<"Enter bool value: ";
        // cin>>t;
        File.wire[i+1].value=arr[i];
    }
     compute_CUT();
     Print(File);
    //cout<<"34tf"<<endl;
     //cout<<File.gate[2].g_name<<" "<<File.gate[2].L<<endl;
     //cout<<"34tf"<<endl;

    return 0;
}
