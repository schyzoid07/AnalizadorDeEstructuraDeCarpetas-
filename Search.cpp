#include "Search.hpp"
/*
void Search::searchName() {

  fs::current_path(getRoot());
  cout << endl;
  string op;
  cout << "Introduce nombre del archivo a buscar ";
  cin >> op;
}

void Search::searchFile() {}

fs::path Search::getRoot() { return root; }
*/

// Presentar la estructura de archivos en forma de árbol (completa o solo
// carpetas)

Search::Search(const fs::path &root) : root(root) {}

void Search::presentTree(bool onlyDirectories) const {
  presentTreeHelper(root, "", onlyDirectories);
}
// Buscar archivos por nombre
void Search::searchByName(const string &name) const {
  vector<fs::directory_entry> results;
  searchHelper(
      root,
      [&name](const fs::directory_entry &entry) {
        return entry.path().filename().string() == name;
      },
      results);
  displayResults(results);
}

// Buscar archivos por extensión
void Search::searchByExtension(const string &extension) const {
  vector<fs::directory_entry> results;
  searchHelper(
      root,
      [&extension](const fs::directory_entry &entry) {
        return entry.path().extension().string() == extension;
      },
      results);
  displayResults(results);
}

void Search::presentTreeHelper(const fs::path &path, const std::string &prefix,
                               bool onlyDirectories) const {
  for (const auto &entry : fs::directory_iterator(path)) {
    std::cout << prefix << (entry.is_directory() ? "[D] " : "[F] ")
              << entry.path().filename().string() << '\n';
    if (entry.is_directory()) {
      presentTreeHelper(entry.path(), prefix + "  ", onlyDirectories);
    } else if (!onlyDirectories) {
      presentTreeHelper(entry.path(), prefix + "  ", onlyDirectories);
    }
  }
}

void Search::searchHelper(
    const fs::path &path,
    const std::function<bool(const fs::directory_entry &)> &predicate,
    std::vector<fs::directory_entry> &results) const {
  for (const auto &entry : fs::recursive_directory_iterator(path)) {
    if (predicate(entry)) {
      results.push_back(entry);
    }
  }
}

void Search::displayResults(
    const std::vector<fs::directory_entry> &results) const {
  for (const auto &entry : results) {
    std::cout << "Archivo: " << entry.path().filename().string()
              << " | Directorio: " << entry.path().parent_path().string()
              << " | Tamaño: " << fs::file_size(entry.path()) << " bytes"
              << '\n';
  }
}
