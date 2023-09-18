#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;

void findRedundant(vector<pair<pair<int,char>,pair<int,int>>> &binary_tree)
{
    bool has_redundant = false;
    for (int i=1;i<binary_tree.size();i++)
    {
        if (binary_tree[i].second.first == binary_tree[i].second.second)
        {
            has_redundant = true;
            for (int j=1;j<i;j++)
            {
                if (binary_tree[j].second.first == binary_tree[i].first.first)
                {
                    binary_tree[j].second.first = binary_tree[i].second.first;
                }
                if (binary_tree[j].second.second == binary_tree[i].first.first)
                {
                    binary_tree[j].second.second = binary_tree[i].second.second;
                }
            }
            binary_tree.erase(binary_tree.begin()+i);
        }
        else
        {
            for (int m = i+1;m<binary_tree.size();m++)
            {
                if (binary_tree[i].first.second == binary_tree[m].first.second && binary_tree[i].second.first == binary_tree[m].second.first && binary_tree[i].second.second == binary_tree[m].second.second)
                {
                    has_redundant = true;
                    for (int j=1;j<i;j++)
                    {
                        if (binary_tree[j].second.first == binary_tree[m].first.first)
                        {
                            binary_tree[j].second.first = binary_tree[i].first.first;
                        }
                        if (binary_tree[j].second.second == binary_tree[m].first.first)
                        {
                            binary_tree[j].second.second = binary_tree[i].first.first;
                        }
                    }
                    binary_tree.erase(binary_tree.begin()+m);
                }
            }
        }
    }
    if (has_redundant)
    {
        return findRedundant(binary_tree);
    }
    return;
}

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        ifstream fin(argv[1]);
        ofstream fout(argv[2]);

        string str = "";
        int var = 0;
        int out_var = 0;
        fin >> str >> var;

        vector<char> varlab;
        string func_label = "";

        fin >> str >> out_var;
        fin >> str;

        for (int i=0;i<var;i++)
        {
            char temp;
            fin >> temp;
            varlab.push_back(temp);
        }

        fin >> str >> func_label;

        int statement_var = 0;
        fin >> str >> statement_var;
        vector<string> statement;

        if (str != ".p")
        {
            statement_var = 0;
            while (str != ".e")
            {
                statement_var++;
                string temp = "";
                string temp_str = str;
                string temp_char = "";
                for (int j=0;j<var;j++)
                {
                    if (temp_str[j] != '-')
                    {
                        if (temp_str[j] == '0')
                        {
                            temp += varlab[j];
                            temp += "!";
                        }
                        else
                        {
                            temp += varlab[j];
                        }
                        
                    }   
                }
                statement.push_back(temp);
                fin >> str >> temp_char;
            }
        }
        else
        {
            for (int i=0;i<statement_var;i++)
            {
                string temp = "";
                string temp_str = "";
                string temp_char = "";
                fin >> temp_str >> temp_char;
                for (int j=0;j<var;j++)
                {
                    if (temp_str[j] != '-')
                    {
                        if (temp_str[j] == '0')
                        {
                            temp += varlab[j];
                            temp += "!";
                        }
                        else
                        {
                            temp += varlab[j];
                        }
                        
                    }   
                }
                statement.push_back(temp);
            }
            
            fin >> str;
        }

        vector<pair<pair<int,char>,pair<int,int>>> binary_tree;
        binary_tree.push_back(make_pair(make_pair(0,'0'),make_pair(0,0)));
        vector<int> record_bool(pow(2,var+1),-1);
        for (int m=1;m<=pow(2,var)-1;m++)
        {
            int current_layer = floor(log2(m));
            binary_tree.push_back(make_pair(make_pair(m,varlab[current_layer]),make_pair(m*2,m*2+1)));
            record_bool[m*2] = 0;
            record_bool[m*2+1] = 1;
            if (current_layer == var-1)
            {
                vector<int> dyna_variable;
                dyna_variable.push_back(0);
                int upper_layer = m;
                while(upper_layer > 0) { 
                    dyna_variable.push_back(record_bool[upper_layer]);
                    upper_layer = floor(upper_layer / 2);
                }
                dyna_variable.pop_back();
                reverse(dyna_variable.begin(),dyna_variable.end());
                int res1 = 0;
                int v = 0;
                for (v;v<statement.size();v++)
                {
                    int current_res = 1;
                    int index = 0;
                    for (int k=0;k<statement[v].length();k++)
                    {
                        if (statement[v][k] == '!') continue;
                        for (int z=0;z<varlab.size();z++)
                        {
                            if (statement[v][k] == varlab[z])
                            {
                                index = z;
                                break;
                            }
                        }
                        if (k+1 < statement[v].length() && statement[v][k+1] == '!')
                        {
                            current_res = current_res & ~dyna_variable[index];
                        }
                        else
                        {
                            current_res = current_res & dyna_variable[index];
                        }
                    }
                    if (v == 0) res1 = current_res;
                    else res1 = res1 | current_res;
                }
                res1 = (!res1) ? 0 : pow(2,var);
                dyna_variable.pop_back();
                dyna_variable.push_back(1);
                int res2 = 0;
                for (int v = 0;v<statement.size();v++)
                {
                    int current_res = 1;
                    int index = 0;
                    for (int k=0;k<statement[v].length();k++)
                    {
                        if (statement[v][k] == '!') continue;
                        for (int z=0;z<varlab.size();z++)
                        {
                            if (statement[v][k] == varlab[z])
                            {
                                index = z;
                                break;
                            }
                        }
                        if (k+1 < statement[v].length() && statement[v][k+1] == '!')
                        {
                            current_res = current_res & ~dyna_variable[index];
                        }
                        else
                        {
                            current_res = current_res & dyna_variable[index];
                        }
                    }
                    if (v == 0) res2 = current_res;
                    else res2 = res2 | current_res;
                }
                res2 = (!res2) ? 0 : pow(2,var);
                binary_tree[m].second.first = res1;
                binary_tree[m].second.second = res2;
            }
        }
        findRedundant(binary_tree);

        //start output
        fout << "digraph ROBDD {" << endl;
        for(int i=1;i<binary_tree.size();i++)
        {
            if (i == 1)
            {
                fout << "  {rank = same " << binary_tree[i].first.first;
            }
            else if (i != 1 && binary_tree[i].first.second != binary_tree[i-1].first.second)
            {
                fout << "}" << "\n";
                fout << "  {rank = same " << binary_tree[i].first.first;
            }
            else
            {
                fout << " " << binary_tree[i].first.first;
            }
        }
        fout << "}" << "\n" << "\n";

        fout << "  0 [label=0, shape=box]" << endl;
        for (int i=1;i<binary_tree.size();i++)
        {
            fout << "  " << binary_tree[i].first.first << " [label=\"" << binary_tree[i].first.second << "\"]" << endl;
        }
        fout << "  " << pow(2,var) << " [label=1, shape=box]" << endl;
        fout << "\n";

        for (int i=1;i<binary_tree.size();i++)
        {
            fout << "  " << binary_tree[i].first.first << " -> " << binary_tree[i].second.first << " [label=0, style=dotted]" << endl;
            fout << "  " << binary_tree[i].first.first << " -> " << binary_tree[i].second.second << " [label=1, style=solid]" << endl;
        }
        fout << "}" << endl;
        //end of output
        return 0;
    }
    else
    {
        cout << "Invalid input." << endl;
        return 0;
    }
}