#include <filesystem>
#include <functional>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using namespace std;
class Search {
public:
  explicit Search(const fs::path &root);

  // Presentar la estructura de archivos en forma de árbol (completa o solo
  // carpetas)
  void presentTree(bool onlyDirectories = false) const;

  // Buscar archivos por nombre
  void searchByName(const std::string &name) const;

  // Buscar archivos por extensión
  void searchByExtension(const std::string &extension) const;

private:
  fs::path root;

  // Función auxiliar para presentar el árbol de archivos
  void presentTreeHelper(const fs::path &path, const std::string &prefix,
                         bool onlyDirectories) const;

  // Función auxiliar para buscar archivos
  void searchHelper(
      const fs::path &path,
      const std::function<bool(const fs::directory_entry &)> &predicate,
      std::vector<fs::directory_entry> &results) const;

  // Función para mostrar resultados de búsqueda
  void displayResults(const std::vector<fs::directory_entry> &results) const;
};
