#include "Execute.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>



using namespace std;


Mandate::Mandate(){ }
Mandate::Mandate(string a, string b)
{
    
    this->Executeble = a;
    this->Connector = b;
    
}
void Mandate::Execute()
{

    int status;
    int count = 1;
    string command;
    string argument[10];
    
    char *str = const_cast<char *>(this->Executeble.c_str());
    char* token = strtok(str," ");
    while(token != NULL)
    {
        if(count == 1)
        {
            command = token;
            count++;
        }
        else
        {
            argument[count] = token;
            count++;
        }
        token = strtok(NULL, " ");
    }
    
    pid_t pid,wpid; 
    pid = fork();
    
    if ( pid < 0  ) 
    {   
        cout <<"Error running fork."<<endl;
       
    }
    if (pid == 0)//child
    {   
        
        if(count==2){
            char *args[2];
            args[0]  = (char*) command.c_str();
            args[1] = NULL;
            if(execvp(args[0], args) == -1){
                perror("Error!");
                exit(errno);
            }
        }else{
            char *args[count -1];
            args[0]  = (char*) command.c_str();
            int i = 1;
            while(count > 2){
                 args[i]  = (char*) argument[i+1].c_str();
                 i++;
                 count--;
            }
            args[i] = NULL;
            if( execvp(args[0], args) == -1){
                perror("Error!");
                exit(errno);
                
            }
            
        }
     
    }
    else//parent
    {   
        
         do{
              wait(&status);
              //if(WIFEXITED(status))
                //cout<<"child exited with = "<<WEXITSTATUS(status)<<endl;        
             if (WEXITSTATUS(status) > 0)
                  this->Flag = false;
             
             wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    
    }
}

void Mandate::setFlag(bool flag)
{
    this->Flag = flag;
}
bool Mandate::getFlag()
{
    return this->Flag;
}
void Mandate::setExecutable(string input)
{
    this->Executeble = input;
    
}
string Mandate::getExecutable(){
    return this->Executeble;
    
}
void Mandate::setConnector(string input){
    this->Connector = input;
    
}
string Mandate::getConnector(){
    return this->Connector;
    
}

/////////////////////////////////////////////
Command::Command(){}
Command::Command(Mandate* item){
    
    commands.push_back(item);
}
void Command::setTree(Base* item ,int i, Base*& out ){
   
   if ( i <  commands.size()-1  )
   {
      
        string connect = commands[i]->getConnector();
      
        if(connect == "&&")
        {       
                unsigned int n =i+1;
                //commands[i]->setChild() );
                 setTree(new And(item , commands[n]),n,out);
              
        }
        else if(connect == "||")
        {
            unsigned int n =i+1;
             setTree(new Or(item, commands[n]),n,out);
            
        }
        else if(connect == ";")
        {
            unsigned int n = i+1;
            if(connect != "end"){
                setTree(new Semicolon(item , commands[n]),n,out);
            }else{
                setTree(new Semicolon(item , NULL),n,out);
            }
             
            
        }
        
    }
    else{
        out = item;
 
    }
    
}
void Command::Execute(){
     
     Base * out;
     unsigned int i = 0;
     setTree(commands[0] , i, out);
     out->Execute();
     
}

void Command::setFlag(bool flag){
    
    this->Flag = flag;
    
}
bool Command::getFlag(){
    
    return this->Flag;
    
}
void Command::setCommand(Mandate* input){
            
    commands.push_back(input);
}

int Command::size(){
    
    return commands.size();
}

Mandate* Command::getCommand(int a){
    
    return (commands.at(a));
    
}

/////////////////////////////////////////////////////////////
//      AND Methods
////////////////////////////////////////////////////////////
And::And(){
    
}
And::And(Base* child1, Base* child2){
    this->child1 = child1;
    this->child2 = child2;
}
void And::Execute(){
    
    
   if (this->child1->Flag == true ){
        this->child1->Execute();
        
         
        if (this->child1->Flag == true ){
            
            this->child2->Execute();
           
            if( this->child2->Flag == false)
                this->Flag = false;

        }
        else
            this->Flag = false;
   }
    else
        this->Flag = false;    
        
}
void And::setFlag(bool flag){
    
    this->Flag = flag;
    
}
bool And::getFlag(){
    
    return this->Flag;
    
}
/////////////////////////////////////////////////////
//    OR methods 
////////////////////////////////////////////////////
Or::Or(Base* child1, Base* child2){
    this->child1 = child1;
    this->child2 = child2;
}
void Or::Execute(){
    
    
    child1->Execute();
    if ( this->child1->Flag == false ){
         this->child2->Execute();
         
         if (this->child2->Flag == false)
            this->Flag = false;
    }       
    else
        this->Flag = false;
}
void Or::setFlag(bool flag){
    Flag = flag;
    
}
bool Or::getFlag(){
    return Flag;
    
}
//////////////////////////////////////////////////////
//       Semicolon Methods
///////////////////////////////////////////////////////
Semicolon::Semicolon(Base* child1, Base* child2){
    this->child1 = child1;
    this->child2 = child2;
    if(this->child2->Flag == false)
        this->Flag = false;
}
void Semicolon::Execute(){
     child1->Execute();
     child2->Execute();
}
void Semicolon::setFlag(bool flag){
    this->Flag=flag;    

   
}
 bool Semicolon::getFlag(){
     
     return Flag;
     
 }