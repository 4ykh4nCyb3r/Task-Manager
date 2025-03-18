#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <exception>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
#include <limits>

class InvalidDeadlineException : public std::exception
{
    public:
    virtual const char* what() const throw(){
        return "Invalid format. Please use mm/dd/yyyy format.";
    }

};

/*******************************************************************************/
bool checkFileContainTitle(const std::string& title, const std::string& FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << FileName << std::endl;
        return false;
    }

    char line[150];
    while (file.getline(line, 150)) {
        char* token = strtok(line, ";");
        std::vector<std::string> tokens;
        while (token != NULL) {
            tokens.push_back(token);
            token = strtok(NULL, ";");
        }
        if (!tokens.empty() && tokens.at(0) == title) {
            file.close(); // Close the file before returning
            return true;
        }
    }
    file.close(); // Close the file if the title is not found
    return false;
}

/*******************************************************************************/
class Task
{
    protected:
    std::string TaskTitle;
    std::string TaskCategory;
    std::string TaskDeadline;
    std::string TaskNote;
    int TaskPriority;
    friend void addNew(Task* p);
    template<typename T>
    friend void transferToArray(Task*& array, int& arraySize);

    public:
    //initialization list
     Task(const std::string& title, const std::string& category, const std::string& deadline, const std::string& note, int priority)
        : TaskTitle(title), TaskCategory(category), TaskDeadline(deadline), TaskNote(note), TaskPriority(priority) {}

    Task(){}; //default constructor 

    
    virtual std::string className() const{
        return "Task";
    }
    // add to database
    virtual void add(const std::string& FileName)
     {  
        //open a file
        std::ofstream outputfile(FileName, std::ios::app); // open file in append mode
        if(!outputfile.is_open())
        {
            std::cout <<"Error opening file!" << std::endl;
            return;
        }
        //add a new task to the file
        outputfile << this->TaskTitle << ";" << this->TaskCategory << ";" << this->TaskDeadline << ";" << this->TaskNote << ";" << this->TaskPriority << std::endl;
        outputfile.close();
        return;
        
    }
    
    // remove from database
   virtual bool remove(const std::string& fileName, const std::string& inTitle) {
    // Open the file for reading
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cout << "Error in opening file!" << std::endl;
        return false;
    }

    if(checkFileContainTitle(inTitle, fileName) == false)
    {
        std::cout << "You don't have a task with this title" << std::endl;
        return false;
    }


    // modified file to write file content
    std::ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        std::cout << "Error creating a temporary file!" << std::endl;
        return false;
    }

    
    std::string line;
    while (std::getline(inputFile, line)) {
        //this line is for splitting a line into tokens
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(iss, token, ';')) {
            tokens.push_back(token);
        }

        // Check if the first token matches the title to remove
        if (!tokens.empty() && tokens[0] != inTitle) {
            // Write line to temporary file if the first token doesn't match
            tempFile << line << "\n";
        }
    }

    // Close files
    inputFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (std::remove(fileName.c_str()) != 0) {
        std::cout << "Error: Unable to delete original file." << std::endl;
        return false;
    }
    if (std::rename("temp.txt", fileName.c_str()) != 0) {
        std::cout << "Error: Unable to rename temporary file." << std::endl;
        return false;
    }
    return true;

}
    
    //display current object
    virtual void display() const
    {
        std::cout << "Title: " << TaskTitle<< std::endl;
        std::cout << "Category: " << TaskCategory << std::endl;
        std::cout << "Deadline:" << TaskDeadline<< std::endl;
        std::cout << "Note: " << TaskNote << std::endl;
        std::cout << "Priority: " << TaskPriority<< std::endl;
        std::cout << "--------------------------------------------------------------------" << std::endl;
    }
