#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <queue>
using namespace std;
void ErrorRedirect(string fileName)
{
    int errFD = open(fileName.c_str(), O_WRONLY | O_CREAT , 0666);
    if (errFD >= 0)
    {
        
        if (dup2(errFD, 3) < 0)
            perror("error  Redirection Failed:");
    }
    else
        perror("Failed to Open Output File for Redirection:");
}
void inputRedirect(string fileName)
{
    int InputFD = open(fileName.c_str(), O_RDONLY);
    if (InputFD >= 0)
    {
        if (dup2(InputFD, 0) < 0)
            perror("Input Redirection Failed:");
    }
    else
        perror("Failed to Open Input File for Redirection:");
}

void OutputRedirect(string fileName)
{
    int OutFD = open(fileName.c_str(), O_WRONLY | O_CREAT, 0666);
    if (OutFD >= 0)
    {

        if (dup2(OutFD, 1) < 0)
            perror("Output Redirection Failed:");
    }
    else
        perror("Failed to Open Output File for Redirection:");
}

class Shell
{
private:
    string input;
    vector<string> commands;
    bool cTerm;
    static int count;
    int PipeCount;
    deque<pair<int, string>> history;

public:
    vector<string> Tokenizer(string command)
    {

        vector<string> Tokens;

        string temp;
        for (int i = 0; i < command.length(); i++)
        {
            temp += command[i];

            if (command[i + 1] == '|' || command[i + 1] == '\0')
            {
                if (command[i + 1] == '|')
                    PipeCount++;
                Tokens.push_back(temp);
                temp = "";
                i++;
            }
        }

        return Tokens;
    }
    Shell()
    {
        PipeCount = 0;
        cTerm = 0;
    }
    ~Shell()
    {
    }
    void showHistory()
    {
        {
            for (auto it = history.crbegin(); it != history.crend(); it++)
                cout << (*it).first << "-" << (*it).second << endl;
        }
    }
    int GetInput()
    { // returns 0 if exit and 1 if continue and 2 if command
        PipeCount = 0;
        cout << "Welcome to My Shell V1.1 (Now Supports History)\n";
        cout << "->";
        getline(cin, input);
        // input="cat <input.txt";
        if (input == "!!")
        {
            if (!history.empty())
            {
                input = history.back().second;
                cout << input << endl;
            }
            else
            {
                cout << "History is empty";
                return 1;
            }
        }
        else if (input.length() != 0)
        {
            int num;
            if (input[0] == '!')
            {
                try
                {
                    num = stoi(input.substr(1, input.length() - 1));
                    /* code */
                }
                catch (const std::exception &e)
                {
                    cout << "Invalid Combination of commands or no such commands exist\n";
                }

                cout << num << "-";
                bool found = false;
                for (auto it = history.crbegin(); it != history.crend(); it++)
                {
                    if ((*it).first == num)
                    {
                        input = (*it).second;
                        cout << input << endl;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    return 1;
                    cout << "Not Found in History";
                }
            }
        }
        if (history.size() >= 10)
        {
            history.pop_front();
        }
        history.push_back(pair<int, string>(++count, input));
        if (strcmp("exit", input.c_str()) == 0)
            return 0;
        if (input == "history")
        {
            showHistory();
            return 1;
        }
        cTerm = false;
        if (input[input.size() - 1] == '&')
        {
            cTerm = 1;
            input.pop_back();
        }
        commands = Tokenizer(input);
        cout << endl;
          return 2;
    }
    void Execute()
    {
        while(1){
        int commandType = GetInput();
        if(commandType==0)
         break;
        else if(commandType==1)
        continue;

        // for(auto i : commands)
        // cout<<i<<endl;

        vector<int *> PipeFd;
        PipeFd.resize(PipeCount);
        for (int i = 0; i < PipeCount; i++)
        {
            PipeFd[i] = new int[2];
            if (pipe(PipeFd[i]) < 0)
                perror("pipe Failed");
        }
        //  cout<<"No of Pipe Comnmands: "<<commands.size()<<endl;

        for (int ProccesCount = 0; ProccesCount < commands.size(); ProccesCount++)
        {

            pid_t id = fork();
            // int id=0;
            if (id < 0)
                perror("fork Failed");
            else if (id == 0)
            {
                stringstream cStream(commands[ProccesCount]);
                string CommandName;
                cStream >> CommandName;
                string arguments;
                string temp;
                int argCount = 0;
                while (cStream >> temp)
                {
                    if (temp[0] == '<')
                    {
                        temp.erase(0, 1);
                        inputRedirect(temp);
                    }
                    else if (temp[0] == '>')
                    {
                        temp.erase(0, 1);
                        OutputRedirect(temp);
                    }
                      else if(temp[0]=='2' && temp[1]=='>'){
                    temp.erase(0,2);
                    ErrorRedirect(temp);
                }
              
                    else
                    {
                        arguments += temp + " ";
                        argCount++;
                    }
                }
                // cout<<commands[ProccesCount];
                // sleep(5);l

                // cout<<endl<<ProccesCount<<"\t" <<CommandName<<" " <<arguments<<endl;
                // sleep(1);
                char **args = NULL;
                args = new char *[argCount + 2];
                args[0] = new char(CommandName.size() + 1);
                strcpy(args[0], CommandName.c_str());
                if (arguments.c_str() != 0)
                {
                    stringstream arg_Stream(arguments);
                    string temp2;
                    for (int i = 1; i <= argCount; i++)
                    {
                        arg_Stream >> temp2;
                        args[i] = new char[temp2.size() + 1];
                        strcpy(args[i], temp2.c_str());
                    }
                }
                args[argCount + 1] = NULL;

                if (ProccesCount != 0) // It will run for middle and end commands.
                {
                    // Input Redirection.
                    if (dup2(PipeFd[ProccesCount - 1][0], 0) < 0)
                        perror("inp red dup failed:");
                }
                if (ProccesCount != commands.size() - 1) // It will run for first and middle.
                {
                    // Output Redirection.
                    if (dup2(PipeFd[ProccesCount][1], 1) < 0)
                        perror("out red dup failed:");
                }
                for (int i = 0; i < PipeFd.size(); i++)
                {

                    close(PipeFd[i][0]);
                    close(PipeFd[i][1]);
                }

                if (execvp(CommandName.c_str(), args))
                perror("Invalid Command");
            }
            else // parent
            {
                if (ProccesCount != commands.size() - 1)
                {
                    close(PipeFd[ProccesCount][1]);
                }
                if (!cTerm)
                     wait(NULL);
            }
        }
        // Execute();
        for (int i = 0; i < commands.size(); i++)
        {
            wait(NULL);
        }
                cout << "\n->\n";
    }

    }
};
int Shell::count = 0;

int main()
{
    Shell Myshell;
        Myshell.Execute();
    
}
