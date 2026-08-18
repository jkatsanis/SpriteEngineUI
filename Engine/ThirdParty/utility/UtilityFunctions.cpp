#include "UtilityFunctions.h"

std::string spe::Utility::BoolToStr(bool b)
{
    return b ? "True" : "False";
}

void spe::Utility::CreateFileWithContent(const std::string& content, const std::string& pathAndName)
{
    std::string path = pathAndName;
#ifdef __linux__
    path = spe::Utility::ToRightPath(pathAndName);
#endif

    std::ofstream file;

    file.open(path, std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open file to write to");
    }

    file << content;

    file.close();
}

bool spe::Utility::Contains(const std::string& str, const std::vector<std::string>& arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (str == arr[i])
        {
            return true;
        }
    }
    return false;
}

std::string spe::Utility::VectorToString(const std::vector<std::string>& vec, char del, const std::string& ext)
{
    std::string rs = "";

    for (size_t i = 0; i < vec.size(); i++)
    {
        rs += vec[i];
        if (i + 1 < vec.size())
        {
            rs += del;
        }
    }

    return rs + ext;
}

std::string spe::Utility::RenamePartOnPath(const std::string& path, const std::string& new_name, char del, const std::string& ext, uint32_t pos)
{
    std::vector<std::string> parts = spe::Utility::Split(path, del);
    parts[parts.size() - pos] = new_name;
    return VectorToString(parts, del, ext);
}

void spe::Utility::SetCurrentDir(const std::string& path)
{
    std::cout << "Setting dir..";
#ifdef _WIN32
    if (!SetCurrentDirectoryA(path.c_str()))
    {
        throw std::runtime_error("Couldn't set directory: " + path);
    }
#else
    if (chdir(path.c_str()) != 0)
    {
        throw std::runtime_error("Couldn't set directory: " + path);
    }
#endif
    std::cout << " Done." << std::endl;
}


std::string spe::Utility::getUserProjectPathSeperatetFromEnginePath(const std::string& path,const std::string& nameOFUsr)
{
    std::string userProject = nameOFUsr;
    int idx = (int)path.find(userProject);
    std::string finalStr = "";
    for (int i = idx + (int)userProject.size() + 1; i < path.size(); i++)
    {
        finalStr += path[i];
    }
    return finalStr;
}


std::vector<std::string> spe::Utility::Split(const std::string& s,char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void spe::Utility::WriteFile(const std::string& content, const std::string& path)
{
    std::ofstream file;

    file.open(path, std::ios::out | std::ios::binary);

    file << content;

    file.close();
    
}

std::string spe::Utility::GetFileExtension(const std::string& file)
{
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '.')
        {
            bool isValid = true;
            std::string extension = "";
            for (int j = i + 1; j < file.size(); j++)
            {
                extension += file[j];
                if (file[j] == '.')
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid)
            {
                return extension;
            }
        }
    }
    return "folder";
}

std::string spe::Utility::GetNamePathSplit(std::string path)
{
    std::filesystem::path p(path);
    const auto fname = p.filename();
    return fname.empty() ? std::string() : fname.string();
}

void spe::Utility::Delete(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("There is no file!");
        return;
    }
    std::filesystem::remove_all(path);
}

std::string spe::Utility::RunCommand(const char* command)
{
    char buffer[128];
    std::string result;

#ifdef _WIN32
    FILE* pipe = _popen(command, "r");
#else
    FILE* pipe = popen(command, "r"); // Linux/macOS
#endif

    if (!pipe) throw std::runtime_error("popen() failed!");

    try {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    }
    catch (...) {
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        throw;
    }

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    return result;
}

std::string spe::Utility::GetDefaultDir(uint32_t depth)
{
    std::cout << "Calling GetDefaultDir().." << std::endl;

    // Get current directory using std::filesystem (cross-platform)
    std::filesystem::path currentPath = std::filesystem::current_path();

    // Split the path into parts
    std::vector<std::string> parts;
    for (const auto& part : currentPath) {
        parts.push_back(part.string());
    }

    // Build the path up to (size - depth)
    std::filesystem::path result;
    for (size_t i = 0; i < parts.size() - depth; ++i) {
        result /= parts[i]; // automatically handles '/' vs '\' cross-platform
    }

    return result.string();
}

std::string spe::Utility::GetCurrentDir()
{
    return std::filesystem::current_path().string();
}

bool spe::Utility::IsStringValid(const std::string& path)
{
    int cnt = 0;
    for (const char c : path)
    {
        if (c == '.' || c == '/' || c == '$')
        {
            cnt++;
        }
    }
    return !(cnt == path.size());
}

bool spe::Utility::IsFolder(const std::string& path)
{
    for (const char c : path)
    {
        if (c == '.')
        {
            return false;
        }
    }
    return true;
}

std::string spe::Utility::RemoveExtension(const std::string& file)
{
    std::string newFileName = "";
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '.')
        {
            break;
        }
        newFileName.push_back(file[i]);
    }
    return newFileName;
}

std::string spe::Utility::CopyDir(const std::string& inputDir, const std::string& outputDir, const std::string& name)
{
    fs::path src(inputDir);
    fs::path dst = fs::path(outputDir) / name;

    try {
        // Create the destination directory if it doesn't exist
        if (!fs::exists(dst)) {
            fs::create_directories(dst);
        }

        // Recursively copy contents
        for (auto& entry : fs::recursive_directory_iterator(src)) {
            const auto& path = entry.path();
            auto relativePath = fs::relative(path, src);
            fs::path targetPath = dst / relativePath;

            if (fs::is_directory(path)) {
                fs::create_directories(targetPath);
            } else if (fs::is_regular_file(path)) {
                fs::copy_file(path, targetPath, fs::copy_options::overwrite_existing);
            }
        }
    } catch (fs::filesystem_error& e) {
        std::cerr << "Error copying directory: " << e.what() << std::endl;
    }

    return dst.string();
}

void spe::Utility::GetFilePathWithExtensionInFolder(const std::filesystem::path& path, const std::string& extension, std::vector<std::string>& to)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry))
        {
            GetFilePathWithExtensionInFolder(entry.path(), extension, to);
        }
        else
        {
            std::string file_extension = "." + GetFileExtension(entry.path().filename().string());
            if (file_extension == extension)
            {
                to.push_back(entry.path().string());
            }
        }
    }
}

#ifdef __linux__
std::string spe::Utility::ToLinuxPath(const std::string &path) {
    std::string cleanPath = path;

    // Replace backslashes with forward slashes
    std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

    // Remove duplicate slashes
    std::string::size_type pos = 0;
    while ((pos = cleanPath.find("//", pos)) != std::string::npos) {
        cleanPath.replace(pos, 2, "/");
    }

    // Ensure it starts with "Assets" (with capital A)
    if (cleanPath.rfind("Assets", 0) == 0) {
        cleanPath[0] = 'A';
    }

    if (!cleanPath.empty() && cleanPath.back() == '\r') {
        cleanPath.pop_back();
    }


    return cleanPath;
}
#endif
std::string spe::Utility::ToRightPath(const std::string &path)
{
    std::string updatetPath = path;
#ifdef __linux__
    updatetPath = spe::Utility::ToLinuxPath(path);
#else
    updatetPath = spe::Utility::ToWindowsPath(path);
#endif

    return updatetPath;
}

