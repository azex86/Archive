#include "archive.hpp"

int help()
{
    cout << " help message !\n\
    Archive.exe [arg] [source] [destination]\
args:\
    -h --help : show this help message\
    -v --verbose : enable print not usable\
    -c --create : create an archive from [source] to [destination]\
    -l --list : show the content of the archive\
    -x --extract : extract the content of the archive([source]) to [destination]\
                                [destination] should be a folder which will be created";
}   


enum Instruction
{
    Create,
    Extract,
    List,
    None
};

int main(int argc,char** argv)
{


    bool verbose = false;
    string sourcePath = "";
    string destPath = "";
    Instruction action = Instruction::None;

    for(int i = 0;i<argc;i++)
    {
        string temp = argv[i];
        if (temp == "--help" || temp == "-h")
            return help();
        else if(temp == "--verbose" || temp == "-v")
            verbose = true;
        else if(temp == "-x" || temp == "--extract")
            action = Extract;
        else if(temp == "--create" || temp == "-c")
            action = Instruction::Create;
        else if(temp == "--list" || temp =="-l")
            action = Instruction::List;
        else if(sourcePath == "")
            sourcePath = argv[i];
        else if(destPath == "")
            destPath = argv[i];
        else
            {
                cerr <<"Erreur argument not understood ! : " <<temp <<endl;
                return -1;
            }
            
    }
    try
    {
        switch (action)
        {
            case Instruction::None:
                break;
            case Instruction::List:
                return list(sourcePath);
            case Instruction::Create:
                return create(sourcePath,destPath);
            case Instruction::Extract:
                return extract(sourcePath,destPath);
            default:
                break;
        }
    }catch(exception e)
    {
        cerr <<"An exception was caught !" <<endl
         <<e.what() <<endl
         <<"Closing the app ....." <<endl;
    }
    return 0;
}