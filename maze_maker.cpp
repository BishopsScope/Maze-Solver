#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

using namespace std;

char AREA = '~';
char BLOCK = 'X';

int main()
{
    int row;
    int col;
    int percentageOfArea;

    do {

    cout << "What percentage of area to blocks do you want? (0-100): ";
    cin >> percentageOfArea;

    } while (percentageOfArea < 0 || percentageOfArea > 100);

    int rand_num;
    
    char **arena;
    
    /* initialize random seed: */
    srand (time(NULL));
    
    cout << "How many rows? ";
    cin >> row;
    
    cout << "How many cols? ";
    cin >> col;
    
    arena = new char* [row];
    
    for(int i = 0; i < row; i++)
    {
        arena[i] = new char [col];
        
        for(int j = 0; j < col; j++)
        {
            rand_num = rand() % 100; // 0 - 99 (100 numbers total)
            
            if(rand_num < percentageOfArea)
            {
                arena[i][j] = AREA;
                
            }
            
            else
            {
                arena[i][j] = BLOCK;
            }
            
        }
        
    }

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(arena[i][j] == AREA)
            {
                cout << "\033[36m" << arena[i][j]; // Cyan
            }

            else
            {
                cout << "\033[32m" << arena[i][j]; // Green
            }
        }
        
        cout << endl;
    }

    return 0;
}

