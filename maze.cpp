#include <iostream>
#include <cstdlib>

using namespace std;

char AREA = '~';
char BCRUMB = '+';
char BLOCK = 'X';
char START = '0';
char END = '*';

int row;
int col;

class List
{
    public:
    
        // Initializes the list
    
        List();
        
        // Adds two elements to the list.
        // Returns true if everything went well,
        // or false otherwise.
        
        bool AddToList(int firstCoord, int secondCoord);
        
        // Gets two elements from the list and puts them
        // in the first two slots of the array.
        // Returns true if everything went well,
        // or false otherwise.
        
        bool GetFromList(int *storage);
        
    private:
    
        //bool ResizeList(); // Used to dynamically resize the list (doubles in size)
    
        // Note: size_of_list is by default row * col (half the elements of the arena).
    
        int *list; // The list used
        int size_of_list; // The current amount of space in the list. NEEDS TO BE EVEN!
        
        // Note: Both of the following ptr's start from the beginning
        
        int *getPtr; // This ptr is used to get elements from list
        int *setPtr; // This ptr is used to set new elements on the list.
                     // It ALWAYS points to a garbage value.
        
        int used_space; // This keeps track of how many elements setPtr has traversed.
    
};

char** CreateArena();
int** CreateNumericArena(char** arena, int start[2], int end[2]);
void Solve(int **numericArena, int currentPosition[2], List &myList);
void ConnectTheDots(char **arena, int **numericArena, int positionOfEnd[2]);

int main()
{
    
    int currentPosition[2]; // start
    int endingPosition[2];  // end
    
    char **arena = CreateArena();
    
    int **numericArena = CreateNumericArena(arena, currentPosition, endingPosition);
    
/*    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << arena[i][j] << " ";
            
        }
        
        cout << endl;
        
    }
    
    cout << endl << endl;*/
    
/*    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << numericArena[i][j];
            
        }
        
        cout << endl;
        
    }*/
    
    cout << endl << endl;
    
    List myList; // Initialize our list
    
    Solve(numericArena, currentPosition, myList);
    
    ConnectTheDots(arena, numericArena, endingPosition);
    
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(arena[i][j] == BCRUMB)
            {
                cout << "\033[31m" << arena[i][j]; // Red
            }

            else if(arena[i][j] == BLOCK)
            {
                cout << "\033[32m" << arena[i][j]; // Green
            }

            else
            {
                cout << "\033[36m" << arena[i][j]; // Cyan
            }
        }
        
        cout << endl;
        
    }
    
    cout << endl << endl << "\033[0m"; // Reset the color codes
    
/*    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << numericArena[i][j] << " ";
            
        }
        
        cout << endl;
        
    }*/

    return 0;
}

List::List()
{
    used_space = 0;
    
    size_of_list = 2 * row * col + 4;
    
    list = new int [size_of_list];
    
    getPtr = &list[0];
    
    setPtr = &list[0];
    
}

bool List::AddToList(int firstCoord, int secondCoord)
{
    // Check if there's enough space 
    
    if(used_space == size_of_list)
    {
        //ResizeList();
        
        cout << "Error! List is too small to add another element!";
        
        return false;
        
    }
    
    else if(firstCoord < 0 || firstCoord >= row)
    {
        cout << "Weird ERROR! 1";
    }
    
    else if(secondCoord < 0 || secondCoord >= col)
    {
        cout << "Weird ERROR! 2";
    }
    
    used_space = used_space + 2;
    
    setPtr[0] = firstCoord;
    setPtr[1] = secondCoord;
    
    setPtr = &setPtr[2];
    
    return true;
}

bool List::GetFromList(int *storage)
{
    if(getPtr == setPtr)
    {
        cout << "No elements left!";
        return false;
    }
    
    storage[0] = getPtr[0];
    storage[1] = getPtr[1];
    
    getPtr = &getPtr[2];
    
    return true;
}

char** CreateArena()
{
    // First, we ask the user what the dimensions are of the arena
    
    cout << "How many rows? ";
    cin >> row;
    
    cout << "\nHow many columns? ";
    cin >> col;
    
    // Now we make the arena
    
    char** arena = new char* [row];
    
    for(int i = 0; i < row; i++)
    {
        arena[i] = new char [col];
        
        cout << "Enter " << col << " chars: ";
        
        for(int j = 0; j < col; j++)
        {
            cin >> arena[i][j];
            
        }
        
    }
    
    return arena;
    
}

