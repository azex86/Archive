# ExerciceArchive

c'est un executable en ligne de commande qui produit une "archive"
on entend par archive un fichier qui regroupe un ensemble de fichier/dossier soit toute une arborescence
zip et tar sont des archiveur connu


notre archiveur fonctionne comme ceci : 

création d'une archive :
	Archive.exe --create Documents mesDocumentsArchive.archive

lister le contenu d'une archive
	Archive.exe --list mesDocumentsArchive.archive

extraire le contenu d'une archive
	Archive.exe --extract mesDocumentsArchive.archive DocumentsBis

l'exécutable se trouve dans x64/Release/Archive.exe