#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

extern bool verbose;
using namespace std;

//retourne la taille du chier [path] en octets
size_t getFileSize(string path);
//retourne le nom du fichier [path] ex: getFileName("/home/user/data.txt"); -> "data.txt"
string getFileName(string path);
//retourne le nom du dossier [path] ex: getFileName("/home/user/data"); -> "data"
string getFolderName(string path);
//retourne ule chemin combinant les deux chemin : ex join("/home/user","data.txt");-> "/home/user/data.txt"
string join(string path1, string path2);

struct Fichier
{
    size_t size;
    string fileName;

    //chemin vers le fichier
    string path;
    //position du début des données du fichier dans le fichier [this->path]
    size_t pos;

    //retourne un pointeur sur les données du fichier, la mémoire doit-être libérée !
    char* read();
    
};

/*
        Quelques explication sur la mise en forme de l'archive sur le disque.
            un objet Archive écris dans un fichier
        ----------------------------
        |   folderName          \n  |
        |   nombre de fichiers  \n  |
        |   nombres De dossiers \n  |
        |---------------------------|
        |   fileName            \n  |   -> nombreDeFichiers fois cette structure
        |   fileSize            \n  |
        |   data                    | -> les données du fichier  au format binaire soit fileSize octets 
        |---------------------------|
        |   ......................  |   -> on repète l'ensemble de la structure récusivement
        |---------------------------|   -> nombreDeDossier fois la structure Archive

    Notes:
        le fichier est au format binaire
        les données (string + int) sont écrites en texte terminé par '\n'
        seule le contenu des fichiers est écrit en binaire pur sans '\n' à la fin 
*/


struct Archive
{
    string folderName;
    vector<Fichier> files;
    vector<Archive>folders;

    //instancie un objet archive depuis un fichier archive en vue d'une extraction
    static Archive loadFromFile(string filePath);
    static Archive loadFromFile(ifstream& in,const string& inPath);
    //instancie un objet archive depuis un dossier en vue d'un archivage
    static Archive loadFromFolder(string folderPath);
    
    //fabrique un fichier archive
    void create(string destFilePath);
    void create(ofstream& out);
    /*
    * extrait l'Archive dans un dossier
    *on assume que [destFolderPath] est un dossier existant
    */
    void extract(string destFolderPath);
    // écris sur la sortie standard le contenu de l'archive de manière récursive
    void print(string startPath = "") const;
};




/*
liste le contenu d'une archive dans la sortie standard
*/
int list(string source);

/*
* extrait l'archive [source] et écris son contenu dans le dossier [dest] (celui-ci is created)
*/
int extract(string source,string dest);

/*
* archive le dossier [source] dans le fichier [dest]
*/
int create(string source,string dest);