int** CreateNumericArena(char** arena, int start[], int end[])
{
    // Note: We assume that row and col are global variables (also ints)
    //       and that they have been set by CreateArena() already.
    
    // Allocate space for our numericArena
    
    int **numericArena = new int* [row];
    
    for(int i = 0; i < row; i++)
    {
        numericArena[i] = new int [col];
        
    }
    
    // Assign values to numericArena
    // Note: We must check to see that START and END exist,
    //       or we'll get segfaults.

    bool start_exists = false;
    bool end_exists = false;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(arena[i][j] == AREA)
            {
                numericArena[i][j] = -1;
            }
            
            else if(arena[i][j] == BLOCK)
            {
                numericArena[i][j] = -2;
                
            }
            
            else if(arena[i][j] == START)
            {
                numericArena[i][j] = 0;
                
                start[0] = i;
                start[1] = j;

                if(start_exists == false)
                {
                    start_exists = true;
                }

                else
                {
                    cerr << "\nError! Too many start characters!\n";
                    exit(1);
                }
            }
            
            else if(arena[i][j] == END)
            {
                numericArena[i][j] = -3;
                
                end[0] = i;
                end[1] = j;

                if(end_exists == false)
                {
                    end_exists = true;
                }

                else
                {
                    cerr << "\nError! Too many end characters!\n";
                    exit(1);
                }
            }
            
            else
            {
                cerr << "Error! Unknown character detected!";
                return nullptr;

            }
            
        }
        
    }

    if(start_exists == false)
    {
        cerr << "\nStart character DNE!\n";
        exit(1);
    }

    else if(end_exists == false)
    {
        cerr << "\nEnd character DNE!\n";
        exit(1);
    }

    return numericArena;
    
}

void Solve(int **numericArena, int currentPosition[], List &myList)
{
    int localCounter = 0;
    
    // Check Right
    
    if(currentPosition[1] + 1 < col && numericArena[currentPosition[0]][currentPosition[1] + 1] == -1)
    {
        // Add the new position to the list
        
        if(myList.AddToList(currentPosition[0], currentPosition[1] + 1))
        {
//            cout << "Successfully added " << currentPosition[0] << ", "
//                 << currentPosition[1] + 1 << " to the list...\n";
        }
        
        else
        {
            cout << "Failed to add " << currentPosition[0] << ", "
                 << currentPosition[1] + 1 << " to the list! Aborting!!\n";
                 
            return;
        }
        
        // Update the value of the new position to our value + -1
        
        numericArena[currentPosition[0]][currentPosition[1] + 1] = numericArena[currentPosition[0]][currentPosition[1]] + 1;
        
        localCounter++;
        
    }
    
    // Check Left
    
    if(currentPosition[1] - 1 >= 0 && numericArena[currentPosition[0]][currentPosition[1] - 1] == -1)
    {
        // Add the new position to the list
        
        if(myList.AddToList(currentPosition[0], currentPosition[1] - 1))
        {
//            cout << "Successfully added " << currentPosition[0] << ", "
//                 << currentPosition[1] - 1 << " to the list...\n";
        }
        
        else
        {
            cout << "Failed to add " << currentPosition[0] << ", "
                 << currentPosition[1] - 1 << " to the list! Aborting!!\n";
                 
            return;
        }
        
        // Update the value of the new position to our value + -1
        
        numericArena[currentPosition[0]][currentPosition[1] - 1] = numericArena[currentPosition[0]][currentPosition[1]] + 1;
        
        localCounter++;
        
    }
    
    // Check Up
    
    if(currentPosition[0] - 1 >= 0 && numericArena[currentPosition[0] - 1][currentPosition[1]] == -1)
    {
        // Add the new position to the list
        
        if(myList.AddToList(currentPosition[0] - 1, currentPosition[1]))
        {
//            cout << "Successfully added " << currentPosition[0] - 1 << ", "
//                 << currentPosition[1] << " to the list...\n";
        }
        
        else
        {
            cout << "Failed to add " << currentPosition[0] - 1 << ", "
                 << currentPosition[1] << " to the list! Aborting!!\n";
                 
            return;
        }
        
        // Update the value of the new position to our value + -1
        
        numericArena[currentPosition[0] - 1][currentPosition[1]] = numericArena[currentPosition[0]][currentPosition[1]] + 1;
        
        localCounter++;
        
    }
    
    // Check Down
    
    if(currentPosition[0] + 1 < row && numericArena[currentPosition[0] + 1][currentPosition[1]] == -1)
    {
        // Add the new position to the list
        
        if(myList.AddToList(currentPosition[0] + 1, currentPosition[1]))
        {
//            cout << "Successfully added " << currentPosition[0] + 1 << ", "
//                 << currentPosition[1] << " to the list...\n";
        }
        
        else
        {
            cout << "Failed to add " << currentPosition[0] + 1 << ", "
                 << currentPosition[1] << " to the list! Aborting!!\n";
                 
            return;
        }
        
        // Update the value of the new position to our value + -1
        
        numericArena[currentPosition[0] + 1][currentPosition[1]] = numericArena[currentPosition[0]][currentPosition[1]] + 1;
        
        localCounter++;
        
    }
    
    // Recursively call ourself
    
    for(int i = 0; i < localCounter; i++)
    {
        // Pull the next element from the list
        
        myList.GetFromList(currentPosition);
        
//        cout << "Getting " << currentPosition[0] << ", " << currentPosition[1] << " from the list...\n";
        
        // Call ourself
        
        Solve(numericArena, currentPosition, myList);
        
    }
    
}

