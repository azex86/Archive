#include "archive.hpp"
namespace fs = std::filesystem;




size_t getFileSize(string path)
{
    
    ifstream in(path);
    in.seekg(0,std::ios::end);
    size_t size = in.tellg();
    in.close();
    return size;
    
}

string getFileName(string path)
{
    size_t i1 = path.find_last_of("/"),
        i2 = path.find_last_of("\\");
    size_t index = (i1!=string::npos && i2!=string::npos)?((i1 > i2) ? i1 : i2):((i1!=string::npos)?i1:i2);
    if (index == string::npos)
            return path;

    return path.substr(index+1);
}
string getFolderName(string path)
{
    if (path[path.length() - 1] == '/' || path[path.length() - 1] == '\\')
        path.pop_back();
    size_t index = path.find_last_of("/");
    if (index == std::string::npos)
        index = path.find_last_of("\\");
    if (index == string::npos)
        return path;
    return path.substr(index + 1);
}

string join(string path1, string path2)
{
#ifdef _WIN32
    if (path1[path1.length() - 1] == '/' || path1[path1.length() - 1] == '\\')
    {
        if (path2[0] == '/' || path2[0] == '\\')
            path1.pop_back();
        return path1 + path2;
    }
    else if (path2[0] == '/' || path2[0] == '\\')
    {
        return path1 + path2;
    }
    else
        return path1 + '\\' + path2;
#else
    if (path1[path1.length() - 1] == '/' )
    {
        if (path2[0] == '/' )
            path1.pop_back();
        return path1 + path2;
    }
    else if (path2[0] == '/')
    {
        return path1 + path2;
    }
    else
        return path1 + '/' + path2;
#endif // _WIN32

}




Archive Archive::loadFromFolder(string folderPath)
{
    if (verbose)
        cout << "chargement des donnees d'une archive depuis le dossier : " << folderPath << endl;
    Archive archive;
    archive.folderName = getFolderName(folderPath);
    for (const auto& entry : fs::directory_iterator(folderPath)) {

        if (entry.is_directory())
        {
            if (verbose)
                cout << "Recursion de l'archivage dans le dossier " << entry.path().string() << endl;
            archive.folders.push_back(Archive::loadFromFolder(entry.path().string()));
        }
        else if (entry.is_regular_file())
        {
            Fichier file;
            file.path = entry.path().string();
            file.fileName = getFileName(file.path);
            file.pos = 0;
            file.size = getFileSize(file.path);
            archive.files.push_back(file);
            if (verbose)
                cout << "Ajout de " << file.path << " : " << file.size << " octets" << endl;
        }
    }

    return archive;
}




char* Fichier::read()
{
    char* retour = new char[this->size];
    ifstream in(this->path);
    in.seekg(this->pos);
    in.read(retour, this->size);
    in.close();
    return retour;
}

Archive Archive::loadFromFile(string filePath)
{
 
    ifstream archiveFile(filePath,ios::binary);
    Archive temp = loadFromFile(archiveFile,filePath);
    archiveFile.close();
    return temp;
}

Archive Archive::loadFromFile(ifstream& in,const string& inPath)
{

    Archive retour;
    size_t nombreDeFichiers = 0, nombreDeDossier = 0;
    in >> retour.folderName
        >> nombreDeFichiers
        >> nombreDeDossier;
   

    for (; nombreDeFichiers > 0; nombreDeFichiers--)
    {
        Fichier temp;
        in >> temp.fileName
            >> temp.size;
        //on avance de 1 octets pour compenser le 'endl' qui est ignor�e par 'in >>' mais pas par '.read'
        in.seekg(1, ios::cur);
        temp.path = inPath;
        temp.pos = in.tellg();
        in.seekg(temp.size,ios::cur);
        retour.files.push_back(temp);
    }
    for (; nombreDeDossier > 0; nombreDeDossier--)
    {
        retour.folders.push_back(Archive::loadFromFile(in, inPath));
    }
    return retour;
}



void Archive::create(string destFilePath)
{
   
    ofstream out(destFilePath,ios::binary);
    this->create(out);
    out.close();
}

void Archive::create(ofstream& out)
{
    out << folderName << endl
        << files.size() << endl
        << folders.size() << endl;
   

    for (size_t i = 0; i < this->files.size(); i++)
    {
        if (verbose)
            cout <<"write : " <<files[i].path <<" => "<< files[i].fileName << " : " << files[i].size << " octets" << endl;
        out << files[i].fileName << endl
            << files[i].size << endl;
       
        char* data = files[i].read();
        out.write(data, files[i].size);
        delete[] data;
    }
    for (Archive& folder : this->folders)
    {
        folder.create(out);
    }
}

void Archive::extract(string destFolderPath)
{
    destFolderPath = join(destFolderPath, this->folderName);
    fs::create_directory(destFolderPath);
    for (Fichier& file : this->files)
    {
        if (verbose)
            cout << "extraction  : " << file.fileName <<" : " <<file.size <<" octets" << endl
            <<"dans " << join(destFolderPath, file.fileName) <<endl;
        ofstream out(join(destFolderPath, file.fileName));
        char* data = file.read();
        out.write(data,file.size);
        delete[] data;
        out.close();
    }
    for (Archive& a : this->folders)
    {
        a.extract(destFolderPath);
    }
}

void Archive::print(string startPath)const
{
    if (startPath == "")
        startPath = this->folderName;
    startPath = join(startPath,"/");
    cout << startPath << endl;
    for (const Fichier& f : this->files)
        cout << startPath  << f.fileName << endl;

    for (const Archive& f : this->folders)
        f.print(startPath  + f.folderName);
}


int list(string source)
{
    if (verbose)
        cout << "chargement des donnees d'une archive depuis le fichier : " << source << endl;
    Archive archive = Archive::loadFromFile(source);
    if (verbose)
        cout << "enumeration du contenu de l'archive" << endl;
    archive.print();
    return 0;
}

int extract(string source, string dest)
{
    if (verbose)
        cout << "chargement des donnees d'une archive depuis le fichier : " << source << endl;
    Archive archive = Archive::loadFromFile(source);
    try
    {
        if (verbose)
            cout << "creation du dossier de sortie : " <<dest << endl;
        fs::create_directory(dest);
    }
    catch (exception e)
    {
        cerr << e.what() << endl;
    }
    if (verbose)
        cout << "extraction de l'archive : " << source <<" dans le dossier "<<dest << endl;
    archive.extract(dest);
    return 0;
}

int create(string source, string dest)
{
    if (verbose)
        cout << "chargement du dossier : " << source << endl;
    Archive archive = Archive::loadFromFolder(source);
    if (verbose)
        cout << "creation de l'archive : " <<dest << endl;
    archive.create(dest);
    
    return 0;
}