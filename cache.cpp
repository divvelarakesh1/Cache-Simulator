#include <bits/stdc++.h>
#include "cache_simulator.hh"
vector<cache *> cache_mem;
void initialise_cache()
{
    if (Associtivity != 0)
    {
        int no_of_structs = cache_size / Block_size;
        int sets = no_of_structs / Associtivity;
        int index = 0;
        for (int i = 0; i < sets; i++)
        {

            cache *dummy = new cache;
            dummy->set = index;
            for (int j = 0; j < Associtivity; j++)
            {
                dummy->valid_bit.push_back(0);
                dummy->Dirty.push_back(1);
            }
            cache_mem.push_back(dummy);
            index++;
        }
    }
    else
    {
        int no_of_structs = cache_size / Block_size;
        int sets = 1;
        cache *dummy = new cache;
        dummy->set = 0;
        for (int j = 0; j < no_of_structs; j++)
        {
            dummy->valid_bit.push_back(0);
            dummy->Dirty.push_back(1);
        }
        cache_mem.push_back(dummy);
    }
}
void cache_invaildate()
{
    for (int i = 0; i < cache_mem.size(); i++)
    {
        for (int j = 0; j < cache_mem[i]->Dirty.size(); j++)
        {
            if ((cache_mem[i]->valid_bit[j] == 1))
            {   
                if ((cache_mem[i]->Dirty[j] == 0))
                {
                    int rock = cache_mem[i]->Tag[j];
                    rock = rock * Associtivity;
                    rock += cache_mem[i]->set;
                    rock = rock * Block_size;
                    string x1 = cachefetch(rock, cache_mem[i]->set, Block_size);
                    WRITE_TO_MEM(rock, Block_size, x1);
                    
                }
               
              
                
            }

        }
        
    }

    for (int i = 0; i < cache_mem.size(); i++)
    {
        for (int j = 0; j < cache_mem[i]->valid_bit.size(); j++)
        {
            cache_mem[i]->valid_bit[j] = 0;
        }
        cache_mem[i]->Tag.pop_back();
    }
}
void cache_details()
{
    cout << "Cache Size: " << cache_size << endl;
    cout << "Block Size:" << Block_size << endl;
    cout << "Associtivity:" << Associtivity << endl;
    cout << "Replacement Policy:" << Replacement_Policy << endl;
    cout << "Write Policy:" << write_policy << endl;
}
void dumping(string s)
{
    ofstream out(s);
    for (int i = 0; i < cache_mem.size(); i++)
    {
        for (int j = 0; j < cache_mem[i]->valid_bit.size(); j++)
        {
            if (cache_mem[i]->valid_bit[j] == 1)
            {
                string s;
                s = "Set:0x" + con_bin_to_hex(number_binary(cache_mem[i]->set, 20), 20) + ",Tag:0x" + con_bin_to_hex(number_binary(cache_mem[i]->Tag[j], 20), 20) + ",";
                if (cache_mem[i]->Dirty[j] == 0)
                {
                    s = s + "Dirty";
                }
                else
                {
                    s = s + "Clean";
                }
                out << (s) << endl;
            }
        }
    }
}
void cache_allocate(int base_address, int sets)
{
    int index;
    int tag;
    int bitoffset;
    int rock = base_address / (Block_size);
    index = rock % sets;
    tag = rock / sets;
    rock = rock * Block_size;
    // cout<<rock<<endl;
    if (cache_mem[index]->no_of_blocks.size() < (Associtivity))
    {

        vector<string> dummy;
        for (int i = 0; i < (Block_size); i++)
        {

            dummy.push_back(mem[rock + i]);
        }
        //  for(int i=0;i<dummy.size();i++){
        //    cout<<dummy[i]<<endl;
        //}
        if ((cache_mem[index]->no_of_blocks.size() == 0) || (Replacement_Policy != "LRU"))
        {
            cache_mem[index]->Tag.push_back(tag);
            cache_mem[index]->no_of_blocks.push_back(dummy);
            int y = cache_mem[index]->no_of_blocks.size();
            cache_mem[index]->valid_bit[y - 1] = 1;
        }
        else
        {
            cache_mem[index]->Tag.insert(cache_mem[index]->Tag.begin(), tag);
            cache_mem[index]->no_of_blocks.insert(cache_mem[index]->no_of_blocks.begin(), dummy);
            int y = cache_mem[index]->no_of_blocks.size();
            cache_mem[index]->valid_bit[y - 1] = 1;
        }
    }
    else
    {
        Replacement(base_address, sets);
        // cout << "Replacement_finished" << endl;
    }
}
void FIFO(int base_address, int sets)
{
    int index;
    int tag;
    int bitoffset;
    int rock = base_address / (Block_size);
    index = rock % sets;
    tag = rock / sets;
    rock = rock * Block_size;
    if (write_policy == "WB")
    {
        if (cache_mem[index]->Dirty[0] == 0)
        {
            string x1 = cachefetch(rock, sets, Block_size);
            WRITE_TO_MEM(rock, Block_size, x1);
        }
    }
    cache_mem[index]->valid_bit.erase(cache_mem[index]->valid_bit.begin());
    // cache_mem[index]->Dirty.erase(cache_mem[index]->Dirty.begin());
    cache_mem[index]->Tag.erase(cache_mem[index]->Tag.begin());
    cache_mem[index]->no_of_blocks.erase(cache_mem[index]->no_of_blocks.begin());
    cache_allocate(base_address, sets);
}
void LRU(int base_address, int sets)
{
    int index;
    int tag;
    int bitoffset;
    int rock = base_address / (Block_size);
    index = rock % sets;
    tag = rock / sets;
    rock = rock * Block_size;
    if (write_policy == "WB")
    {
        if (cache_mem[index]->Dirty[0] == 0)
        {
            string x1 = cachefetch(rock, sets, Block_size);
            WRITE_TO_MEM(rock, Block_size, x1);
        }
    }
    cache_mem[index]->valid_bit[cache_mem[index]->valid_bit.size() - 1] = 0;
    // cache_mem[index]->Dirty.erase(cache_mem[index]->Dirty.end()-1);
    cache_mem[index]->Tag.erase(cache_mem[index]->Tag.end() - 1);
    cache_mem[index]->no_of_blocks.erase(cache_mem[index]->no_of_blocks.end() - 1);
    cache_allocate(base_address, sets);
}
void RANDOM(int base_address, int sets)
{
    int index;
    int tag;
    int bitoffset;
    int rock = base_address / (Block_size);
    index = rock % sets;
    tag = rock / sets;
    rock = rock * Block_size;
    bitoffset = base_address - rock;
    srand(time(NULL));
    int replacing_val = rand() % Associtivity;
    if (write_policy == "WB")
    {
        if (cache_mem[index]->Dirty[replacing_val] == 0)
        {
            string x1 = cachefetch(rock, sets, Block_size);
            WRITE_TO_MEM(rock, Block_size, x1);
        }
    }
    cache_mem[index]->valid_bit.erase(cache_mem[index]->valid_bit.begin() + replacing_val);
    cache_mem[index]->Dirty.erase(cache_mem[index]->Dirty.begin() + replacing_val);
    cache_mem[index]->Tag.erase(cache_mem[index]->Tag.begin() + replacing_val);
    cache_mem[index]->no_of_blocks.erase(cache_mem[index]->no_of_blocks.begin() + replacing_val);
    cache_allocate(base_address, sets);
}
void Replacement(int base_address, int sets)
{
    if (Replacement_Policy == "FIFO")
    {
        FIFO(base_address, sets);
    }
    if (Replacement_Policy == "LRU")
    {
        LRU(base_address, sets);
    }
    if (Replacement_Policy == "RANDOM")
    {
        RANDOM(base_address, sets);
    }
}

