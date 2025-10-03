#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
using namespace std;

const int MAX_FILES = 100;
const int MAX_FILENAME = 50;

// Structure to represent a file in our simulation
struct File {
    char name[MAX_FILENAME];
    char content[500];
    int size;
    time_t createdTime;
    bool exists;
};

// File System class to manage operations
class FileSystem {
private:
    File* files[MAX_FILES];
    int fileCount;

public:
    FileSystem() {
        fileCount = 0;
        for (int i = 0; i < MAX_FILES; i++) {
            files[i] = nullptr;
        }
    }

    ~FileSystem() {
        for (int i = 0; i < fileCount; i++) {
            delete files[i];
        }
    }

    // Create a new file
    bool createFile(const char* filename, const char* content) {
        // Check if file already exists
        if (findFile(filename) != -1) {
            cout << "Error: File '" << filename << "' already exists!" << endl;
            return false;
        }

        // Check if we've reached maximum files
        if (fileCount >= MAX_FILES) {
            cout << "Error: Maximum file limit reached!" << endl;
            return false;
        }

        // Validate filename
        if (strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
            cout << "Error: Invalid filename!" << endl;
            return false;
        }

        // Create new file
        File* newFile = new File();
        strcpy(newFile->name, filename);
        strcpy(newFile->content, content);
        newFile->size = strlen(content);
        newFile->createdTime = time(nullptr);
        newFile->exists = true;

        files[fileCount++] = newFile;

        // Write to actual file
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error: Could not create physical file!" << endl;
            delete newFile;
            fileCount--;
            return false;
        }
        outFile << content;
        outFile.close();

        cout << "Success: File '" << filename << "' created successfully!" << endl;
        return true;
    }

    // Delete a file
    bool deleteFile(const char* filename) {
        int index = findFile(filename);
        if (index == -1) {
            cout << "Error: File '" << filename << "' not found!" << endl;
            return false;
        }

        // Delete physical file
        if (remove(filename) != 0) {
            cout << "Warning: Could not delete physical file, but removing from system." << endl;
        }

        // Mark as deleted and remove from array
        delete files[index];
        
        // Shift remaining files
        for (int i = index; i < fileCount - 1; i++) {
            files[i] = files[i + 1];
        }
        files[fileCount - 1] = nullptr;
        fileCount--;

        cout << "Success: File '" << filename << "' deleted successfully!" << endl;
        return true;
    }

    // Read a file
    bool readFile(const char* filename) {
        int index = findFile(filename);
        if (index == -1) {
            cout << "Error: File '" << filename << "' not found!" << endl;
            return false;
        }

        // Read from actual file
        ifstream inFile(filename);
        if (!inFile) {
            cout << "Error: Could not open file for reading!" << endl;
            return false;
        }

        cout << "\n====================================" << endl;
        cout << "         FILE CONTENT               " << endl;
        cout << "====================================" << endl;
        cout << "Filename: " << files[index]->name << endl;
        cout << "Size: " << files[index]->size << " bytes" << endl;
        cout << "Created: " << ctime(&files[index]->createdTime);
        cout << "------------------------------------" << endl;

        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        cout << "====================================" << endl;
        inFile.close();

        return true;
    }

    // Write/Append to a file
    bool writeFile(const char* filename, const char* content, bool append = false) {
        int index = findFile(filename);
        if (index == -1) {
            cout << "Error: File '" << filename << "' not found!" << endl;
            return false;
        }

        // Write to actual file
        ofstream outFile;
        if (append) {
            outFile.open(filename, ios::app);
        } else {
            outFile.open(filename);
        }

        if (!outFile) {
            cout << "Error: Could not open file for writing!" << endl;
            return false;
        }

        outFile << content;
        outFile.close();

        // Update file info
        if (append) {
            strcat(files[index]->content, content);
        } else {
            strcpy(files[index]->content, content);
        }
        files[index]->size = strlen(files[index]->content);

        cout << "Success: Content written to '" << filename << "'!" << endl;
        return true;
    }

    // List all files
    void listFiles() {
        if (fileCount == 0) {
            cout << "\nNo files in the system." << endl;
            return;
        }

        cout << "\n====================================" << endl;
        cout << "          FILE LIST                 " << endl;
        cout << "====================================" << endl;
        cout << "Total Files: " << fileCount << endl;
        cout << "------------------------------------" << endl;
        for (int i = 0; i < fileCount; i++) {
            cout << (i + 1) << ". " << files[i]->name;
            cout << " (" << files[i]->size << " bytes)" << endl;
        }
        cout << "====================================" << endl;
    }

    // Find file by name (returns index or -1 if not found)
    int findFile(const char* filename) {
        for (int i = 0; i < fileCount; i++) {
            if (strcmp(files[i]->name, filename) == 0) {
                return i;
            }
        }
        return -1;
    }

    int getFileCount() const {
        return fileCount;
    }
};

// Display menu
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "    FILE SYSTEM SIMULATION MENU        " << endl;
    cout << "========================================" << endl;
    cout << " 1. Create File                        " << endl;
    cout << " 2. Delete File                        " << endl;
    cout << " 3. Read File                          " << endl;
    cout << " 4. Write to File                      " << endl;
    cout << " 5. Append to File                     " << endl;
    cout << " 6. List All Files                     " << endl;
    cout << " 7. Exit                               " << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

int main() {
    FileSystem fs;
    int choice;
    char filename[MAX_FILENAME];
    char content[500];

    cout << "Welcome to File System Simulation!" << endl;

    do {
        displayMenu();
        
        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Invalid input! Please enter a number." << endl;
            continue;
        }
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1: { // Create File
                cout << "\nEnter filename: ";
                cin.getline(filename, MAX_FILENAME);
                cout << "Enter content: ";
                cin.getline(content, 500);
                fs.createFile(filename, content);
                break;
            }

            case 2: { // Delete File
                cout << "\nEnter filename to delete: ";
                cin.getline(filename, MAX_FILENAME);
                fs.deleteFile(filename);
                break;
            }

            case 3: { // Read File
                cout << "\nEnter filename to read: ";
                cin.getline(filename, MAX_FILENAME);
                fs.readFile(filename);
                break;
            }

            case 4: { // Write to File
                cout << "\nEnter filename: ";
                cin.getline(filename, MAX_FILENAME);
                cout << "Enter content (overwrites existing): ";
                cin.getline(content, 500);
                fs.writeFile(filename, content, false);
                break;
            }

            case 5: { // Append to File
                cout << "\nEnter filename: ";
                cin.getline(filename, MAX_FILENAME);
                cout << "Enter content to append: ";
                cin.getline(content, 500);
                fs.writeFile(filename, content, true);
                break;
            }

            case 6: { // List All Files
                fs.listFiles();
                break;
            }

            case 7: { // Exit
                cout << "\nExiting File System Simulation..." << endl;
                cout << "Total files managed: " << fs.getFileCount() << endl;
                cout << "Goodbye!" << endl;
                break;
            }

            default:
                cout << "Error: Invalid choice! Please enter 1-7." << endl;
        }

    } while (choice != 7);

    return 0;
}