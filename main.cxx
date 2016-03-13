#include <iostream>
#include <ctime>
#include <cstdlib>

const bool debug = 1;
const int numbers1to9[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

using namespace std;

typedef struct sudoku_t {
    int array[9][9];
} sudoku_t;

void generator (sudoku_t & field);
void generateSquare3x3 (sudoku_t & field, int sq_coord_x, int sq_coord_y);
bool generationFailureCheck (sudoku_t field, int coord_x, int coord_y);
void print(sudoku_t field);
bool uniqueCheck(sudoku_t field, int input_x, int input_y, int number);

void generator (sudoku_t & field)
{
    srand(time(NULL));

    for (int coord_x = 0; coord_x < 9; coord_x++)
        for (int coord_y = 0; coord_y < 9; coord_y++)
            field.array[coord_x][coord_y] = 0; // inits field with zeros

    //outer coords for 3*3 squares, inner coords for numbers inside, 3*3 too

    for (int outer_x = 0; outer_x < 3; outer_x++)
        for (int outer_y = 0; outer_y < 3; outer_y++)
            generateSquare3x3(field, outer_x*3, outer_y*3);
}

void generateSquare3x3 (sudoku_t & field, int sq_coord_x, int sq_coord_y)
{
    for (int inner_x = 0; inner_x < 3; inner_x++) //inner square coordinates
        for (int inner_y = 0; inner_y < 3;)
        {
            int coord_x = sq_coord_x + inner_x;
            int coord_y = sq_coord_y + inner_y;

            if (generationFailureCheck(field, coord_x, coord_y))
            {
                cout << "generation failed\n";
                exit(-1);
            }

            int temp_rand = rand()%9 + 1;
            if (uniqueCheck(field, coord_x, coord_y, temp_rand))
            {
                field.array[coord_x][coord_y] = temp_rand;
                inner_y++;
            }

            if (debug) { system("clear"); print(field); }
            //prints full field after every generation step
        }
}

void print(sudoku_t field)
{
    int counterLine = 0;
    int counterCol = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << field.array[i][j] << " ";
            if (++counterCol%3 == 0) cout << "\t";
        }
        cout << endl;
        if (++counterLine%3 == 0) cout << endl;
    }
}

bool uniqueCheck(sudoku_t field, int input_x, int input_y, int number)
//checks, if there are any contradictions with sudoku logic
{
    for (int coord_y = 0; coord_y < 9; coord_y++) //column checker
        if (field.array[input_x][coord_y] == number) return false;

    for (int coord_x = 0; coord_x < 9; coord_x++) //string checker
        if (field.array[coord_x][input_y] == number) return false;

    { //square checker (little tricky move to check 9 field squares 3*3)
        int sq_coord_x = input_x - input_x%3; //parse 0 or 3 or 6 from input coord
        int sq_coord_y = input_y - input_y%3; //parse 0 or 3 or 6 from input coord
        for (int inner_x = 0; inner_x < 3; inner_x++) //three times right from sq_coord_x
            for (int inner_y = 0; inner_y < 3; inner_y++) //three times down from sq_coord_y
                if (number != 0)
                    if (field.array[sq_coord_x + inner_x][sq_coord_y + inner_y] == number)
                        return false;
    }
    return true;
}

bool generationFailureCheck (sudoku_t field, int coord_x, int coord_y)
//checks, if there is a generation failure
{
    bool failure = false;
    for (int i = 0; i < 9; i++)
    {
        failure = failure | uniqueCheck(field, coord_x, coord_y, numbers1to9[i]);
    }
    return !failure;
}

int main()
{
    sudoku_t field;
    generator(field);
    print(field);

    return 0;
}
