#include <bits/stdc++.h>
#include "cache_simulator.hh"
int Programcounter;
vector<int> breakpoints;
stack<sstack> gs;
int alpha = 0;
long long registers[32] = {0};
string mem[0x50000];
int cache_size;
int Block_size;
int Associtivity;
string Replacement_Policy;
string write_policy;
int Hits;
int Misses;
int Total_Access;
int Associtivityf;
int c12 = 0;
vector<string> access_lines;
void RegisterInitialisation()
{

    for (int i = 0; i < 32; i++)
    {
        registers[i] = 0;
    }
}
void memoryinitialisation()
{
    for (auto &element : mem)
        element = "00";
}
int indicator = 0;
int dataadress = 0x10000;
map<string, int> Label_line_number = {};
std::map<char, std::string> map1 = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'a', "1010"}, {'b', "1011"}, {'c', "1100"}, {'d', "1101"}, {'e', "1110"}, {'f', "1111"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};
bool islabel(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == ':')
        {
            return true;
        }
    }
    return false;
}
long long neg(unsigned long long x)
{
    return -x;
}
unsigned long long string_to_number(string s, int y)
{
    unsigned long long number = 0;
    unsigned long long a = 1;
    if (s[0] == '-')
    {
        for (int i = s.size() - 1; i > 0; i--)
        {
            number += a * (s[i] - '0');
            a = a * y;
        }
        return neg(number);
    }
    else
    {
        for (int i = s.size() - 1; i >= 0; i--)
        {
            number += a * (s[i] - '0');
            a = a * y;
        }
    }
    // cout<<s<<" "<<number<<endl;

    return number;
}