/////////////////////////////////////////////////////////////////////////
    //update method
    virtual void update()
{
    int choice;
    do {
       std::cout << "What you want to update?" << std::endl;
       std::cout << "1. Task Title\n2. Task Category\n3. Task Deadline\n4. Task Note\n5. Task Priority\n6. Don't make an update." << std::endl;
            
            // Check for valid input
        while (!(std::cin >> choice)) {
        std::cin.clear(); // clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Invalid input. Please enter a number between 1 and 6: ";
        }
            
        if (choice < 1 || choice > 6) {
            std::cout << "Error: Invalid selection. Please enter a number between 1 and 6." << std::endl;
        }
        
    } while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice!=6);

    if(choice == 1)
    {
        std::string currentCategory = TaskCategory;
        currentCategory.erase(std::remove(currentCategory.begin(), currentCategory.end(), ' '), currentCategory.end());
        std::string FileName = currentCategory + ".txt";
        bool removed = remove(FileName, TaskTitle); //remove task with current title
        std::cout << "Enter new title:";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, this->TaskTitle);
        add(FileName);
        std::cout << "Update has been made!" << std::endl;
        return;
    }
    else if( choice == 2)
    {
        std::vector<std::string> categories = {"My Day", "Assigned To Me", "Important", "Planned"};
        // Remove the task from current category
        std::string currentCategory = this->TaskCategory;
        currentCategory.erase(std::remove(currentCategory.begin(), currentCategory.end(), ' '), currentCategory.end());
        std::string currentFileName = currentCategory + ".txt";
        remove(currentFileName, this->TaskTitle);
        std::string newCategory;
        bool isValidCategory = false;
        std::cin.ignore();
        do {
            std::cout << "Enter new Task Category (word by word same!): ";
            std::getline(std::cin, newCategory);
            
            if (!newCategory.empty()) {
                if (std::find(categories.begin(), categories.end(), newCategory) != categories.end()) {
                    isValidCategory = true;
                } else {
                    std::cout << "Invalid category! Try again!" << std::endl;
                }
                } else {
                std::cout << "Please enter a category name." << std::endl;
                }

        
        } while (!isValidCategory);
        
        //add to new database/category
        this->TaskCategory = newCategory;
        newCategory.erase(std::remove(newCategory.begin(), newCategory.end(), ' '), newCategory.end());
        std::string newCategoryFileName = newCategory + ".txt"; 
        
        //adding to database
        add(newCategoryFileName);
        std::cout << "Update has been made!" << std::endl;
        return;
    }
    else if(choice == 3)
    {
        std::string currentCategory = TaskCategory;
        currentCategory.erase(std::remove(currentCategory.begin(), currentCategory.end(), ' '), currentCategory.end());
        std::string FileName = currentCategory + ".txt";
        remove(FileName, this->TaskTitle);

        std::string deadline;
        bool validDeadline = false;
        std::regex dateRegex("^(0[1-9]|1[0-2])/(0[1-9]|[1-2][0-9]|3[0-1])/\\d{4}$");
        std::cin.ignore();
        do
        {
            try
            {
                std::cout << "Enter new deadline of the task (mm/dd/yy): ";
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::getline(std::cin, deadline);
                if(!std::regex_match(deadline, dateRegex)){
                    throw InvalidDeadlineException();
                }
                validDeadline = true;
            }catch(const InvalidDeadlineException& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        } while (!validDeadline);
        this->TaskDeadline = deadline;
        add(FileName); //change in database
        std::cout << "Update has been made!" << std::endl;
        return;
    }
    else if(choice == 4)
    {
        std::string currentCategory = TaskCategory;
        currentCategory.erase(std::remove(currentCategory.begin(), currentCategory.end(), ' '), currentCategory.end());
        std::string FileName = currentCategory + ".txt";
        remove(FileName, this->TaskTitle);
        std::cout << "Enter a new note to the task:";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, this->TaskNote);
        add(FileName);
        std::cout << "Update has been made!" << std::endl;
        return;
    }
    else if (choice == 5)
    {
        std::string currentCategory = TaskCategory;
        currentCategory.erase(std::remove(currentCategory.begin(), currentCategory.end(), ' '), currentCategory.end());
        std::string FileName = currentCategory + ".txt";
        remove(FileName, this->TaskTitle);
        std::cout << "Enter new priority (1-high, 2-medium, 3-low):";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int pr;
        while (!(std::cin >> pr) || pr < 1 || pr > 3) {
            std::cout << "Invalid input. Please enter a number between 1 and 3: ";
            // Clear the input buffer to avoid reading the leftover newline character
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        this->TaskPriority = pr;
        add(FileName);
        std::cout << "Update has been made!" << std::endl;
        return;
    }
    else{
        std::cout << "No update made!" << std::endl;
        return;
    }
   
    
}
    
    //Get attributes of task
    std::string title() const {return this->TaskTitle;}
    std::string category() const { return this->TaskCategory;}
    std::string deadline() const { return this->TaskDeadline;}
    std::string note() const { return this->TaskNote;}
    int priority() const { return this->TaskPriority;}
};

