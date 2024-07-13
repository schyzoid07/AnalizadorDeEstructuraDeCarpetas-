#include "Search.hpp"
#include "../GUI/Gui.hpp"
// Constructor de la clase Search, inicializa el directorio raíz
Search::Search(const fs::path &root) : root(root) {}

/**
 * @brief 
 * Este metodo crea un nuevo directorio por el usuario y permite crear mas dentro del ya creado por el usuario
 */
void Search::crearNuevoDirectorio()
{
    string option;

    cout << "Quisiera crear un nuevo directorio?\n Presione Y si quiere, de lo contrario, presione N \n:";
    cin>>option;
   
    if (option == "Y" || option == "y")
    {
        string directorio;

        cout << "Introduzca el nombre del directorio a crear\n:";
        cin>>directorio;
        std::filesystem::create_directory(directorio);
        cout << "Directorio creado con éxito.\n";
        cout << "Desea crear carpetas dentro del directorio? (Y/N)\n:";
        cin>>option;
       
        if (option == "Y" || option == "y")
        {
            string subdirectorio;
            cout << "Introduzca el nombre de la carpeta a crear dentro del directorio\n:";
            cin>>subdirectorio;
           

            // Crear la carpeta dentro del directorio
            std::filesystem::path path(directorio);
            path /= subdirectorio;
            std::filesystem::create_directory(path);

            cout << "Carpeta creada con éxito dentro del directorio.\n";
        }
        Gui gui;
        gui.mostrarMenuPrincipal();
    }
    else
    {
        system("cls");
        Gui gui;
        gui.mostrarMenuPrincipal();
    }
}
// Método público para presentar la estructura de archivos en forma de árbol
void Search::presentTree(bool onlyDirectories) const {
  // Llama a la función auxiliar presentTreeHelper con el directorio raíz y el
  // prefijo vacío
  presentTreeHelper(root, "", onlyDirectories);
}

// Método público para buscar archivos por nombre
void Search::searchByName(const std::string &name) const {
  // Vector para almacenar los resultados de la búsqueda
  std::vector<fs::directory_entry> results;
  // Llama a la función auxiliar searchHelper con un predicado lambda que
  // compara nombres de archivo
  searchHelper(
      root,
      [&name](const fs::directory_entry &entry) {
        return entry.path().filename().string() == name;
      },
      results);
  // Muestra los resultados de la búsqueda
  displayResults(results);
}

// Método público para buscar archivos por extensión
void Search::searchByExtension(const std::string &extension) const {
  // Vector para almacenar los resultados de la búsqueda
  std::vector<fs::directory_entry> results;
  // Llama a la función auxiliar searchHelper con un predicado lambda que
  // compara extensiones de archivo
  searchHelper(
      root,
      [&extension](const fs::directory_entry &entry) {
        return entry.path().extension().string() == extension;
      },
      results);
  // Muestra los resultados de la búsqueda
  displayResults(results);
}

// Función auxiliar privada para presentar la estructura de archivos en forma de
// árbol
void Search::presentTreeHelper(const fs::path &path, const std::string &prefix,
                               bool onlyDirectories) const {
  // Itera sobre cada entrada en el directorio dado
  for (const auto &entry : fs::directory_iterator(path)) {
    // Imprime el prefijo seguido de [D] o [F] dependiendo si es directorio o
    // archivo
    std::cout << prefix << (entry.is_directory() ? "[D] " : "[F] ")
              << entry.path().filename().string() << '\n';
    // Si es un directorio, llama recursivamente a presentTreeHelper con un
    // nuevo prefijo
    if (entry.is_directory()) {
      presentTreeHelper(entry.path(), prefix + "  ", onlyDirectories);
    } else if (!onlyDirectories) {
      // Si no es solo directorios, imprime el nombre del archivo con un prefijo
      // adicional
      std::cout << prefix << "  " << entry.path().filename().string() << '\n';
    }
  }
}

// Función auxiliar privada para buscar archivos según un predicado dado
void Search::searchHelper(
    const fs::path &path,
    const std::function<bool(const fs::directory_entry &)> &predicate,
    std::vector<fs::directory_entry> &results) const {
  // Itera recursivamente sobre cada entrada en el directorio dado
  for (const auto &entry : fs::recursive_directory_iterator(path)) {
    // Verifica si la entrada existe y es un archivo regular
    if (fs::exists(entry.path()) && fs::is_regular_file(entry.path())) {
      // Si la entrada cumple con el predicado, la añade a los resultados
      if (predicate(entry)) {
        results.push_back(entry);
      }
    }
  }
}

// Función auxiliar privada para mostrar los resultados de la búsqueda
void Search::displayResults(
    const std::vector<fs::directory_entry> &results) const {
  // Itera sobre cada resultado
  for (const auto &entry : results) {
    // Verifica si la entrada existe y es un archivo regular
    if (fs::exists(entry.path()) && fs::is_regular_file(entry.path())) {
      // Imprime el nombre del archivo, el directorio padre y el tamaño del
      // archivo
      std::cout << "Archivo: " << entry.path().filename().string()
                << " | Directorio: " << entry.path().parent_path().string()
                << " | Tamaño: " << fs::file_size(entry.path()) << " bytes"
                << '\n';
    }
  }
}

void Search::exportToHTML(const string &filename) {
  /*Se crea el objeto de archivo con extensio HTML*/
  ofstream htmlFile(filename + ".html");

  /*Se verifica si el archivo creado esta corrupto o no*/
  if (!htmlFile) {
    cerr << "Error: no se pudo crear el archivo HTML." << endl;
    return;
  }

  /*Empieza el encabezado tipo html*/
  htmlFile << "<html><head><title>Archivo de búsqueda</title></head><body>"
           << endl;
  htmlFile << "<h1>Archivo de búsqueda</h1>" << endl;

  vector<fs::directory_entry> results;
  searchHelper(
      root, [](const fs::directory_entry &entry) { return true; }, results);

  htmlFile << "<p>Cantidad de archivos: " << results.size() << "</p>" << endl;

  int totalSize = 0;
  int maxSize = 0;
  string largestFile;

  /* Este bucle ayuda a sumar el tamanio total del direcotrio
   * Y busca quien es el archivo con mayor tamanio
   */
  for (const auto &entry : results) {
    int fileSize = fs::file_size(entry.path());
    totalSize += fileSize;
    if (fileSize > maxSize) {
      maxSize = fileSize;
      largestFile = entry.path().filename().string();
    }
  }

  htmlFile << "<p>Tamaño total: " << totalSize << " bytes</p>" << endl;
  htmlFile << "<p>Archivo con mayor peso: " << largestFile << " (" << maxSize
           << " bytes)</p>" << endl;
  htmlFile << "<table border='1'>" << endl;
  htmlFile << "<tr><th>Archivo</th><th>Tamaño</th></tr>" << endl;

  for (const auto &entry : results) {
    int fileSize = fs::file_size(entry.path());
    htmlFile << "<tr><td>" << entry.path().filename().string() << "</td><td>"
             << fileSize << " bytes</td></tr>" << endl;
  }

  htmlFile << "</table>" << endl;
  /*TERMINA el encabezado tipo html*/
  htmlFile << "</body></html>" << endl;
  /*Se cierra el archivo*/
  htmlFile.close();
}