string con_bin_to_hex(string s, int n)
{
    string s1;
    int z = 0;
    for (int i = 0; i < n / 4; i++)
    {
        string s2 = s.substr(z, 4);
        int y = string_to_number(s2, 2);
        z = z + 4;
        if (y > 9)
        {
            s1.push_back(y - 10 + 'a');
        }
        else
        {
            s1.push_back(y + '0');
        }
    }

    return s1;
}
int label_position(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == ':')
        {
            return i;
        }
    }
    return -1;
}
string twos_complement(string s)
{
    string s1 = s;
    string s2(s.size() - 1, '0');
    s2 = s2 + '1';
    string output(s.size(), '0');
    int carry = 0;
    for (int i = s.size() - 1; i >= 0; i--)
    {
        if ((carry + (s1[i] - '0') + (s2[i] - '0')) > 1)
        {
            output[i] += (carry + (s1[i] - '0') + (s2[i] - '0') - 2);
            carry = 1;
        }
        else
        {
            output[i] += ((s1[i] - '0') + (s2[i] - '0') + carry);
            carry = 0;
        }
    }
    return output;
}
string complement(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
        {
            s[i] = '1';
        }
        else
        {
            s[i] = '0';
        }
    }

    return s;
}
string number_binary(long long number, long long no_of_bits)
{
    string s;

    for (int i = 0; i < no_of_bits; i++)
    {
        s.push_back('0');
    }

    if (number == 0)
    {
        return s;
    }

    int number_is_neg = 0;
    if (number < 0)
    {
        number = 0 - number;
        number_is_neg = 1;
    }
    int z = no_of_bits - 1;
    while (number > 0)
    {
        int x = number & 1;
        s[z] = x + '0';
        z--;
        number = number >> 1;
    }
    if (number_is_neg)
    {
        s = twos_complement(complement(s));
    }
    return s;
}
string con_hex_to_bin(string s)
{
    string s1 = "";
    for (int i = 0; i < s.size(); i++)
    {
        s1 += map1[s[i]];
    }
    return s1;
}
void display_register_values()
{
    cout << "Registers:" << endl;
    for (int i = 0; i < 32; i++)
    {
        cout << "x" << i << "=" << "0x" << con_bin_to_hex(number_binary(registers[i], 64), 64) << endl;
        // cout<<number_binary(registers[i], 64)<<endl;
    }
}
int register_number(string s)
{
    if (s == "x0" || s == "zero")
        return 0;
    if (s == "x1" || s == "ra")
        return 1;
    if (s == "x2" || s == "sp")
        return 2;
    if (s == "x3" || s == "gp")
        return 3;
    if (s == "x4" || s == "tp")
        return 4;
    if (s == "x5" || s == "t0")
        return 5;
    if (s == "x6" || s == "t1")
        return 6;
    if (s == "x7" || s == "t2")
        return 7;
    if (s == "x8" || s == "s0" || s == "fp")
        return 8;
    if (s == "x9" || s == "s1")
        return 9;
    if (s == "x10" || s == "a0")
        return 10;
    if (s == "x11" || s == "a1")
        return 11;
    if (s == "x12" || s == "a2")
        return 12;
    if (s == "x13" || s == "a3")
        return 13;
    if (s == "x14" || s == "a4")
        return 14;
    if (s == "x15" || s == "a5")
        return 15;
    if (s == "x16" || s == "a6")
        return 16;
    if (s == "x17" || s == "a7")
        return 17;
    if (s == "x18" || s == "s2")
        return 18;
    if (s == "x19" || s == "s3")
        return 19;
    if (s == "x20" || s == "s4")
        return 20;
    if (s == "x21" || s == "s5")
        return 21;
    if (s == "x22" || s == "s6")
        return 22;
    if (s == "x23" || s == "s7")
        return 23;
    if (s == "x24" || s == "s8")
        return 24;
    if (s == "x25" || s == "s9")
        return 25;
    if (s == "x26" || s == "s10")
        return 26;
    if (s == "x27" || s == "s11")
        return 27;
    if (s == "x28" || s == "t3")
        return 28;
    if (s == "x29" || s == "t4")
        return 29;
    if (s == "x30" || s == "t5")
        return 30;
    if (s == "x31" || s == "t6")
        return 31;
    return -1;
}
void execute(map<int, string> PC_Instruction, int total_no_of_instructions)
{

    if (Programcounter == (total_no_of_instructions) * 4)
    {
        cout << "Nothing to be executed" << endl;
        cout << endl;
    }
    while (Programcounter != (total_no_of_instructions) * 4)
    {
        int alpha = (Programcounter / 4) + 1;
        auto it = find(breakpoints.begin(), breakpoints.end(), alpha);

        if (it != breakpoints.end())
        {
            indicator = 1;

            Programcounter += 4;
            cout << "Execution stopped at break poin" << endl; // forgot to add
            break;
        }

        if (indicator == 1)
        {
            Programcounter -= 4;
            indicator = 0;
        }

        execute_one(PC_Instruction[Programcounter], total_no_of_instructions);
    }
    if (Programcounter == (total_no_of_instructions) * 4)
    {
        if (!gs.empty())
            gs.pop();
    }
}
void execute_one(string s, int total_no_of_instructions)
{
    alpha = 1;
    if (Programcounter == (total_no_of_instructions) * 4)
    {
        if (!gs.empty())
            gs.pop();
        cout << "Nothing to step" << endl;
        cout << endl;
        return;
    }
    gs.top().line = Programcounter / 4;
    registers[0] = 0;
    string s7 = con_bin_to_hex(number_binary(Programcounter, 32), 32);
    cout << "Executed  " << s << ";  " << "PC: 0X" << s7 << endl;
    string s2 = s;
    stringstream ss(s2);
    string s3;
    ss >> s3;
    if (s3 == "add" || s3 == "sub" || s3 == "sll" || s3 == "xor" || s3 == "srl" || s3 == "sra" || s3 == "or" || s3 == "and")
    {
        execute_R(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "addi" || s3 == "slli" || s3 == "xori" || s3 == "srli" || s3 == "srai" || s3 == "ori" || s3 == "andi")
    {
        execute_IA(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "ld" || s3 == "lh" || s3 == "lw" || s3 == "lbu" || s3 == "lhu" || s3 == "jalr" || s3 == "lwu" || s3 == "lb" || s3 == "lbu")
    {
        execute_Il(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "sb" || s3 == "sh" || s3 == "sw" || s3 == "sd")
    {
        execute_S(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "beq" || s3 == "bne" || s3 == "blt" || s3 == "bge" || s3 == "bltu" || s3 == "bgeu")
    {
        execute_B(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "jal")
    {
        execute_J(s);
        registers[0] = 0;
        return;
    }
    if (s3 == "lui")
    {

        execute_U(s);
        registers[0] = 0;
        return;
    }

    return;
}
void execute_R(string s)
{
    vector<string> swords; // index 0-instruction 1-destination 2-src1 3-src2
    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {
        swords[0].push_back(s[i]);
    }
    // cout<<swords[0]<<endl;
    int z = 1;
    swords.push_back("");

    for (int i = swords[0].size() + 1; i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    if (swords[0] == "add")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] + registers[register_number(swords[3])];
        Programcounter += 4;
    }
    if (swords[0] == "sub")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] - registers[register_number(swords[3])];
        Programcounter += 4;
    }
    if (swords[0] == "and")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[3])] & registers[register_number(swords[2])];
        Programcounter += 4;
    }
    if (swords[0] == "or")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[3])] | registers[register_number(swords[2])];
        Programcounter += 4;
    }
    if (swords[0] == "xor")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[3])] ^ registers[register_number(swords[2])];
        Programcounter += 4;
    }
    if (swords[0] == "srl")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] >> registers[register_number(swords[3])];
        Programcounter += 4;
    }
    if (swords[0] == "sll")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] << registers[register_number(swords[3])];
        Programcounter += 4;
    }
    if (swords[0] == "sra")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[3])] >> registers[register_number(swords[2])];
        Programcounter += 4;
    }
}
void execute_IA(string s)
{
    vector<string> swords; // index 0-instruction 1-destination 2-src1 3-src2
    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {
        swords[0].push_back(s[i]);
    }
    // cout<<swords[0]<<endl;
    int z = 1;
    swords.push_back("");

    for (int i = swords[0].size() + 1; i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    int immediate = string_to_number(swords[3], 10);

    if (swords[0] == "addi")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] + immediate;
        // cout<<registers[register_number(swords[2])]<<" "<<immediate<<endl;

        Programcounter += 4;
    }
    if (swords[0] == "xori")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] ^ immediate;
        Programcounter += 4;
    }
    if (swords[0] == "ori")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] | immediate;
        Programcounter += 4;
    }
    if (swords[0] == "andi")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] & immediate;
        Programcounter += 4;
    }
    if (swords[0] == "slli")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] << immediate;
        Programcounter += 4;
    }
    if (swords[0] == "srli")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] >> immediate;
        Programcounter += 4;
    }
    if (swords[0] == "srai")
    {
        registers[register_number(swords[1])] = registers[register_number(swords[2])] >> immediate;
        Programcounter += 4;
    }
}
void execute_Il(string s)
{
    // cout<<Total_Access<<endl;
    if (cache_enabled == 1)
    {
        vector<string> swords;
        swords.push_back("");
        int z = 0;
        char ch[3] = {',', '(', ')'};
        int j = 0;
        for (int i = 0; s[i] != ' '; i++)
        {
            swords[0].push_back(s[i]);
        }
        z++;
        swords.push_back("");
        for (int i = ((swords[0].size()) + 1); i < s.size(); i++)
        {

            if (s[i] != ch[j])
            {

                swords[z].push_back(s[i]);
            }
            else
            {
                z++;
                j++;
                swords.push_back("");
            }
        }
        for (int i = 0; i < (z + 1); i++)
        {
            if (swords[i][0] == ' ')
            {
                swords[i] = swords[i].substr(1);
            }
        }

        int index;
        int Tag;
        int bitoffset;
        int sets = cache_size / (Block_size * Associtivity);

        int immediate = string_to_number(swords[2], 10);

        int base_address = registers[register_number(swords[3])] + immediate;

        int rock = base_address / Block_size;

        index = rock % sets;

        Tag = rock / sets;

        int ind = 0;
        int beta = 0;

        if (swords[0] != "jalr")
        {
            Total_Access++;
            for (int i = 0; i < cache_mem[index]->Tag.size(); i++)
            { // cout<<"hi2"<<endl;
                if (cache_mem[index]->valid_bit[i] == 1 && Tag == cache_mem[index]->Tag[i])
                {
                    Hits++;
                    ind = 1;
                    beta = i;
                }
            }
            if (ind == 0)
            {
                Misses++;

                cache_allocate(base_address, sets);
            }

            //   cout << base_address << endl;
            string h;
            h = "R: ";
            h = h + "Address:0x";
            h = h + con_bin_to_hex(number_binary(base_address, 64), 64);
            h = h + ", set:0x";
            h = h + con_bin_to_hex(number_binary(index, 20), 20);
            if (ind == 0)
            {
                h = h + ", Miss, ";
            }
            else
            {
                h = h + ", Hit, ";
            }
            h = h + "Tag:";
            h = h + con_bin_to_hex(number_binary(Tag, 20), 20);
            if (ind == 1)
            {
                if (cache_mem[index]->Dirty[beta] == 0)
                {
                    h = h + ", Dirty";
                }
                else
                {
                    h = h + ", Clean";
                }
            }
            else
            {
                h = h + ", Clean";
            }
            access_lines.push_back(h);
            // cout<<access_lines[access_lines.size()-1]<<endl;
            // cout<<h<<endl;
        }
        string s1 = "";
        if (swords[0] == "ld")
        {

            string s1 = cachefetch(base_address, sets, 8);
            // cout<<s1<<endl;
            // cout<<"hi4"<<endl;
            if (s1 == "null")
            {

                return;
            }
            s1 = con_hex_to_bin(s1);
            // cout<<s1<<endl;
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lw")
        {
            string s1 = cachefetch(base_address, sets, 4);
            if (s1 == "null")
            {
                return;
            }
            s1 = con_hex_to_bin(s1);

            for (int i = 0; i < 32; i++)
            {
                s1 = s1[0] + s1;
            }

            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lwu")
        {
            string s1 = cachefetch(base_address, sets, 4);
            if (s1 == "null")
            {
                Programcounter += 4;
                return;
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 32; i++)
            {
                s1 = '0' + s1;
            }
            //  cout<<string_to_number(s1, 2)<<endl;
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            //  cout<<number_binary(registers[register_number(swords[1])],64)<<endl;
            // cout<<con_bin_to_hex(number_binary(registers[register_number(swords[1])],64),64)<<endl;
            Programcounter += 4;
        }
        if (swords[0] == "lh")
        {
            string s1 = cachefetch(base_address, sets, 2);
            // cout << s1 << endl;
            if (s1 == "null")
            {
                Programcounter += 4;
                return;
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 48; i++)
            {
                s1 = s1[0] + s1;
            }
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lhu")
        {
            string s1 = cachefetch(base_address, sets, 2);
            if (s1 == "null")
            {
                Programcounter += 4;
                return;
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 48; i++)
            {
                s1 = '0' + s1;
            }
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            Programcounter += 4;
        }
        if (swords[0] == "lb")
        {
            string s1 = cachefetch(base_address, sets, 1);
            // cout << s1 << endl;
            if (s1 == "null")
            {
                Programcounter += 4;
                return;
            }
            s1 = con_hex_to_bin(s1);
            //     cout << s1 << endl;
            for (int i = 0; i < 56; i++)
            {
                s1 = s1[0] + s1;
            }
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lbu")
        {
            string s1 = cachefetch(base_address, sets, 1);
            if (s1 == "null")
            {
                Programcounter += 4;
                return;
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 56; i++)
            {
                s1 = '0' + s1;
            }
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            Programcounter += 4;
        }
        if (swords[0] == "jalr")
        { // cout<<gs.top().s<<endl;
            gs.pop();
            int immediate = string_to_number(swords[2], 10);
            registers[register_number(swords[1])] = Programcounter + 4;
            Programcounter = (registers[register_number(swords[3])] + immediate);
        }
    }
    else
    {
        vector<string> swords;
        swords.push_back("");
        int z = 0;
        char ch[3] = {',', '(', ')'};
        int j = 0;
        for (int i = 0; s[i] != ' '; i++)
        {
            swords[0].push_back(s[i]);
        }
        z++;
        swords.push_back("");
        for (int i = ((swords[0].size()) + 1); i < s.size(); i++)
        {

            if (s[i] != ch[j])
            {

                swords[z].push_back(s[i]);
            }
            else
            {
                z++;
                j++;
                swords.push_back("");
            }
        }
        for (int i = 0; i < (z + 1); i++)
        {
            if (swords[i][0] == ' ')
            {
                swords[i] = swords[i].substr(1);
            }
        }
        // cout<<swords[0]<<endl;

        string s1 = "";
        if (swords[0] == "ld")
        {
            int immediate = string_to_number(swords[2], 10);
            long long int base_address = registers[register_number(swords[3])];
            for (int i = 7; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            // cout<<s1<<endl;
            s1 = con_hex_to_bin(s1);
            // cout<<s1<<endl;
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lw")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 3; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            s1 = con_hex_to_bin(s1);

            for (int i = 0; i < 32; i++)
            {
                s1 = s1[0] + s1;
            }

            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lwu")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 3; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 32; i++)
            {
                s1 = '0' + s1;
            }
            //  cout<<string_to_number(s1, 2)<<endl;
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            //  cout<<number_binary(registers[register_number(swords[1])],64)<<endl;
            // cout<<con_bin_to_hex(number_binary(registers[register_number(swords[1])],64),64)<<endl;
            Programcounter += 4;
        }
        if (swords[0] == "lh")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 1; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            s1 = con_hex_to_bin(s1);
            //  cout << s1 << endl;
            for (int i = 0; i < 48; i++)
            {
                s1 = s1[0] + s1;
            }
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lhu")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 1; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 48; i++)
            {
                s1 = '0' + s1;
            }
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            Programcounter += 4;
        }
        if (swords[0] == "lb")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 0; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            //   cout << s1 << endl;
            s1 = con_hex_to_bin(s1);
            // cout << s1 << endl;
            for (int i = 0; i < 56; i++)
            {
                s1 = s1[0] + s1;
            }
            if (s1[0] == '1')
            {
                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);

                registers[register_number(swords[1])] += string_to_number(s1, 2);

                registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
            }
            else
            {
                registers[register_number(swords[1])] = string_to_number(s1, 2);
            }

            Programcounter += 4;
        }
        if (swords[0] == "lbu")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            for (int i = 0; i >= 0; i--)
            {
                s1 += mem[immediate + base_address + i];
            }
            s1 = con_hex_to_bin(s1);
            for (int i = 0; i < 56; i++)
            {
                s1 = '0' + s1;
            }
            registers[register_number(swords[1])] = string_to_number(s1, 2);
            Programcounter += 4;
        }
        if (swords[0] == "jalr")
        { // cout<<gs.top().s<<endl;
            gs.pop();
            int immediate = string_to_number(swords[2], 10);
            registers[register_number(swords[1])] = Programcounter + 4;
            Programcounter = (registers[register_number(swords[3])] + immediate);
        }
    }
}
void execute_S(string s)
{
    if (cache_enabled == 1)
    {
        Total_Access++;
        //  cout << Total_Access << endl;
        vector<string> swords;
        swords.push_back("");
        for (int i = 0; s[i] != ' '; i++)
        {
            swords[0].push_back(s[i]);
        }
        int z = 1;
        swords.push_back("");
        char ch[3] = {',', '(', ')'};
        int j = 0;
        for (int i = ((swords[0].size()) + 1); i < s.size(); i++)
        {

            if (s[i] != ch[j])
            {

                swords[z].push_back(s[i]);
            }
            else
            {
                z++;
                j++;
                swords.push_back("");
            }
        }
        for (int i = 0; i < (z + 1); i++)
        {
            if (swords[i][0] == ' ')
            {
                swords[i] = swords[i].substr(1);
            }
        }
        int index;
        int Tag;
        int bitoffset;
        int sets = cache_size / (Block_size * Associtivity);
        int immediate = string_to_number(swords[2], 10);
        int base_address = registers[register_number(swords[3])] + immediate;
        int rock = base_address / Block_size;
        index = rock % sets;
        Tag = rock / sets;
        int ind = 0;
        int beta = 0;
        for (int i = 0; i < cache_mem[index]->Tag.size(); i++)
        {
            if (cache_mem[index]->valid_bit[i] == 1 && Tag == cache_mem[index]->Tag[i])
            {
                Hits++;
                ind = 1;
                beta = i;
            }
        }
        if (ind == 0)
        {
            Misses++;

            //  cache_allocate(base_address,sets);
        }
        string h;
        h = "W: ";
        h = h + "Address:0x";
        h = h + con_bin_to_hex(number_binary(base_address, 64), 64);
        h = h + ", set:0x";
        h = h + con_bin_to_hex(number_binary(index, 20), 20);
        if (ind == 0)
        {
            h = h + ", Miss, ";
        }
        else
        {
            h = h + ", Hit, ";
        }
        h = h + "Tag:";
        h = h + con_bin_to_hex(number_binary(Tag, 20), 20);
        if (ind == 1)
        {
            if (cache_mem[index]->Dirty[beta] == 0)
            {
                h = h + ", Dirty";
            }
            else
            {
                h = h + ", Clean";
            }
        }
        else
        {
            h = h + ", Clean";
        }
        access_lines.push_back(h);
        string s1 = "";
        if (swords[0] == "sd")
        {

            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 64), 64);
            if (write_policy == "WT")
            {

                for (int i = 7; i >= 0; i--)
                {

                    mem[base_address + i] = s1.substr(2 * (7 - i), 2);
                    // cout<<immediate+base_address+i<<endl;
                }

                if (ind == 1)
                {
                    cachewrite(base_address, s1, sets, 8);
                }
            }
            else
            {
                if (ind == 0)
                {
                    cache_allocate(base_address, sets);
                    cachewrite(base_address, s1, sets, 8);
                }
                else
                { // cout<<"hi2"<<endl;
                    cachewrite(base_address, s1, sets, 8);
                }
            }
            Programcounter += 4;
        }
        if (swords[0] == "sw")
        {

            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 32), 32);
            if (write_policy == "WT")
            {

                for (int i = 3; i >= 0; i--)
                { // cout<<"hell0"<<endl;
                    mem[base_address + i] = s1.substr(2 * (3 - i), 2);
                }
                if (ind == 1)
                {
                    cachewrite(base_address, s1, sets, 4);
                }
            }
            else
            {
                if (ind == 0)
                {
                    cache_allocate(base_address, sets);
                    cachewrite(base_address, s1, sets, 4);
                }
                else
                {
                    cachewrite(base_address, s1, sets, 4);
                }
            }
            Programcounter += 4;
        }
        if (swords[0] == "sh")
        {

            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 16), 16);
            if (write_policy == "WT")
            {
                for (int i = 1; i >= 0; i--)
                {
                    mem[base_address + i] = s1.substr(2 * (1 - i), 2);
                }
                if (ind == 1)
                {
                    cachewrite(base_address, s1, sets, 2);
                }
            }
            else
            {
                if (ind == 0)
                {
                    cache_allocate(base_address, sets);
                    cachewrite(base_address, s1, sets, 2);
                }
                else
                {
                    cachewrite(base_address, s1, sets, 2);
                }
            }

            Programcounter += 4;
        }
        if (swords[0] == "sb")
        {

            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 8), 8);
            if (write_policy == "WT")
            { // cout<<s1<<endl;
                for (int i = 0; i >= 0; i--)
                {
                    mem[base_address + i] = s1.substr(i, 2);
                    //  cout<<mem[immediate + base_address + i]<<" "<<immediate<<base_address + i<<endl;
                }
                if (ind == 1)
                {
                    cachewrite(base_address, s1, sets, 1);
                }
            }
            else
            {
                if (ind == 0)
                {
                    cache_allocate(base_address, sets);
                    cachewrite(base_address, s1, sets, 1);
                }
                else
                {
                    cachewrite(base_address, s1, sets, 1);
                }
            }
            Programcounter += 4;
        }
    }
    else
    {
        vector<string> swords;
        swords.push_back("");
        for (int i = 0; s[i] != ' '; i++)
        {
            swords[0].push_back(s[i]);
        }
        int z = 1;
        swords.push_back("");
        char ch[3] = {',', '(', ')'};
        int j = 0;
        for (int i = ((swords[0].size()) + 1); i < s.size(); i++)
        {

            if (s[i] != ch[j])
            {

                swords[z].push_back(s[i]);
            }
            else
            {
                z++;
                j++;
                swords.push_back("");
            }
        }
        for (int i = 0; i < (z + 1); i++)
        {
            if (swords[i][0] == ' ')
            {
                swords[i] = swords[i].substr(1);
            }
        }
        string s1;
        if (swords[0] == "sd")
        {

            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 64), 64);
            for (int i = 7; i >= 0; i--)
            {

                mem[immediate + base_address + i] = s1.substr(2 * (7 - i), 2);
                // cout<<immediate+base_address+i<<endl;
            }
            Programcounter += 4;
        }
        if (swords[0] == "sw")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 32), 32);
            for (int i = 3; i >= 0; i--)
            { // cout<<"hell0"<<endl;
                mem[immediate + base_address + i] = s1.substr(2 * (3 - i), 2);
            }
            Programcounter += 4;
        }
        if (swords[0] == "sh")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 16), 16);
            for (int i = 1; i >= 0; i--)
            {
                mem[immediate + base_address + i] = s1.substr(2 * (1 - i), 2);
            }
            Programcounter += 4;
        }
        if (swords[0] == "sb")
        {
            int immediate = string_to_number(swords[2], 10);
            int base_address = registers[register_number(swords[3])];
            s1 = con_bin_to_hex(number_binary(registers[register_number(swords[1])], 8), 8);
            for (int i = 0; i >= 0; i--)
            {
                mem[immediate + base_address + i] = s1.substr(i, 2);
            }
            Programcounter += 4;
        }
    }
}
void execute_B(string s)
{
    vector<string> swords;
    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {

        swords[0].push_back(s[i]);
    }

    int z = 1;
    swords.push_back("");
    for (int i = swords[0].size() + 1; i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    int immediate = 4 * Label_line_number[swords[3]] - Programcounter - 4;
    if (swords[0] == "beq")
    {
        if (registers[register_number(swords[1])] == registers[register_number(swords[2])])
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
    if (swords[0] == "bne")
    {
        if (registers[register_number(swords[1])] != registers[register_number(swords[2])])
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
    if (swords[0] == "blt")
    {
        if (registers[register_number(swords[1])] < registers[register_number(swords[2])])
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
    if (swords[0] == "bge")
    {
        if (registers[register_number(swords[1])] >= registers[register_number(swords[2])])
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
    if (swords[0] == "bgeu")
    {
        long long int s1 = registers[register_number(swords[1])];
        long long int s2 = registers[register_number(swords[2])];

        if (u_int64_t(s1) >= u_int64_t(s2))
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
    if (swords[0] == "bltu")
    {
        long long int s1 = registers[register_number(swords[1])];
        long long int s2 = registers[register_number(swords[2])];
        if (u_int64_t(s1) <= u_int64_t(s2))
        {
            Programcounter += immediate;
        }
        else
        {
            Programcounter += 4;
        }
    }
}
void execute_J(string s)
{
    vector<string> swords;

    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {

        swords[0].push_back(s[i]);
    }

    int z = 1;
    swords.push_back("");
    for (int i = swords[0].size(); i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    gs.push({swords[2], Programcounter / 4});
    int immediate = 4 * Label_line_number[swords[2]] - Programcounter - 4;
    // cout<<immediate<<endl;
    registers[register_number(swords[1])] = Programcounter + 4;
    Programcounter += immediate;
}
void execute_U(string s)
{
    vector<string> swords;
    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {

        swords[0].push_back(s[i]);
    }

    int z = 1;
    swords.push_back("");
    for (int i = swords[0].size(); i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    swords[2] = swords[2].substr(2);

    string immediate = "";
    int j = 0;
    if (swords[2].size() > 5)
    {
        j = swords[2].size() - 5;
    }
    if (swords[2].size() < 5)
    {
        for (int i = swords[2].size(); i < 5; i++)
        {
            immediate += '0';
        }
    }
    for (int i = j; i < swords[2].size(); i++)
    {
        immediate += swords[2][i];
    }
    immediate += "000";

    immediate = con_hex_to_bin(immediate);

    for (int i = 0; i < 32; i++)
    {
        immediate = immediate[0] + immediate;
    }
    if (immediate[0] == '1')
    { // cout<<registers[register_number(swords[1])]<<endl;
        // cout<<(unsigned long long )pow(2,63)<<endl;
        registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
        // cout<<registers[register_number(swords[1])]<<endl;
    }
    registers[register_number(swords[1])] += string_to_number(immediate, 2);

    if (immediate[0] == '1')
    { // cout<<registers[register_number(swords[1])]<<endl;
        registers[register_number(swords[1])] -= (unsigned long long)pow(2, 63);
        // cout<<registers[register_number(swords[1])]<<endl;
    }
    Programcounter += 4;
}
void displaymem(string s, int no_of_elements)
{
    string s1 = s.substr(2);
    long long z = string_to_number(con_hex_to_bin(s1), 2);
    //    cout << z << endl; // forgot comment it out

    for (int i = 0; i < no_of_elements; i++)
    {
        cout << "Memory[0X" << con_bin_to_hex(number_binary(z + i, 20), 20) << "]=0x" << mem[z + i] << endl;
    }
    cout << endl;
}
void datasection(string s, int no_of_bytes)
{
    vector<string> swords;
    swords.push_back("");
    for (int i = 0; s[i] != ' '; i++)
    {

        swords[0].push_back(s[i]);
    }

    int z = 1;
    swords.push_back("");
    for (int i = swords[0].size(); i < s.size(); i++)
    {

        if (s[i] != ',')
        {
            swords[z].push_back(s[i]);
        }
        else
        {
            z++;
            swords.push_back("");
        }
    }
    for (int i = 0; i < (z + 1); i++)
    {
        if (swords[i][0] == ' ')
        {
            swords[i] = swords[i].substr(1);
        }
    }
    for (int i = 1; i < swords.size(); i++)
    {
        string s2 = con_bin_to_hex(number_binary(string_to_number(swords[i], 10), 8 * no_of_bytes), 8 * no_of_bytes);
        for (int i = (no_of_bytes - 1); i >= 0; i--)
        {
            mem[dataadress] = s2.substr(2 * (i), 2);
            dataadress++;
        }
    }
}
void showstack(stack<sstack> x)
{
    if (x.empty())
    {
        cout << "Empty Call Stack: Execution complete" << endl;
    }
    stack<sstack> y = x;
    stack<sstack> z;
    while (!y.empty())
    {
        z.push(y.top());
        y.pop();
    }
    while (!z.empty())
    {
        if (alpha != 0)
        {
            cout << z.top().s << ":" << z.top().line + 1 << endl;
            z.pop();
        }
        else
        {
            cout << z.top().s << ":" << z.top().line << endl;
            z.pop();
        }
    }
    cout << endl;
    return;
}
