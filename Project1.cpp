/*
Course: ECCS 2671 -- Data Structures and Algorithms 1
Group Members: Alex Randall and Chase McCluskey

Use this code template to submit Project 1.

Implement all of the member functions that have not been implemented based on the functionality in the handout

You are NOT allowed to modify the TrainCompositionManager class, only complete the class implementation.
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TrainCompositionManager {

private:
    struct CarNode {
        int carNumber;
        string carName;
        string carType;
        string manufacturerName;
        double weightTons;
        CarNode* next;
        CarNode* prev;

        // Default constructor for CarNode
        CarNode() { carNumber=1; carName=""; carType=""; manufacturerName=""; weightTons=0.0; next=prev=nullptr; }
        // Parameterized constructor for CarNode
        CarNode(string cName, int cNum, string cType, double wTons, string mName = "", CarNode* nCar = nullptr, CarNode* pCar = nullptr)
        {
            carNumber = cNum;
            carName = cName;
            carType = cType;
            weightTons = wTons;
            manufacturerName = mName;
            next = nCar;
            prev = pCar;
        }
    };

    int numCars;
    CarNode* head;
    CarNode* tail;

    // A private helper function for Student 1
    CarNode* getCarNode(const string carName);

    // A private helper function for Student 2
    CarNode* getCarNode(const int carNumber);

public:
    // Default constructor
    TrainCompositionManager() { numCars = 0; head = tail = nullptr; }
    
    // Destructor to be implemented by Student 2
    ~TrainCompositionManager();

    // Student 1 Member Functions
    void addCar(const string addedCarName, const string carType, const double weightTons);
    void addCar(const string addedCarName, const int carNumber, const string carType, const double weightTons);
    void deleteCar(const string deletedCarName);
    int getCarNum(const string searchedCarName);
    vector<string> getAllCarNamesByType(const string carType);

    // Student 2 Member Functions
    void addCar(const string addedCarName, const int carNumber, const string carType, const double weightTons, const string manufacturer);
    void deleteLastCar();
    void deleteCar(const int carNumInList);
    string getCarName(const int carNumInList);
    int getNumCars() const { return numCars; }
    int getCarTypeCount(const string carType);
    
    // Overloaded << operator to print the train composition
    friend ostream& operator<<(ostream& outputStream, const TrainCompositionManager& trainList);
};

// ********************************************************************************************
// Implement the following functions here.
// Each student should only complete their assigned functions.
// ********************************************************************************************

// Student 1 Functions:
// Function descriptions are in the project handout.

/**
 * Traverses the list to find the first car with a given name.
 *
 * @param carName The name of the car to find.
 * @return A pointer to the CarNode if found, otherwise nullptr.
 */
TrainCompositionManager::CarNode* TrainCompositionManager::getCarNode(const string carName) {
    // Start from the head of the list
    CarNode *temp = head;

    // Traverse the list until the end
    while(temp != nullptr){
        // If the car name matches, return the pointer
        if(temp->carName == carName){
            return temp;
        }
        // Move to the next node
        temp = temp->next;
    }
    // If not found, return nullptr
    return nullptr;
}

/**
 * Adds a new car to the head of the train.
 *
 * @param addedCarName The name of the new car.
 * @param carType The type of the new car.
 * @param weightTons The weight of the new car.
 */
void TrainCompositionManager::addCar(const string addedCarName, const string carType, const double weightTons) {
    // Only allow cars with valid weight
    if(!(10 <= weightTons && weightTons <= 143)){
        return;
    }

    // Create a new car node for the head (carNumber is always 1)
    CarNode *newCar = new CarNode(addedCarName, 1, carType, weightTons, "");

    // If the list is empty, set head and tail to the new car
    if(head == nullptr){
        head = newCar;
        tail = newCar;
    }
    else{
        // Insert at the head
        newCar->next = head;
        head->prev = newCar;
        head = newCar;

        // Increment car numbers for all subsequent cars
        CarNode *temp = head->next;
        while(temp != nullptr){
            temp->carNumber++;
            temp = temp->next;
        }
    }

    // Increase the car count
    numCars++;
}

/**
 * Adds a new car at a specified position in the train.
 *
 * @param addedCarName The name of the new car.
 * @param carNumber The desired position for the new car.
 * @param carType The type of the new car.
 * @param weightTons The weight of the new car.
 */
