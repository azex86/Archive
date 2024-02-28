#include "archive.hpp"




enum Instruction
{
    Create,
    Extract,
    List,
    None
};

int main(int argc,char** argv)
{


    /* 
        size_t size = 10000;
        char* data = new char[size];

        ofstream o("out");
        cout << o.tellp() << endl;
        o << size << endl << size << endl;
        cout << o.tellp() << endl;
        o.write(data, size);
        cout << o.tellp() << endl;
        o << size << endl;
        cout << o.tellp() << endl;
        o.close();

        size_t size2 = 0, size3 = 0, size4 = 0;
        ifstream in("out");
        cout << in.tellg() << endl;
        in >> size2 >> size4;
        in.seekg(2, ios::cur);
        cout << in.tellg() << endl;
        char* data2 = new char[size2];
        in.read(data2, size2);
        cout << in.tellg() << endl;
        in >> size3;
        cout << in.tellg() << endl;
        in.close();

        int value = memcmp(data, data2, (size > size2) ? size2 : size);
        return 0;
    */
        
    create("D:/Plan_3D/Gaufrier_a_cire", "test.arch");
    list("test.arch");
    extract("test.arch", "testExtraction/");
    return 0;
    bool verbose = false;
    string sourcePath = "";
    string destPath = "";
    Instruction action = Instruction::None;

    for(int i = 0;i<argc;i++)
    {
        string temp = argv[i];

        if(temp == "--verbose" || temp == "-v")
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
                cerr <<"Erreur argument not understood !" <<endl;
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