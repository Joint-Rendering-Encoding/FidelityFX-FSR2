#include "IOHelpers.h"

bool folderExists(const std::string &folderPath)
{
    DWORD fileAttributes = GetFileAttributesA(folderPath.c_str());
    return (fileAttributes != INVALID_FILE_ATTRIBUTES) && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

void recreateFolder(const std::string &folderPath)
{
    // remove the folder if it exists
    if (folderExists(folderPath))
    {
        // remove directory and its contents recursively
        WIN32_FIND_DATAA findData;
        HANDLE findHandle;
        std::string fileOrFolderPath = folderPath + "\\*";

        // find the first file or subfolder in the folder
        findHandle = FindFirstFileA(fileOrFolderPath.c_str(), &findData);

        if (findHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                // skip "." and ".." entries
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                    fileOrFolderPath = folderPath + "\\" + findData.cFileName;

                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        // recursively remove subfolder
                        recreateFolder(fileOrFolderPath);
                    }
                    else
                    {
                        // delete file
                        DeleteFileA(fileOrFolderPath.c_str());
                    }
                }
            } while (FindNextFileA(findHandle, &findData));

            FindClose(findHandle);
        }

        // remove the empty folder
        RemoveDirectoryA(folderPath.c_str());
    }

    // create the folder
    CreateDirectoryA(folderPath.c_str(), NULL);
}