void TrainCompositionManager::addCar(const string addedCarName, const int carNumber, const string carType, const double weightTons) {
    // Only allow cars with valid weight
    if(!(10 <= weightTons && weightTons <= 143)){
        return;
    }

    // If the list is empty or carNumber is less than 2, add at the head
    if(head == nullptr || carNumber < 2){
        addCar(addedCarName, carType, weightTons);
        return;
    }

    // If carNumber is greater than the number of cars, add at the tail
    if(carNumber > numCars){
        CarNode *newCar = new CarNode(addedCarName, (numCars + 1), carType, weightTons, "");
        tail->next = newCar;
        newCar->prev = tail;
        tail = newCar;
        numCars++;
        return;
    }

    // Traverse to the node at the desired position
    CarNode *temp = head;
    while(temp != nullptr && temp->carNumber < carNumber){
        temp = temp->next;
    }

    // Insert the new car before the found node
    CarNode *newCar = new CarNode(addedCarName, carNumber, carType, weightTons, "");
    newCar->next = temp;
    newCar->prev = temp->prev;
    temp->prev->next = newCar;
    temp->prev = newCar;

    // Increment car numbers for all subsequent cars
    while(temp != nullptr){
        temp->carNumber++;
        temp = temp->next;
    }

    // Increase the car count
    numCars++;
}

/**
 * Deletes a car from the train based on its name.
 *
 * @param deletedCarName The name of the car to delete.
 */
void TrainCompositionManager::deleteCar(const string deletedCarName) {
    // Find the car node to delete
    CarNode *carToDelete = getCarNode(deletedCarName);

    // If not found, do nothing
    if(carToDelete == nullptr){
        return;
    }

    // Decrement car numbers for all subsequent cars
    CarNode *temp = carToDelete->next;
    while(temp != nullptr){
        temp->carNumber--;
        temp = temp->next;
    }

    // Handle deletion for head, tail, or middle node
    if(carToDelete == head && carToDelete == tail){
        // Only one node in the list
        head = nullptr;
        tail = nullptr;
    }
    else if(carToDelete == head){
        // Deleting the head node
        head = head->next;
        head->prev = nullptr;
    }
    else if(carToDelete == tail){
        // Deleting the tail node
        tail = tail->prev;
        tail->next = nullptr;
    }
    else{
        // Deleting a middle node
        carToDelete->prev->next = carToDelete->next;
        carToDelete->next->prev = carToDelete->prev;
    }

    // Free the memory
    delete carToDelete;
    // Decrease the car count
    numCars--;
}

/**
 * Gets the car number for a car with a specific name.
 *
 * @param searchedCarName The name of the car to search for.
 * @return The car number if found, otherwise -1.
 */
int TrainCompositionManager::getCarNum(const string searchedCarName) {
    // Find the car node by name
    CarNode *carToFind = getCarNode(searchedCarName);

    // If found, return its car number
    if(carToFind != nullptr){
        return carToFind->carNumber;
    }

    // If not found, return -1
    return -1;
}

/**
 * Gets a list of all car names that match a given type.
 *
 * @param carType The type of cars to search for.
 * @return A vector of strings containing the names of matching cars.
 */
vector<string> TrainCompositionManager::getAllCarNamesByType(const string carType){
    // Vector to store matching car names
    vector<string> carNames;

    // Traverse the list and collect names of matching type
    CarNode *temp = head;
    while(temp != nullptr){
        if(temp->carType == carType){
            carNames.push_back(temp->carName);
        }
        temp = temp->next;
    }

    // Return the list of names
    return carNames;
}

// Student 2 Functions:
// Function descriptions are in the project handout.
// Student 2 Functions - Corrected Implementation
// Function descriptions are in the project handout.

/**
 * @brief Retrieves a pointer to the CarNode at the specified car number position.
 * 
 * This is a private helper function that traverses the doubly linked list to find
 * the node at the given position. Handles boundary conditions by returning head
 * for positions less than 2, and tail for positions greater than or equal to numCars.
 * 
 * @param carNumber The position of the car to retrieve (1-based indexing)
 * @return CarNode* Pointer to the node at the specified position, or head/tail for boundary cases
 * 
 * @note Returns head if carNumber < 2
 * @note Returns tail if carNumber >= numCars
 * @note For valid positions, returns the first node with carNumber >= the specified value
 */
