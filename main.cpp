#include <bits/stdc++.h>
#include "cache_simulator.hh"
using namespace std;
ofstream ouputfile1;
int cache_enabled;
int cache_simul = 0;
int main()
{
 
  string s;
  string command;
  string enable;
  string input_file;
 int b = 0;
  int c = 0;
  
  while (1)
  {
    int a = 0;
    Hits = 0;
    Misses = 0;
    Total_Access = 0;
    string config_file_name;

    if (b == 0)
    {

      getline(cin, enable);
    }
    //  cout<<enable<<endl;
    stringstream con(enable);
    con >> config_file_name;
    if (config_file_name == "cache_sim")
    {
      con >> config_file_name;
      con >> config_file_name;
      //  cout<<config_file_name<<endl;
      ifstream config_file(config_file_name);
      string temp; // dummy string
      int a;
      int counter = 0;
      config_file >> a;
      if (counter == 0)
      {
        cache_size = a;
        // cout<<cache_size<<endl;
        counter++;
      }
      config_file >> a;
      if (counter == 1)
      {
        Block_size = a;
        // cout<<Block_size<<endl;
        counter++;
      }
      config_file >> a;
      if (counter == 2)
      {
        Associtivityf = a;
        if (Associtivityf == 0)
        {
          Associtivity = cache_size / Block_size;
        }
        else
        {
          Associtivity = Associtivityf;
        }
        // cout<<Associtivity<<endl;
        counter++;
      }
      config_file >> temp;
      if (counter == 3)
      {
        Replacement_Policy = temp;
        // cout<<Replacement_Policy<<endl;
        counter++;
      }
      config_file >> temp;
      if (counter == 4)
      {
        write_policy = temp;
        // cout<<write_policy<<endl;
      }
      cache_enabled = 1;
      initialise_cache();
    }
    else
    {
      input_file = enable;
      c = 1;
    }

    if (c == 0)
    {
      getline(cin, s);
    }
    // cout << s << Programcounter << endl;
    gs.push({"main", Programcounter / 4});
    memoryinitialisation();
    RegisterInitialisation();
    stringstream ss5(s);
    string s13;
    ss5 >> s13;
    if (s13 == "load")
    {
      ss5 >> s13;
      input_file = s13;
    }
    else
    {
      cout << "incorrect command and exited the program" << endl;
      return 0;
    }
    string outputfile;
    ifstream inputfiled(input_file);
    ifstream input_file2(input_file);
    outputfile = input_file.substr(0, input_file.size() - 2) + ".output";
    string input;

    ofstream ouputfile1(outputfile);
    int no_of_lines_till_text = 0;
    int line_number = 0;
    while (getline(inputfiled, input))
    {
      if (input == ".data")
      {
        no_of_lines_till_text++;
        while (input != ".text")
        {

          getline(inputfiled, input);
          no_of_lines_till_text++;
          stringstream ss(input);
          string datatype;
          ss >> datatype;
          if (datatype == ".dword")
          {
            datasection(input, 8);
          }
          if (datatype == ".word")
          {
            datasection(input, 4);
          }
          if (datatype == ".half")
          {
            datasection(input, 2);
          }
          if (datatype == ".byte")
          {
            datasection(input, 1);
          }
        }
      }

      if (input != ".text")
      {
        string first;
        line_number++;
        stringstream strings1(input);
        strings1 >> first;
        if (first[first.size() - 1] == ':')
        {
          first = first.substr(0, first.size() - 1);
          if (Label_line_number.find(first) != Label_line_number.end())
          {
            cout << "error in the line the no" << line_number << "the program gets terminated" << endl;
            return 0;
          }
          Label_line_number[first] = (line_number);
        }
      }
    }
    // cout<<no_of_lines_till_text<<endl;
    int total_no_of_instructions = line_number;
    line_number = 0;
    map<int, string> PC_instruction;

    while (getline(input_file2, input))
    {

      if (input == ".data")
      {
        while (input != ".text")
        {
          getline(input_file2, input);
          // cout<<input<<endl;
        }
      }
      if (input != ".text")
      {

        line_number++;
        int instruction_start_index = 0;
        if (islabel(input))
        {
          instruction_start_index = label_position(input) + 2;
        }
        PC_instruction[(line_number - 1) * 4] = input.substr(instruction_start_index);
      }
    }

    while (1)
    {
      if ((c12 == 1 || Programcounter == (total_no_of_instructions) * 4) && (a != 1))
      {

        for (int i = 0; i < access_lines.size(); i++)
        {
          ouputfile1 << access_lines[i] << endl;
        }
        c12 = 0;
        a++;
      } // out<<access_lines.size()<<endl;
      getline(cin, command);
      if (command == "regs")
      {
        display_register_values();
      }
      if (command == "run")
      {
        if (gs.empty())
          gs.push({"main", Programcounter / 4});
        execute(PC_instruction, total_no_of_instructions);
      }
      if (command == "step")
      {
        // cout<<indicator<<endl;
        if (indicator == 1)
        {
          // cout << "hello" << endl;
          Programcounter -= 4;
          indicator = 0;
        }
        if (gs.empty())
        {
          // cout<<"gqwe"<<endl;
          gs.push({"main", Programcounter / 4});
        }
        execute_one(PC_instruction[Programcounter], total_no_of_instructions);
      }

      if (command == "exit")
      {
        break;
      }
      if (command == "show-stack")
      {
        cout << "Callstack:" << endl;
        showstack(gs);
      }
      stringstream ss(command);
      string s3;
      ss >> s3;
      // cout<<s3<<endl;
      if (s3 == "break")
      {
        ss >> s3;
        breakpoints.push_back(stoi(s3) - no_of_lines_till_text);
        cout << "Breakpoint set at line " << stoi(s3) - no_of_lines_till_text << endl;
        cout << endl;
      }
      if (s3 == "mem")
      {
        ss >> s3;
        string s4;
        ss >> s4;
        cout << s3 << " " << s4 << endl;
        displaymem(s3, stoi(s4));
      }
      if (s3 == "del")
      {
        ss >> s3;
        ss >> s3;
        breakpoints.erase(remove(breakpoints.begin(), breakpoints.end(), stoi(s3) - no_of_lines_till_text), breakpoints.end());
        cout << "Breakpoint deleted at line" << stoi(s3) << endl;
        cout << endl;
      }
      if (s3 == "cache_sim")
      {
        string temp1;

        (ss >> temp1);
        if (temp1 == "enable" && (Programcounter == (total_no_of_instructions) * 4))
        {
          enable = " " + temp1;
          string s11;
          ss >> s11;
          enable = enable + " " + s11;
          enable = s3 + enable;
          b = 1;
          //c12 = 1;
          cache_disable();
          Programcounter = 0;
          cache_simul = 1;
          // cout<<"cache_simul"<<" "<<cache_simul<<endl;
          break;
        }
        else
        {
          if (temp1 == "status")
          {
            cache_details();
          }
          if (temp1 == "dump")
          {
            ss >> temp1;
            dumping(temp1);
          }
          if (temp1 == "stats")
          {
            cache_stats();
          }
          if (temp1 == "invalidate")
          {
            cache_invaildate();
          }
          if (temp1 == "disable")
          {
            cache_enabled = 0;
            cache_disable();
            //c12 = 1;
          }
          // cout << "we are in the middle of the program and cache is already enabled" << endl;
        }
      }
      if ((s3 == "load") && (cache_simul == 0))
      { // cout<<"hello"<<endl;
        string temp1;
        (ss >> temp1);
        input_file = temp1;
        c = 1;
       // c12 = 1;
      }
    }
  skip:
    // invalidating the cache by making the valid bit of the block to 0.
    if (command == "exit")
    {
      cache_disable();
      cout << "Exited the simulator" << endl;
      break;
    }
  }
}
