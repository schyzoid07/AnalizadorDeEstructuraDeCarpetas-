#include "Search.hpp"
#include "Gui.hpp"

Search::Search(const fs::path &root) : root(root) {}


void Search::presentTree(bool onlyDirectories) const
 {
  presentTreeHelper(root, "", onlyDirectories);
}

void Search::searchByName(const string &name) const
 {
  vector<fs::directory_entry> results;
  searchHelper(
      root,
      [&name](const fs::directory_entry &entry) 
      {
        return entry.path().filename().string() == name;
      },
      results);
  displayResults(results);
}

void Search::searchByExtension(const string &extension) const 
{
  vector<fs::directory_entry> results;
  searchHelper(root,[&extension](const fs::directory_entry &entry) 
  {
        return entry.path().extension().string() == extension;
      },
      results);
  displayResults(results);
}

void Search::presentTreeHelper(const fs::path &path, const std::string &prefix, bool onlyDirectories) const 
 {
  for (const auto &entry : fs::directory_iterator(path)) 
  {
    std::cout << prefix << (entry.is_directory() ? "[D] " : "[F] ")
              << entry.path().filename().string() << '\n';
    if (entry.is_directory())
     {
      presentTreeHelper(entry.path(), prefix + "  ", onlyDirectories);
    } else if (!onlyDirectories) 
    {
      std::cout << prefix << "  " << entry.path().filename().string() << '\n';
    }
  }
}

void Search::searchHelper( const fs::path &path, const std::function<bool(const fs::directory_entry &)> &predicate, std::vector<fs::directory_entry> &results) const 
    {
  for (const auto &entry : fs::recursive_directory_iterator(path)) 
  {
    if (fs::exists(entry.path()) && fs::is_regular_file(entry.path())) 
    {
      if (predicate(entry)) {
        results.push_back(entry);
      }
    }
  }
}

void Search::displayResults( const std::vector<fs::directory_entry> &results) const 
{
  for (const auto &entry : results) 
  {
    if (fs::exists(entry.path()) && fs::is_regular_file(entry.path())) 
    {
      std::cout << "Archivo: " << entry.path().filename().string()
                << " | Directorio: " << entry.path().parent_path().string()
                << " | Tamaño: " << fs::file_size(entry.path()) << " bytes"
                << '\n';
    }
  }
}


void Search::exportToHTML(const string &filename) 
 {
  /*Se crea el objeto de archivo con extensio HTML*/
  ofstream htmlFile(filename + ".html");

  /*Se verifica si el archivo creado esta corrupto o no*/
  if (!htmlFile)
   {
    cerr << "Error: no se pudo crear el archivo HTML." << endl;
    return;
   }

  /*Empieza el encabezado tipo html*/
  htmlFile << "<html><head><title>Archivo de búsqueda</title></head><body>" << endl;
  htmlFile << "<h1>Archivo de búsqueda</h1>" << endl;

  vector<fs::directory_entry> results;
  searchHelper(root, [](const fs::directory_entry &entry) { return true; }, results);

  htmlFile << "<p>Cantidad de archivos: " << results.size() << "</p>" << endl;

  int totalSize = 0;
  int maxSize = 0;
  string largestFile;

/* Este bucle ayuda a sumar el tamanio total del direcotrio
 * Y busca quien es el archivo con mayor tamanio
 */
  for (const auto &entry : results) 
  {
    int fileSize = fs::file_size(entry.path());
    totalSize += fileSize;
    if (fileSize > maxSize) 
    {
      maxSize = fileSize;
      largestFile = entry.path().filename().string();
    }
  }

  htmlFile << "<p>Tamaño total: " << totalSize << " bytes</p>" << endl;
  htmlFile << "<p>Archivo con mayor peso: " << largestFile << " (" << maxSize << " bytes)</p>" << endl;
  htmlFile << "<table border='1'>" << endl;
  htmlFile << "<tr><th>Archivo</th><th>Tamaño</th></tr>" << endl;

  for (const auto &entry : results) 
  {
    int fileSize = fs::file_size(entry.path());
    htmlFile << "<tr><td>" << entry.path().filename().string() << "</td><td>" << fileSize << " bytes</td></tr>" << endl;
  }

  htmlFile << "</table>" << endl;
  /*TERMINA el encabezado tipo html*/
  htmlFile << "</body></html>" << endl;
/*Se cierra el archivo*/
  htmlFile.close();
}