TrainCompositionManager::CarNode* TrainCompositionManager::getCarNode(const int carNumber) {
    // Return head if carNumber < 2
    if (carNumber < 2) {
        return head;
    }
    
    // Return tail if carNumber >= numCars
    if (carNumber >= numCars) {
        return tail;
    }
    
    // Traverse to find the node with the specified carNumber
    CarNode* current = head;
    while (current != nullptr) {
        if (current->carNumber >= carNumber) {
            return current;
        }
        current = current->next;
    }
    
    // Fallback return (should not reach here in normal cases)
    return tail;
}

/**
 * @brief Adds a new car to the train at the specified position with manufacturer information.
 * 
 * Creates and inserts a new CarNode into the doubly linked list at the position specified
 * by carNumber. All cars at the insertion position and beyond have their car numbers
 * incremented. Handles weight validation and all special cases including empty list,
 * head insertion, tail insertion, and middle insertion.
 * 
 * @param addedCarName Unique identifier name for the new car (e.g., "Silver Streak Car")
 * @param carNumber Position where the car should be inserted (1-based indexing)
 * @param carType Classification of the car (e.g., "Locomotive", "Passenger", "Cargo")
 * @param weightTons Weight of the car in tons (must be between 10-143 tons)
 * @param manufacturerName Name of the company that built the car
 * 
 * @pre weightTons must be between 10 and 143 tons (inclusive)
 * @post numCars is incremented by 1
 * @post All car numbers at insertion position and beyond are incremented
 * @post All pointers (next/prev) are properly connected
 * 
 * @note If carNumber < 2, car is placed at head
 * @note If carNumber > numCars, car is placed at tail
 * @note Function returns early if weight constraints are not met
 */
void TrainCompositionManager::addCar(const string addedCarName, const int carNumber, const string carType, const double weightTons, const string manufacturer) {
    // Check weight constraints
    if (weightTons < 10 || weightTons > 143) {
        cout << addedCarName << " has invalid weight " << weightTons << " - please enter a number between 10 and 143" << endl;
        return;
    }
    
    // Create new car node
    CarNode* newCar = new CarNode();
    newCar->carName = addedCarName;
    newCar->carType = carType;
    newCar->weightTons = weightTons;
    newCar->manufacturerName = manufacturer;
    newCar->next = nullptr;
    newCar->prev = nullptr;
    
    // Handle empty list
    if (head == nullptr) {
        newCar->carNumber = 1;
        head = tail = newCar;
        numCars++;
        return;
    }
    
    // Add at head (carNumber < 2)
    if (carNumber < 2) {
        newCar->carNumber = 1;
        newCar->next = head;
        head->prev = newCar;
        head = newCar;
        
        // Update car numbers for all existing cars
        CarNode* current = newCar->next;
        while (current != nullptr) {
            current->carNumber++;
            current = current->next;
        }
        numCars++;
        return;
    }
    
    // Add at tail (carNumber > numCars)
    if (carNumber > numCars) {
        newCar->carNumber = numCars + 1;
        newCar->prev = tail;
        tail->next = newCar;
        tail = newCar;
        numCars++;
        return;
    }
    
    // Add in middle - find the position
    CarNode* current = head;
    int position = 1;
    
    while (current != nullptr && position < carNumber) {
        current = current->next;
        position++;
    }
    
    // Insert before current
    newCar->carNumber = carNumber;
    newCar->next = current;
    newCar->prev = current->prev;
    
    if (current->prev != nullptr) {
        current->prev->next = newCar;
    }
    current->prev = newCar;
    
    // Update car numbers for all cars at this position and beyond
    current = newCar->next;
    while (current != nullptr) {
        current->carNumber++;
        current = current->next;
    }
    
    numCars++;
}

/**
 * @brief Deletes the last car (tail) from the train.
 * 
 * Removes the car at the tail of the doubly linked list and properly updates
 * the tail pointer. Handles special cases including empty list and single node list.
 * Memory is properly deallocated and numCars is decremented.
 * 
 * @pre Train may be empty or contain one or more cars
 * @post If train was not empty, numCars is decremented by 1
 * @post tail pointer is updated to the new last car (or nullptr if empty)
 * @post Memory for deleted node is properly deallocated
 * 
 * @note Does nothing if the train is already empty
 * @note Handles single node case by setting both head and tail to nullptr
 */
