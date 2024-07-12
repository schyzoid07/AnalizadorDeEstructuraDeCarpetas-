#include "Search.hpp"
#include <iostream>

int main() {
  std::string dir;
  std::cout << "Ingrese el directorio a analizar: ";
  std::cin >> dir;

  fs::path rootPath(dir);
  Search search(rootPath);

  int option;
  std::cout << "Seleccione una opción:\n"
            << "1. Presentar estructura de archivos (completa)\n"
            << "2. Presentar estructura de archivos (solo carpetas)\n"
            << "3. Buscar archivos por nombre\n"
            << "4. Buscar archivos por extensión\n";
  std::cin >> option;

  switch (option) {
  case 1:
    search.presentTree();
    break;
  case 2:
    search.presentTree(true);
    break;
  case 3: {
    std::string name;
    std::cout << "Ingrese el nombre del archivo a buscar: ";
    std::cin >> name;
    search.searchByName(name);
    break;
  }
  case 4: {
    std::string extension;
    std::cout << "Ingrese la extensión del archivo a buscar (incluyendo el "
                 "punto, e.g., .txt): ";
    std::cin >> extension;
    search.searchByExtension(extension);
    break;
  }
  default:
    std::cout << "Opción no válida.\n";
  }

  return 0;
}