void ConnectTheDots(char **arena, int **numericArena, int positionOfEnd[])
{
    // Recall, positionOfEnd contains ONLY TWO ELEMENTS!
    
    // Note: We can never be in a spot where our value is the smallest positive
    //       value unless we're at zero. This means we won't EVER have to compare
    //       our value with the value of an adjacent square.
    
    // 'U' for uNknown, because we don't know where to go yet.
    // This value can be 'R', 'L', 'U' or 'D' for Right, Left, Up or Down.
    char shortest_path = 'N';
    int smallest_number = -1;
    
    // If one of the adjacent squares is 0, we place a BCRUMB at our spot
    // and the 0 spot, then we're finished.
    
    // Otherwise, we must take note of every adjacent square and remember
    // the smallest number. This and the algorithm above are the same.
    
    // Check Right
    
    if(positionOfEnd[1] + 1 < col && numericArena[positionOfEnd[0]][positionOfEnd[1] + 1] >= 0)
    {
        if(numericArena[positionOfEnd[0]][positionOfEnd[1] + 1] == 0)
        {
            arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
            return;
        }
        
        else if(smallest_number < 0 || numericArena[positionOfEnd[0]][positionOfEnd[1] + 1] < smallest_number)
        {
            smallest_number = numericArena[positionOfEnd[0]][positionOfEnd[1] + 1];
            shortest_path = 'R';
        }

    }
    
    // Check Left
    
    if(positionOfEnd[1] - 1 >= 0 && numericArena[positionOfEnd[0]][positionOfEnd[1] - 1] >= 0)
    {
        if(numericArena[positionOfEnd[0]][positionOfEnd[1] - 1] == 0)
        {
            arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
            return;
        }
        
        else if(smallest_number < 0 || numericArena[positionOfEnd[0]][positionOfEnd[1] - 1] < smallest_number)
        {
            smallest_number = numericArena[positionOfEnd[0]][positionOfEnd[1] - 1];
            shortest_path = 'L';
        }

    }
    
    // Check Up
    
    if(positionOfEnd[0] - 1 >= 0 && numericArena[positionOfEnd[0] - 1][positionOfEnd[1]] >= 0)
    {
        if(numericArena[positionOfEnd[0] - 1][positionOfEnd[1]] == 0)
        {
            arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
            return;
        }
        
        else if(smallest_number < 0 || numericArena[positionOfEnd[0] - 1][positionOfEnd[1]] < smallest_number)
        {
            smallest_number = numericArena[positionOfEnd[0] - 1][positionOfEnd[1]];
            shortest_path = 'U';
        }

    }
    
    // Check Down
    
    if(positionOfEnd[0] + 1 < row && numericArena[positionOfEnd[0] + 1][positionOfEnd[1]] >= 0)
    {
        if(numericArena[positionOfEnd[0] + 1][positionOfEnd[1]] == 0)
        {
            arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
            return;
        }
        
        else if(smallest_number < 0 || numericArena[positionOfEnd[0] + 1][positionOfEnd[1]] < smallest_number)
        {
            smallest_number = numericArena[positionOfEnd[0] + 1][positionOfEnd[1]];
            shortest_path = 'D';
        }

    }
    
    // Recursively call ourself depending on the fastest path
    
    if(shortest_path == 'R')
    {
        // Leave a BCRUMB in our orginal spot
        
        arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
        
        // Move our position to the right
        
        positionOfEnd[1]++;
        
        ConnectTheDots(arena, numericArena, positionOfEnd);
    }
    
    else if(shortest_path == 'L')
    {
        // Leave a BCRUMB in our orginal spot
        
        arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
        
        // Move our position to the left
        
        positionOfEnd[1]--;
        
        ConnectTheDots(arena, numericArena, positionOfEnd);
        
    }
    
    else if(shortest_path == 'U')
    {
        // Leave a BCRUMB in our orginal spot
        
        arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
        
        // Move our position to the left
        
        positionOfEnd[0]--;
        
        ConnectTheDots(arena, numericArena, positionOfEnd);
        
    }
    
    else if(shortest_path == 'D')
    {
        // Leave a BCRUMB in our orginal spot
        
        arena[positionOfEnd[0]][positionOfEnd[1]] = BCRUMB;
        
        // Move our position to the left
        
        positionOfEnd[0]++;
        
        ConnectTheDots(arena, numericArena, positionOfEnd);
        
    }
    
}