void TrainCompositionManager::deleteLastCar() {
    // Handle empty list
    if (tail == nullptr) {
        cout << "Empty Train\n";
        return;
    }
    
    CarNode* nodeToDelete = tail;
    
    // Handle single node case
    if (head == tail) {
        head = tail = nullptr;
    } else {
        // Update tail pointer
        tail = tail->prev;
        tail->next = nullptr;
    }
    
    delete nodeToDelete;
    numCars--;
}

/**
 * @brief Deletes a car from the train at the specified position.
 * 
 * Removes the CarNode at the given position in the doubly linked list. All cars
 * after the deleted position have their car numbers decremented. Handles boundary
 * conditions and special cases including empty list, single node, head deletion,
 * tail deletion, and middle deletion.
 * 
 * @param carNumInList Position of the car to delete (1-based indexing)
 * 
 * @pre Train may be empty or contain one or more cars
 * @post If deletion occurs, numCars is decremented by 1
 * @post All car numbers after deleted position are decremented
 * @post All pointers (next/prev) are properly reconnected
 * @post Memory for deleted node is properly deallocated
 * 
 * @note Deletes head if carNumInList < 2
 * @note Deletes tail if carNumInList >= numCars
 * @note Does nothing if train is empty
 */
void TrainCompositionManager::deleteCar(const int carNumInList) {
    // Handle empty list
    if (head == nullptr) {
        cout << "Empty Train\n";
        return;
    }
    
    CarNode* nodeToDelete = nullptr;
    
    // Delete head if carNumInList < 2
    if (carNumInList < 2) {
        nodeToDelete = head;
        
        // Handle single node case
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
            
            // Update car numbers for remaining cars
            CarNode* current = head;
            while (current != nullptr) {
                current->carNumber--;
                current = current->next;
            }
        }
    }
    // Delete tail if carNumInList >= numCars
    else if (carNumInList >= numCars) {
        nodeToDelete = tail;
        
        // Handle single node case (already covered above, but for clarity)
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
    }
    // Delete from middle
    else {
        // Find the node to delete
        CarNode* current = head;
        int position = 1;
        
        while (current != nullptr && position < carNumInList) {
            current = current->next;
            position++;
        }
        
        if (current != nullptr) {
            nodeToDelete = current;
            
            // Connect the nodes around the deleted node
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            }
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }
            
            // Update car numbers for cars after the deleted position
            CarNode* updateCurrent = current->next;
            while (updateCurrent != nullptr) {
                updateCurrent->carNumber--;
                updateCurrent = updateCurrent->next;
            }
        }
    }
    
    if (nodeToDelete != nullptr) {
        delete nodeToDelete;
        numCars--;
    }
}

/**
 * @brief Retrieves the name of the car at the specified position.
 * 
 * Searches through the doubly linked list to find the car at the given position
 * and returns its carName. Handles boundary conditions and special cases including
 * empty train, position out of bounds, and normal position lookup.
 * 
 * @param carNumInList Position of the car whose name should be retrieved (1-based indexing)
 * @return string The carName of the car at the specified position, or "Empty Train" if list is empty
 * 
 * @note Returns "Empty Train" if the train is empty
 * @note Returns head car's name if carNumInList < 2
 * @note Returns tail car's name if carNumInList >= numCars
 * @note For valid positions, returns the name of the car at that exact position
 */
string TrainCompositionManager::getCarName(const int carNumInList) {
    // Handle empty list
    if (head == nullptr) {
        return "Empty Train";
    }
    
    // Return head name if carNumInList < 2
    if (carNumInList < 2) {
        return head->carName;
    }
    
    // Return tail name if carNumInList >= numCars
    if (carNumInList >= numCars) {
        return tail->carName;
    }
    
    // Find the car at the specified position
    CarNode* current = head;
    while (current != nullptr) {
        if (current->carNumber == carNumInList) {
            return current->carName;
        }
        current = current->next;
    }
    
    // Fallback (should not reach here in normal cases)
    return tail->carName;
}

