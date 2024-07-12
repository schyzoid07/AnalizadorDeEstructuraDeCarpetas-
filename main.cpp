#include "Search.hpp"
#include <iostream>

int main() 
{
  string dir;
  cout << "Ingrese el directorio a analizar: ";
  cin >> dir;

  fs::path rootPath(dir);
  Search search(rootPath);

  int option;
  cout << "\t\t\t\tSeleccione una opción:\n"
            << "\t\t\t\t1. Presentar estructura de archivos (completa)\n"
            << "\t\t\t\t2. Presentar estructura de archivos (solo carpetas)\n"
            << "\t\t\t\t3. Buscar archivos por nombre\n"
            << "\t\t\t\t4. Buscar archivos por extensión\n"
            <<"\t\t\t\t5. Exportar HTML\t\t\t\t\n:";
  cin >> option;

  switch (option)
   {
  case 1:
    search.presentTree();
    break;
  case 2:
    search.presentTree(true);
    break;
  case 3:
   {
    string name;
    cout << "Ingrese el nombre del archivo a buscar: ";
    cin >> name;
    search.searchByName(name);
    break;
  }
  case 4:
   {
    string extension;
    cout << "Ingrese la extensión del archivo a buscar (incluyendo el "
                 "punto, e.g., .txt): ";
    cin >> extension;
    search.searchByExtension(extension);
    break;
  }
  case 5:
   {
    search.exportToHTML("archivoHTML");
    break;
   }
  default:
    cout << "Opción no válida.\n";
  }
  system("pause");
  return 0;
}