/********************************************************************************/
void addNew(Task* p) {
    std::cout << "Enter specs of the task:" << std::endl;
    std::cout << "Title: ";
    std::cin.ignore();
    std::getline(std::cin, p->TaskTitle);
    
    std::string catg = p->className();
    p->TaskCategory = catg;
    
    std::string deadline;
    bool validDeadline = false;
    std::regex dateRegex("^(0[1-9]|1[0-2])/(0[1-9]|[1-2][0-9]|3[0-1])/\\d{4}$");

    do{
    std::cout << "Deadline (mm/dd/yyyy): ";
    std::getline(std::cin, deadline);
    if(std::regex_match(deadline, dateRegex))
    {
        validDeadline = true;
    } else {
        std::cout << "Invalid input! Try again!" << std::endl;
    }
    }while(!validDeadline);
    p->TaskDeadline = deadline;
    

    std::cout << "Note: ";
    std::getline(std::cin, p->TaskNote);


    std::cout << "Priority (1 - high, 2 - medium, 3 - low): ";

    int priority;
    // Use a loop to keep reading input until a valid integer is entered
    while (!(std::cin >> priority) || priority < 1 || priority > 3) {
        std::cout << "Invalid input. Please enter a number between 1 and 3: ";
        // Clear the input buffer to avoid reading the leftover newline character
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // Assign the valid priority to the task
    p->TaskPriority = priority;

    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return;
}

/**********************************************************************************/
void reallocateSize(Task*& array, int& arraySize) {
  try {
    if (array == nullptr) {
      // ... (allocation and size increment)
      array = new Task[1];
      arraySize = 1;
      return;
    } else {
      Task* newArray = new Task[arraySize + 1];
      if (newArray == nullptr) {
        throw std::bad_alloc();
      }
      for(int i = 0; i < arraySize; i++){
        newArray[i] = array[i];
      }
      delete[] array;
      array = newArray;
      arraySize++;
      return;
    }
  } catch (const std::exception& e) {
    std::cout << "Error: Unable to allocate memory!" << std::endl;
  }
}
/*************************************************************************************/

void removeFromArray(Task*& array, int& arraySize, const std::string& tit, const std::string& catg) 
{
    try {
        int indexToRemove = -1;
        for (int i = 0; i < arraySize; i++) {
            if (array[i].title() == tit && array[i].category() == catg) {
                indexToRemove = i;
                break;
            }
        }

        if (indexToRemove == -1) {
            return;
        }

        Task* newArray = new Task[arraySize - 1];

        int j = 0;
        for (int i = 0; i < arraySize; i++) {
            if (i != indexToRemove) {
                newArray[j] = array[i];
                j++;
            }
        }

        delete[] array;
        array = newArray;
        arraySize--;

    } catch (const std::bad_alloc& e) {
        std::cout << "Error: Unable to allocate memory!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

/*************************************************************************************/
class MyDay : public Task{
    public:
    std::string className() const override{
        return "My Day";
    }
    void add(const std::string& filename) override
    {
        std::ofstream outputfile(filename, std::ios::app); // open file in append mode
        if(!outputfile.is_open())
        {
            std::cout <<"Error opening file!" << std::endl;
            return;
        }
        //add a new task to the file
        outputfile << this->TaskTitle << ";" << this->TaskCategory << ";" << this->TaskDeadline << ";" << this->TaskNote << ";" << this->TaskPriority << std::endl;
        outputfile.close();
        return;

    }
   
};
/****************************************************************************************/
class AssignedToMe : public Task{
    public:
    std::string className() const override{
        return "Assigned To Me";
    }

    
    void add(const std::string& filename) override
    {
        std::ofstream outputfile(filename, std::ios::app);
        if(!outputfile.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }
        outputfile << this->TaskTitle << ";" << this->TaskCategory << ";" << this->TaskDeadline << ";" << this->TaskNote << ";" << this->TaskPriority << std::endl;
        outputfile.close();
    }
    
};
/****************************************************************************************/
class Important : public Task{
    public:
    std::string className() const override{
        return "Important";
    }
    void add(const std::string& filename) override
    {
        std::ofstream outputfile(filename, std::ios::app); // open file in append mode
        if(!outputfile.is_open())
        {
            std::cout <<"Error opening file!" << std::endl;
            return;
        }
        //add a new task to the file
        outputfile << this->TaskTitle << ";" << this->TaskCategory << ";" << this->TaskDeadline << ";" << this->TaskNote << ";" << this->TaskPriority << std::endl;
        outputfile.close();
        return;
    }
    
};
/**************************************************************************************/
class Planned : public Task{
    public:
    std::string className() const{
        return "Planned";
    }
   
    void add(const std::string& filename) override
    {
        std::ofstream outputfile(filename, std::ios::app); // open file in append mode
        if(!outputfile.is_open())
        {
            std::cout <<"Error opening file!" << std::endl;
            return;
        }
        //add a new task to the file
        outputfile << this->TaskTitle << ";" << this->TaskCategory << ";" << this->TaskDeadline << ";" << this->TaskNote << ";" << this->TaskPriority << std::endl;
        outputfile.close();
        return;

    }
 
};
/**************************************************************************************/
bool displayDerivedObjects(Task*& array, int& arraySize, const std::string& Category)
{
    bool found = false;
    for(int i = 0; i < arraySize; i++)
    {
        if(Category == array[i].category())
        {
            array[i].display();
            found = true;
        }
    }
    return found;
}
/**************************************************************************************/
void displayArray(Task* p, int arraySize)
{
    std:: cout << "------------------Displaying Database-------------------" << std::endl;
    for(int i = 0; i < arraySize; i++)
    {
        p[i].display();
    }
}
/**************************************************************************************/
template<typename T>
void flowControl(Task*& myArray, int& arraySize) {
    do{
        int n;
        do {
            std::cout << "Menu:\n1. Add\n2. Remove\n3. Display Tasks\n4. Update task\n5. Go back" << std::endl;
            std::cout << "Select Operation: ";
            
            // Check for valid input
            while (!(std::cin >> n)) {
                std::cin.clear(); // clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
                std::cout << "Invalid input. Please enter a number between 1 and 5: ";
            }
            
            if (n < 1 || n > 5) {
                std::cout << "Error: Invalid selection. Please enter a number between 1 and 5." << std::endl;
            }
        
    } while(n != 1 && n != 2 && n != 3 && n != 4 && n != 5);

    if (n == 1) {
        T* obj = new T();
        addNew(obj);
        std::string fileName = obj->category();
        fileName.erase(std::remove(fileName.begin(), fileName.end(), ' '), fileName.end());
        fileName += ".txt";
        reallocateSize(myArray, arraySize);
        myArray[arraySize - 1] = *obj; // add a new task to the array
        obj->add(fileName);
        std::cout << "New task has been added!" << std::endl;
        delete obj;
    } else if (n == 2){
        /*
        if(arraySize == 0){
            std::cout << "No tasks to remove!" << std::endl;
            return;
        }
        */
        std::string catg = T().className();
        bool f = displayDerivedObjects(myArray, arraySize, catg);
        if(!f)
        {
            std::cout << "No tasks to remove!" << std::endl;
            return;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string titleToRemove;
        std::cout << "Enter a title you want to remove:";
        std::getline(std::cin, titleToRemove);
        std::string fileName;
        fileName = catg; //the category of the current task is assigned to fileName
        fileName.erase(std::remove(fileName.begin(), fileName.end(), ' '), fileName.end());
        fileName+=".txt";  
        if(myArray[0].remove(fileName, titleToRemove) == true)
        {
        for(int i = 0; i < arraySize; i++)
        {
            if(myArray[i].title() == titleToRemove && myArray[i].category() == catg)
            {
                std::string temp = myArray[i].title();
                removeFromArray(myArray, arraySize, temp, catg);
            }
        }
        std::cout << "Task successfully removed!" << std::endl;
        }
        
        return;
    }else if(n == 3){
        std::string catg = T().className();
        bool f = displayDerivedObjects(myArray, arraySize, catg);
        if(!f)
        {
            std::cout << "No task in this category" << std::endl;
            return;
        }

    }else if(n == 4){
        std::string catg = T().className();
        std::cout << "Current Tasks:" << std::endl;
        bool f = displayDerivedObjects(myArray, arraySize, catg);
        if(!f)
        {
            std::cout << "No task to update" << std::endl;
            return;
        }
        std::string Title;
        std::cout << "Categories:\n" 
                  << "My Day\n"
                  << "Assigned To Me\n"
                  << "Important\n"
                  << "Planned"<< std::endl;
        
        std::cout << "Enter the title of the task you want to update:";
        std::cin.ignore();
        std::getline(std::cin, Title);
        bool boolean = false;
        for(int i = 0; i < arraySize; i++)
        {
            if(myArray[i].title() == Title)
            {
                boolean = true;
                myArray[i].update();
            }
        }
        if(!boolean)
        {
            std::cout << "No task with this title!" << std::endl;

        }
        
    }else{
        break;
        return;
    }
    }while(1);
    
}


template<typename T>
void transferToArray(Task*& array, int& arraySize)
{   
    
    std::string FileName = T().className();
    FileName.erase(std::remove(FileName.begin(), FileName.end(), ' '), FileName.end());
    FileName += ".txt";
    std::ifstream file(FileName);
    if (!file.is_open()) {
        return;
    }
   
    char line[300];
    while (file.getline(line, 300)) {
        reallocateSize(array, arraySize);
        char* token = strtok(line, ";");
        std::vector<std::string> tokens;
        while (token != NULL) {
            tokens.push_back(token);
            token = strtok(NULL, ";");
        }
        if (tokens.size() != 5) { // Ensure that there are exactly 5 tokens
            arraySize--; // Decrement the size since reallocateSize increased it
            continue; // Skip this line
        }
        T obj;
        obj.TaskTitle = tokens.at(0);
        obj.TaskCategory = tokens.at(1);
        obj.TaskDeadline = tokens.at(2);
        obj.TaskNote = tokens.at(3);
        obj.TaskPriority = std::stoi(tokens.at(4));
        array[arraySize-1] = obj;
    
    }
    file.close();
    return;
}
 

int main()
{
    int num;
    Task* myArray;
    int arraySize = 0;
    myArray = nullptr;
    transferToArray<MyDay>(myArray, arraySize);
    transferToArray<AssignedToMe>(myArray, arraySize);
    transferToArray<Important>(myArray, arraySize);
    transferToArray<Planned>(myArray, arraySize);
    do{
        do {
            std::cout << "Task Categories:\n1. My day\n2. Assigned To Me\n3. Important\n4. Planned\n5. Display Database\n6. Exit" << std::endl;
            std::cout << "Select a category: ";
            
            // Check for valid input
            while (!(std::cin >> num)) {
                std::cin.clear(); // clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
                std::cout << "Invalid input. Please enter a number between 1 and 6: ";
            }
        
            if (num < 1 || num > 6) {
                std::cout << "Error: Invalid category. Please enter a number between 1 and 6." << std::endl;
            }
        } while(num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6);
    switch (num)
    {
    case 1:
        flowControl<MyDay>(myArray, arraySize);
        break;
    case 2:
        flowControl<AssignedToMe>(myArray, arraySize);
        break;
    case 3:
        flowControl<Important>(myArray, arraySize);
        break;
    case 4:
        flowControl<Planned>(myArray, arraySize);
        break;
    case 5:
        displayArray(myArray, arraySize);
        break;
    case 6:
        break;
    default:
        std::cout << "Invalid input! Try again.." << std::endl;
        continue;
    }
    }while(num!=6);
    std::cout << "Exiting ...." <<std:: endl;
    delete[] myArray;
    return 0;
}