/**
 * @brief Counts the total number of cars that match the specified car type.
 * 
 * Traverses the entire doubly linked list and counts all cars whose carType
 * matches the provided string. This is useful for inventory management and
 * reporting purposes to determine how many cars of each category are in the train.
 * 
 * @param carType The type of car to count (e.g., "Locomotive", "Passenger", "Cargo", "Dining", "Caboose")
 * @return int The total count of cars matching the specified type, or 0 if none found or train is empty
 * 
 * @note Returns 0 if the train is empty
 * @note Case-sensitive string comparison is used for carType matching
 * @note Traverses entire list to ensure accurate count
 * 
 * @example 
 * // Count all passenger cars in the train
 * int passengerCount = manager.getCarTypeCount("Passenger");
 */
int TrainCompositionManager::getCarTypeCount(const string carType) {
    // Handle empty list
    if (head == nullptr) {
        return 0;
    }
    
    int count = 0;
    CarNode* current = head;
    
    // Traverse entire list and count matches
    while (current != nullptr) {
        if (current->carType == carType) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}
// ********************************************************************************************
// Fully implemented functions (no modification needed)
// ********************************************************************************************
TrainCompositionManager::~TrainCompositionManager() {
    CarNode* deleteNode = head;
    while (deleteNode != nullptr) {
        CarNode* nextNode = deleteNode->next;
        delete deleteNode;
        deleteNode = nextNode;
    }
    head = tail = nullptr;
    numCars = 0;
}

ostream& operator<<(ostream& outputStream, const TrainCompositionManager& trainList) {
    TrainCompositionManager::CarNode* current = trainList.head;
    while (current != nullptr) {
        outputStream << "(" << current->carNumber << ") " << current->carName << ", "<< current->carType <<", ";
        current = current->next;
    }
    outputStream << "End of Train" << endl;
    return outputStream;
}

int main() {
    // You can add your own test cases here to test the functions as you implement them.
    // Comment out or uncomment functions as needed for testing.
    TrainCompositionManager myTrain;
    
    cout << "We start with: " << myTrain.getCarName(1) << endl; // Student 2 member function

    // Test Student 2 Functions
    myTrain.addCar("Red Caboose", 4, "Caboose", 25.0, "Alstom");
    myTrain.addCar("Heavy Cargo Car", 3, "Cargo", 100.0, "Siemens Mobility");

    // Test Student 1 Functions
    myTrain.addCar("The Iron Horse", "Locomotive", 130.0);
    myTrain.addCar("Silver Streak Car", "Passenger", 55.5);
    myTrain.addCar("Grand View Diner", 2, "Dining", 75.2);
    
   // Using Student 1's function to add a car at the head
    myTrain.addCar("Coal Car 1", "Cargo", 80.0); 

    // Using Student 2's function to add a car with a manufacturer
    myTrain.addCar("Observation Car", 3, "Passenger", 60.5, "Kawasaki Heavy Industries");

    // Using Student 1's function to add a car to the middle
    myTrain.addCar("Baggage Car", 5, "Baggage", 45.0);

    // Using Student 2's function to add a car with a manufacturer at the tail
    myTrain.addCar("Reefer Car 1", 100, "Cargo", 110.0, "CRRC Corporation");

    // Using Student 1's function with an invalid weight
    myTrain.addCar("Invalid Weight Car", "Test", 5.0); // Should trigger an error message

    // Using Student 2's function with an invalid weight and a manufacturer
    myTrain.addCar("Another Invalid Car", 2, "Test", 200.0, "Alstom"); // Should trigger an error message

    // Print the train composition
    cout << "The train has " << myTrain.getNumCars() << " cars:" << endl;
    cout << myTrain << endl;
    
    // Demonstrate other functions
    cout << "The car number for 'Grand View Diner' is " << myTrain.getCarNum("Grand View Diner") << endl; // Student 1 member function
    cout << "The car name at position 3 is " << myTrain.getCarName(3) << endl; // Student 2 member function

    cout << "There are " << myTrain.getCarTypeCount("Cargo") << " cargo cars in the train." << endl; // Student 2 member function
    cout << "There are " << myTrain.getCarTypeCount("Passenger") << " passenger cars in the train." << endl; // Student 1 member function

    // Test deletion functions
    cout << "\nDeleting a few cars..." << endl; 
    myTrain.deleteCar("Grand View Diner"); // Student 1 member function
    myTrain.deleteLastCar(); // Student 2 member function
    myTrain.deleteCar(1); // Student 2 member function

    // Print the final train composition
    cout << "The final train has " << myTrain.getNumCars() << " cars:" << endl;
    cout << myTrain << endl;
    return 0;
}