void WRITE_TO_MEM(int base_address, int block_size, string data)
{
    for (int i = 0; i < block_size; i++)
    {
        mem[base_address + i] = data.substr(i, 2);
    }
}
string cachefetch(int base_address, int sets, int data_size)
{
    int index;
    int Tag;
    int bitoffset;
    int rock = base_address >> (int)(log2(Block_size));
    index = rock % sets;
    Tag = rock / sets;
    rock = rock * Block_size;
    bitoffset = base_address - rock;
    string output;
    int replacing = 0;
    
    for (int i = 0; i < cache_mem[index]->Tag.size(); i++)
    {
        if ((cache_mem[index]->valid_bit[i] == 1) && (cache_mem[index]->Tag[i] == Tag))
        {
            if (bitoffset + data_size - 1 > Block_size)
            {
                cout << "Unaligned memory access its an error " << endl;
                return "null";
            }

            else
            {
                replacing = i;
                for (int j = bitoffset + data_size - 1; j >= (bitoffset); j--)
                {
                    output = output + cache_mem[index]->no_of_blocks[i][j];
                }
            }
        }
    }

    // cout<<output<<" "<<replacing<<endl;;
    if (Replacement_Policy == "LRU")
    {
        int valid = cache_mem[index]->valid_bit[replacing];
        int Dirty = cache_mem[index]->Dirty[replacing];
        int Tag1 = cache_mem[index]->Tag[replacing];

        vector<string> dummy;
        for (int i = 0; i < cache_mem[index]->no_of_blocks[replacing].size(); i++)
        {
            //  output=output+cache_mem[index]->no_of_blocks[replacing][i];
            dummy.push_back(cache_mem[index]->no_of_blocks[replacing][i]);
        }
        // cout<<valid<<" "<<Dirty<<" "<<Tag1<<" "<<endl;
        cache_mem[index]->valid_bit.erase(cache_mem[index]->valid_bit.begin() + replacing);
        cache_mem[index]->Dirty.erase(cache_mem[index]->Dirty.begin() + replacing);
        cache_mem[index]->Tag.erase(cache_mem[index]->Tag.begin() + replacing);
        cache_mem[index]->no_of_blocks.erase(cache_mem[index]->no_of_blocks.begin() + replacing);

        cache_mem[index]->valid_bit.insert(cache_mem[index]->valid_bit.begin(), valid);
        cache_mem[index]->Dirty.insert(cache_mem[index]->Dirty.begin(), Dirty);
        cache_mem[index]->no_of_blocks.insert(cache_mem[index]->no_of_blocks.begin(), dummy);
        cache_mem[index]->Tag.insert(cache_mem[index]->Tag.begin(), Tag1);
    }
    return output;
}
void cachewrite(int base_address, string s, int sets, int datasize)
{
    int index;
    int Tag;
    int bitoffset;
    int rock = base_address >> (int)(log2(Block_size));
    index = rock % sets;
    Tag = rock * sets;
    rock = rock * Block_size;
    bitoffset = base_address - rock;
    int r1 = 0;
    for (int i = 0; i < cache_mem[index]->Tag.size(); i++)
    {
        if (cache_mem[index]->Tag[i] == Tag)
        {
            r1 = i;
        }
    }
    string s1 = cachefetch(base_address, sets, datasize);
    // cout<<s1<<s<<endl;

    cache_mem[index]->Dirty[r1] == 0;
 //   cout << "hi" << cache_mem[index]->Dirty[r1] << endl;
    for (int i = 0; i < datasize; i++)
    {
        cache_mem[index]->no_of_blocks[r1][i] = s.substr(2 * (datasize - 1 - i), 2);
    }
}
void cache_stats()
{
    cout << "D-cache statistics:Accesses=" << Total_Access << ",Hits: " << Hits << ",Misses: " << Misses << ",Hit ratio: ";
    if (Total_Access != 0)
    {
        printf("%.2f", 1.0 * Hits / Total_Access);
    }
    else
    {
        printf("0.00");
    }
    cout << endl;
}
void cache_disable()
{
    for (int i = 0; i < cache_mem.size(); i++)
    {
        cache_mem.pop_back();
    }
}
