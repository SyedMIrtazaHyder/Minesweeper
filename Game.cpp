#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

void bombs_location(int, vector<int>&);
void ans_grid(vector<int>&);
void grid_generator(vector<int>&, int*); //generating a minesweeper game
void adjacent_opener(int, vector<int>&);
void player_grid(vector<int>&, vector<int>&);

int rows = 5, columns = 5, totalbombs = 5, winorlose = 0, auth = 0, total_pos = 10;
const int std_rows = 6, std_cols = 6, std_bombs = 10, exit_code = -12345;

string filename = "scoresheet.txt", adminpass = "123";

struct players
{
    int position = 1;
    int time_taken = 0;
    string name = "";
};

//assumptions:1) no one can score below 10 seconds and take more than 999999 seconds to solve the grid to have a highscore
//2) No one enters a name with only spaces e.g " " or "    "
void top_players(vector<players>&, string);
void sorter(vector<players>&);
void file_writer(vector<players>&);
string rewrite(fstream&, int);
void display_scores(string, fstream&);
void File_part(int);
void game_menu(void);
void game_main(void);
void admin();
void admin_powers();
void settings(int& input);
void new_player_info(vector<players>& arr, int time, string filename, string old_data);

void animation()
{
    for (int counter = 3; counter > 0; counter--) {
        cout << setw(45) << "___     ___     ___\n" << setw(45) << "\\ *\\___|   |___/* /\n" << setw(47)
            << "___>   (* ___ *)   <___\n" << setw(48) << "|___  (*  |___|  *)  ___|\n" << setw(44)
            << ">__(*__   __*)__<\n" << setw(45) << "/*_/   |___|   \\_*\\\n"
            << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(300);
        system("CLS");

        cout << setw(49) << "_____      _____      _____\n" << setw(49)
            << "\\   *\\____|     |____/*   /\n" << setw(51) << "___>     (*  _____  *)     <___\n"
            << setw(47) << "|\t       |     |\t          |\n" << setw(52) << "|___     (*  |_____|  *)     ___|\n"
            << setw(49) << ">_____(*___   ___*)_____<\n" << setw(49) << "/*____/   |_____|   \\____*\\"
            << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(300);
        system("CLS");
    }

    cout << setw(39) << "_ /\\ _\n" << setw(43) << "\t \\___  /      \\ ___ /\n" <<
        setw(48) << "_\\/                  \\/_\n" << setw(36) << "_\\\t\t    / _\n"
        << setw(38) << "\t_ |    \t\t    | _\n" << setw(48) << "/      BOOOOM!!!      \\\n"
        << setw(42) << "< __    \t\t    __ >\n" << setw(43) << "/ \\ \\___ \t___/ / \\ \n"
        << setw(48) << "|     / |_____| \\     |\n" << setw(38) << "v     \tv" << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    Sleep(300);
    system("CLS");

    cout << setw(42) << "~~~~~~~~~~~~\n" << setw(47) << " ~  ~  ~  ~  ~  ~  ~\n" << setw(45)
        << "  ~  ~ ~  ~  ~  ~  ~\n" << setw(49) << "~~~~~~~~~~~~~~~~~~~~~~~\n" << setw(47) << " ~  ~  ~  ~  ~  ~  ~\n"
        << setw(45) << "  ~  ~ ~  ~  ~  ~  ~\n" << setw(43) << "~~~~~~~~~~~~"
        << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

    Sleep(300);
    system("CLS");

    cout << setw(43) << "      ~ ~ ~ ~ ~ ~ ~ ~\n" << setw(46) << "  ~~{Choose Option: }~~\n" << setw(47) << "  ~~{1.New Game       }~~\n" << setw(48)
        << " ~~{2.High Scores      }~~\n" << setw(49) << "~~{3.Options           }~~\n" << setw(48)
        << " ~~{4.Admin Access     }~~\n" << setw(47) << "  ~~{5.Help           }~~\n" << setw(46) << "  ~~{6.Exit        }~~\n" << setw(43) << "      ~ ~ ~ ~ ~ ~ ~ ~"
        << "\n\n";

}

void animatedmenu()
{
    cout << setw(43) << "      ~ ~ ~ ~ ~ ~ ~ ~\n" << setw(46) << "  ~~{Choose Option: }~~\n" << setw(47) << "  ~~{1.New Game       }~~\n" << setw(48)
        << " ~~{2.High Scores      }~~\n" << setw(49) << "~~{3.Options           }~~\n" << setw(48)
        << " ~~{4.Admin Access     }~~\n" << setw(47) << "  ~~{5.Help           }~~\n" << setw(46) << "  ~~{6.Exit        }~~\n" << setw(43) << "      ~ ~ ~ ~ ~ ~ ~ ~"
        << "\n\n";
}

int main()
{
    animation();
    game_menu();
    return 0;
}

void game_menu()
{
    ::winorlose = 0; //written so that the game does not ask for name when exiting
    system("CLS");
    animatedmenu();

    static int input = 0;//initializing local input variable dictating case switch statement
    cout << "Enter number to perform action (Exit code for any menu is -12345): ";
    cin >> input;

    system("CLS");

    switch (input)
    {
    case 1://playing game
        game_main();
        break;

    case 2://displaying the highscores
        File_part(-1);
        break;

    case 3://settings for custom game and stuff
        cout << "\nPress\n1: Standard Game\n2: Custom Game\nInput:";
        cin >> input;
        settings(input);
        break;
    case 4://admin access
        if (auth == 1)
        {
            cout << "Press\n1: To exit admin mode\n2:Use admin Powers\nInput: ";
            cin >> input;

            switch (input)
            {
            case 1:
                return admin();

            case 2:
                return admin_powers();

            default:
                cout << "invalid Input, resetting system";
                return game_menu();
            }
        }

        else
            admin();

        break;

    case 5://exiting the game
        cout << "\n1. How to Play Minesweeper\n2.Explain Game Options\nEnter your option: ";
        cin >> input;

        if (input == 1)
        {
            cout << "\nMinesweeper is played by selecting cells of a grid. \nThe grid that will appear will consist of rows and columns.";
            cout << "\n\nYou will be prompted for the row and column of the cell. You cannot enter a number higher than the max length of rows and columns or lower than 1.";
            cout << "\n> represents a flag being placed on the grid.\n\n Once you've opened every space on the grid, you will win the game. If you hit a bomb, you will lose the game";
            cout << "\nopening up one space will open any other adjacent spaces that do not surround a bomb";
            cout << "\n\n0-9 Indicates the number of bombs surrounding the cell. 9 indicates that the space itself is a bomb.\n";
        }

        if (input == 2)
        {
            cout << "\n\"1.New Game\" lets you start a new game using the current settings for Rows, Columns and Bombs.";
            cout << "\n\n\"2.High Scores\" lets you check the current highscores of the entered players.";
            cout << "\n\n\"3.Options\" lets you adjust the number of Rows, Columns and Bombs in a game. You can also select the default values for them.";
            cout << "\n\n\"4.Admin Access\" lets you get admin access as long as you know the password. It lets you see which spaces have bombs and allows you to alter the leaderboard.";
            cout << "\n\n\"5.Help\" lets you get more information on how to run the program and how it works.";
            cout << "\n\n\"6.Exit\" lets you exit the game. You can also accomplish this by entering the exit code.";
            cout << "\n\nWriting the exit code (-12345) lets you return to the main menu from anywhere.";
            //This work is inspired by the game maker Todd Howard.
            //Any problem found in the game is NOT a bug, rather an easter egg feature
        }
        break;

    case 6:
        break;//exiting the game

    default:
        cout << "Invalid input, re-enter input\n";

    }

    if (input != 6)
    {
        system("pause");
        system("CLS");
        return game_menu();
    }
}
void game_main()
{
    ::winorlose = 0;
    /*cout << "Enter no. of rows: ";
    cin>>rows;
    cout<<"Enter no. of columns: ";
    cin>>columns;*/

    vector<int> game_grid;

    for (int index = 0; index < rows * columns; index++)
        game_grid.push_back(0);

    int cords[2];

    for (int lines = 0; lines < rows * columns; lines++)//printing empty grid
    {
        if (lines % columns == 0 && lines != 0)
            cout << lines / columns << endl;

        cout << "[_] ";
    }

    cout << rows << endl << setw(2) << 1;

    for (int index = 1; index < columns; index++)
        cout << setw(4) << index + 1;

    cout << endl;

    int start = clock();//starts time after first inputs taken

    cout << "Enter row: ";//taking first inputs
    cin >> cords[0];

    if (cords[0] == exit_code)
        return game_menu();

    cout << "Enter column: ";
    cin >> cords[1];

    if (cords[1] == exit_code)
        return game_menu();

    cords[0] -= 1;
    cords[1] -= 1;

    vector <int> inputs;

    if (((cords[0] * columns) + cords[1] > rows * columns) || (cords[0] * columns) + cords[1] < 0)
    {
        cout << "Invalid Input, restarting the game...\n";
        system("pause");
        system("CLS");
        return game_menu();
    }

    inputs.push_back((cords[0] * columns) + cords[1]); //starting cord

    grid_generator(game_grid, cords);//generating a solved grid

    player_grid(game_grid, inputs);//going to the function where the player will play

    int end = clock();//ends time when the player has won/lost
    //cout << endl << start << " to " << end << endl;
    int elasped_time = (end - start) / 1000;//as calculates in milliseconds

    //cout << elasped_time;
    if (::winorlose == 1)
        File_part(elasped_time);
}
//---------------------------------------------------------------------------------------------------------------------------
void grid_generator(vector<int>& game_grid, int* cords)
{
    int input_loc = (cords[0] * columns) + cords[1];//cords converted to index
    vector<int> starting_tiles;

    adjacent_opener(input_loc, starting_tiles);
    starting_tiles.push_back(input_loc);//as the adjacent opener only gives tiles adjacent as output, not the input tile. So we append input tile as well

    for (auto& it : starting_tiles)
        game_grid[it] = 10;

    bombs_location(totalbombs, game_grid);

    ans_grid(game_grid);

}
//--------------------------------------------------------------------------------------------------------------------------
void bombs_location(int bombs, vector<int>& input)
{
    srand(time(0));
    //int tiles=size;

    for (; bombs > 0; bombs--)//itering through flat array and placing bombs
    {
        int position = rand() % (rows * columns);//rand pos

        if (input[position] == 10 || input[position] == 9)//checks if the random spot has a 10 or anther bomb, then cannot place bomb there
        {
            bombs++;
            continue;
        }
        input[position] = 9;
    }
    //return input;//as array is already a pointer so we return only arr
}
//---------------------------------------------------------------------------------------------------------------------------
void ans_grid(vector<int>& grid)
{
    for (int index = 0; index < rows * columns; index++)
    {
        if (grid[index] >= 10)//to make the points user entered into default zeros
            grid[index] -= 10;

        if (grid[index] == 9)//placing numbers around grid 
        {
            vector<int> adj_bomb_tiles;
            adjacent_opener(index, adj_bomb_tiles);//getting locations of tiles adjacent to the bomb tile

            for (auto& it : adj_bomb_tiles)
                (grid[it] != 9 ? grid[it]++ : grid[it] += 0);//adding one to each tile unless that tile has a 9, in that case we add 0 as bombs be bombs
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------
void adjacent_opener(int input_loc, vector<int>& locations)//returns an array with the adjacent possible blocks
{//it first checks for repitition of cords, as all indexes are different, then checks for the locations
//and then gives more locations to open.
    for (int col_iter = 0 - columns; col_iter <= columns; col_iter += columns)
        if (input_loc + col_iter < rows * columns && input_loc + col_iter >= 0)
        {
            if (input_loc + col_iter != input_loc)
            {
                bool repeat = std::find(locations.begin(), locations.end(), input_loc + col_iter) != locations.end();//finding all the open tiles

                if (repeat == false)
                    locations.push_back(input_loc + col_iter);
            }

            if (input_loc % columns != 0)
            {
                bool repeat = std::find(locations.begin(), locations.end(), input_loc + col_iter - 1) != locations.end();//finding all the open tiles

                if (repeat == false)
                    locations.push_back(input_loc + col_iter - 1);
            }
            // as the new row start with the index 5, so it has no adjacent tile on its left

            if ((input_loc + 1) % columns != 0)
            {
                bool repeat = std::find(locations.begin(), locations.end(), input_loc + col_iter + 1) != locations.end();//finding all the open tiles

                if (repeat == false)
                    locations.push_back(input_loc + col_iter + 1);
            }
            // as the row ends with the index 4, so it has no adjacent tile on its right
        }
}
//---------------------------------------------------------------------------------------------------------------------------
void player_grid(vector<int>& ans_sheet, vector<int>& prev_moves)
{
    int gameover = 0, init_size = prev_moves.size();//neutral, -1 for lose, +1 for win
    static vector<int> flags;//stores where player has placed flags

    if (init_size == 1)
        flags = {};//resetting flags if a new game starts

    do {

        for (int lines = 0; lines < rows * columns; lines++)//printing the grid
        {
            bool click = std::find(prev_moves.begin(), prev_moves.end(), lines) != prev_moves.end();//finding all the open tiles

            if (click)//prints the number from grid
            {

                if (ans_sheet[lines] == 0)//opening all spaces around 0
                {
                    adjacent_opener(lines, prev_moves);
                }

                else if (ans_sheet[lines] == 9)
                {
                    gameover = -1;//bomb EXPLODED so we set gameover to -1
                }

            }
        }

        if (prev_moves.size() > init_size)
            return player_grid(ans_sheet, prev_moves);

        system("CLS");//clearing output screen

        for (int lines = 0; lines < rows * columns; lines++)//printing the updated grid
        {
            if (lines % columns == 0)//making the rows of the grid
                cout << lines / columns << endl;

            bool click = std::find(prev_moves.begin(), prev_moves.end(), lines) != prev_moves.end();

            bool flag = std::find(flags.begin(), flags.end(), lines) != flags.end();

            if (click)
                cout << "[" << ans_sheet[lines] << "] ";

            else if (flag)
                cout << "[<] ";

            else
                cout << "[_] ";

        }

        cout << rows << endl << setw(2) << 1;

        for (int index = 1; index < columns; index++)//printing the column indexes for ease of user
            cout << setw(4) << index + 1;

        cout << endl;

        cout << endl << prev_moves.size() << endl << "Flags placed: " << flags.size() << "/" << totalbombs << endl;

        if (gameover == -1)//checks if any bomb EXPLODED yet
        {
            cout << "BOMB EXPLODED. YOU LOSE" << endl; break;
        }

        else if (prev_moves.size() == (rows * columns) - totalbombs)//need to open all grids not containing
        {
            cout << "BOMBS FOUND. YOU WIN" << endl;
            winorlose = 1;
            break;
        }

        //ANS GRID
        if (::auth == 1)//admin can see the answer grid for any bugs or glitches
        {
            for (int index = 0; index < rows * columns; index++)//printing background grid
            {
                if (index % columns == 0)
                    cout << endl;
                cout << setw(3) << ans_sheet[index] << " ";
            }
            cout << endl;
        }

        int flag_placer;

        cout << "To place flag press 1, to open a space press 2: ";
        cin >> flag_placer;

        if (flag_placer == exit_code)
            return game_menu();

        int next_cords[2];
        cout << "Enter row: ";
        cin >> next_cords[0];

        if (next_cords[0] == exit_code)//escape sequence in middle of the game
            return game_menu();

        cout << "Enter column: ";
        cin >> next_cords[1];

        if (next_cords[1] == exit_code)//escape sequence in middle of the game
            return game_menu();

        next_cords[0] -= 1;
        next_cords[1] -= 1;

        int index = (next_cords[0] * columns) + next_cords[1];

        if (index > rows * columns)
        {
            cout << "Invalid Inputs\n";
            system("pause");
        }

        else if (flag_placer == 2)//clicking an empty space
        {
            bool repeat = std::find(prev_moves.begin(), prev_moves.end(), index) != prev_moves.end();//finding all the open tiles

            if (repeat == false)
                prev_moves.push_back(index);
        }

        else if (flag_placer == 1)// placing/removing a flag
        {
            bool repeat = std::find(flags.begin(), flags.end(), index) != flags.end();//finding all flaged tiles

            bool repeat_tile = std::find(prev_moves.begin(), prev_moves.end(), index) != prev_moves.end();//prevent placing flag on open tile

            if (repeat == false && repeat_tile == false)//as no previous flag on that location we place the flag
                flags.push_back(index);

            else if (repeat == true)//we are removing the flag otherwise
            {
                for (int it = 0; it < flags.size(); it++)
                {
                    if (flags[it] == index)
                        flags.erase(flags.begin() + it);//deletes the specified element
                }
            }
        }

        else
        {
            cout << "Invalid input, resetting board\n";
            system("pause");
            system("CLS");
        }

    } while (gameover == 0);
}

void File_part(int times)
{
    fstream Scores;
    vector<players> winners;
    string backup;
    string filename = "scoresheet.txt";

    Scores.open(filename, ios::in);//checking if file has anything in it
    string lines;

    while (getline(Scores, lines))
        backup += lines + "\n";

    Scores.close();

    if (backup.size() == 0)//meaning file is empty so we write it again
        backup = rewrite(Scores, total_pos);

    new_player_info(winners, times, filename, backup);

    //if (auth == 1 && times != -1)
        //admin_powers();
    if (times == -1)
        display_scores(filename, Scores);
}//----------------------------------------------------------------------------------------------------------------------------------------------------
void top_players(vector<players>& arr, string filename)
{
    fstream Scores;//opening the scores file
    Scores.open(filename, ios::in);

    vector<int> scores;//letting the previous highscores be stored in the player arr
    string lines;
    while (getline(Scores, lines))//getting all the scores
    {
        players bois;
        if (regex_match(lines, regex("([0-9]{1,2}.)\t[A-z]{1,4}\t[0-9]{1,3}")))//filter to only get the filled spaces with names
        {
            regex regscore("[0-9]{1,3}[^.]");//defining the string/pattern to find the score

            smatch highscore;

            regex_search(lines, highscore, regscore);//filter for getting the scores only

            bois.time_taken = stoi(highscore[0]);//taking the the only element in highscore and making it int

            smatch saved_name;

            regex regname("[A-z]{1,4}");//defining the string/pattern to find a name

            regex_search(lines, saved_name, regname);//filter for getting the name only

            bois.name = saved_name[0];//saving players name

            int pos = 1;

            int i = 0;
            for (players& ppl : arr)
            {
                if (bois.name == ppl.name)
                {
                    i = -1;
                    if (bois.time_taken < ppl.time_taken)
                        ppl.time_taken = bois.time_taken;
                }

                else if (ppl.time_taken < bois.time_taken)//checking the possible position of the user
                    pos++;

                else
                    ppl.position += 1;
            }

            bois.position = pos;

            if (i != -1)
                arr.push_back(bois);
        }
    }
    Scores.close();

    sorter(arr);//sorting the ppl in leaderboard
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void sorter(vector<players>& nums)
{//this function reads the value in the given index (e.g 5) and swaps the values of this index with the value on index of the value
    //e.g the avalue stored at index 5
    int iterations = 1, index;

    while (iterations != 0)
    {
        iterations = 0;
        index = 0;
        for (auto& scores : nums)
        {
            int it = scores.position;
            //cout<<it<<endl;
            int pos = it % (nums.size() + 1) - 1;
            if (it > nums[pos].position)
            {
                swap(nums[index], nums[pos]);
                iterations++;
                continue;
            }

            else
                index++;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void new_player_info(vector<players>& arr, int time, string filename, string old_data)//updating the file with new highscores
{
    if (time == -1 || auth == 1)//signals to not trigger file writing
        return;
    string name;
    cout << "Enter 4 character name: ";
    //cin.ignore('\n');//ignoring the \n
    cin.ignore();
    getline(cin, name);//getting whole name, with spaces

    if (!regex_match(name, regex("[[:alpha:]]{1,4}")))//checking is name is of proper size
    {
        cout << "\nName too long, please re-nter\n";
        return new_player_info(arr, time, filename, old_data);
    }

    cout << "Congratz " << name << " on finishing the game in " << time << " seconds\n";

    players new_player;//creating the new player
    new_player.name = name;
    new_player.time_taken = time;

    arr.push_back(new_player);

    top_players(arr, filename);//adjusting the new player in the leader_board
    file_writer(arr);//writing into the files the new updated player list
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void file_writer(vector<players>& arr)
{
    string newdata = "";
    int gamers = 0, counter = 1;
    fstream Scores;
    string lines;

    Scores.open(filename, ios::in);

    while (getline(Scores, lines))//reading the files and placing the players in their respective positions
    {

        if (gamers < arr.size())//checking in case not all the highscore labels are filled
        {
            newdata += to_string(arr[gamers].position) + ".\t" + arr[gamers].name + "\t" + to_string(arr[gamers].time_taken);
            gamers++;
        }

        else
            newdata += to_string(counter) + ".";
        newdata += "\n";
        counter++;
    }

    Scores.close();

    Scores.open(filename, ios::out);//making the new score sheet
    Scores << newdata;
    Scores.close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
string rewrite(fstream& Scores, int total_pos)
{
    Scores.close();//incase a file is previously opened

    string backup;

    for (int nums = 1; nums <= total_pos; nums++)//making the backup stirng 
        backup += to_string(nums) + ".\n";

    Scores.open(filename, ios::out);
    Scores << backup;

    Scores.close();
    return backup;
}

void display_scores(string filename, fstream& Scores)
{
    string s1;
    Scores.open(filename, ios::in);//opening file as read only
    cout << "\tNames\tScores\n";
    while (getline(Scores, s1))//reading and printing eaxh line
        cout << s1 << endl;;
}

void admin()
{
    if (::auth == 1)
    {
        ::auth = 0;
        cout << "\nExiting admin access\n";
        return game_menu();
    }
    cout << "Enter pass: ";
    string password = "";
    char alphanum = ' ';
    while (true)//loop runs until enter is hit
    {
        alphanum = _getch();
        if (alphanum == '\n' || alphanum == '\r')//detects enter being hit so exits the loop
        {
            cout << "\nEnter pressed\n";
            break;
        }
        cout << "*";
        password += alphanum;
    }

    if (password == ::adminpass)
        ::auth = 1;

    cout << (::auth == 1 ? "Welcome Admin, Admin access avaliable" : "Invalid password") << endl;//checks whether auth has entered the right password
}

void settings(int& input)//to save memory we re use the input variable
{
    string words[3] = { "rows", "columns", "bombs" };
    int cords[3] = { ::std_rows,::std_cols,::std_bombs };
    switch (input)
    {
    case 1:
        ::rows = ::std_rows;
        ::columns = ::std_cols;
        ::totalbombs = ::std_bombs;
        break;

    case 2:
        for (int index = 0; index < 3; index++)
        {
            cout << "\nEnter custom " << words[index];
            if (index < 2)
                cout << " (should be smaller than or equal to 45): ";
            else
                cout << ": ";
            cin >> input;

            if (input == exit_code)
                break;

            if ((cords[index] > 45 || cords[index] <= 5) && index != 2)//only for rows and columns
            {
                cout << "Invalid set of rows or columns enter, please re-enter the inputs\n";
                input = 2;
                settings(input);
            }

            else if ((input > (cords[0] * cords[1]) - 16 || input < 1) && index == 2 || input == exit_code) // for bombs
            {
                cout << "Invalid number of bombs\n";
                input = 2;
                settings(input);
            }


            cords[index] = input;
        }

        if (input == exit_code)
            return game_menu();

        ::rows = cords[0];
        ::columns = cords[1];
        ::totalbombs = cords[2];
        break;

    case (exit_code)://escape sequence
        return game_menu();

    default:
        cout << "Invalid Input, please re-enter: ";
        cin >> input;
        return settings(input);//using recursion to get desired input
    }
}

void admin_powers()
{
    int input = 0;
    int timeinput = 0;
    string input2 = "0";
    cout << "Press\n1: Put more position on the leader board\n2: Clear the leaderboard\n3: Delete a specific score on the leader board\n4: Search Leaderboard by Name";
    cout << "\n5: Add a new player on the leaderboard";
    cout << "\nInput: ";
    cin >> input;
    fstream Scores;

    vector<players> gamers;
    switch (input)
    {
    case 1://Updating the leader board to accomadate more positions
        cout << "Enter new total number of positions: ";
        cin >> input;

        if (input < 1 && input > 45)//45 is cap as that is the maximum number of lines which can be displayed on the cmd prompt at once
        {
            if (input == exit_code)
                return game_menu();

            cout << "Invalid Input, pleae re-enter\n";
            break;
        }

        ::total_pos = input;
        cout << "Resetting leaderboard...\n";
        system("pause");

    case 2:
        Scores.open(filename, ios::out);//leader board clear
        Scores.close();
        File_part(-2);//so the admin can see the changes
        break;

    case 3:
        File_part(-1);//displaying highscore
        cout << "\n Enter the users position to delete him from the leaderboard: ";
        cin >> input;

        top_players(gamers, filename);//making the array gamers have all the players infos currently on the leaderboard

        if (input > gamers.size() || input < 1)
        {
            if (input == exit_code)
                return game_menu();

            cout << "Invalid deletion, returning to admin screen\n";
            system("pause");
            system("CLS");
            return admin_powers();

        }

        gamers.erase(gamers.begin() + input - 1);
        for (int index = 1; index <= gamers.size(); index++)
            gamers[index - 1].position = index;
        file_writer(gamers);
        break;

    case 4:

        cout << "\nEnter the name of the person who you're searching out: ";
        cin >> input2;

        while (input2 != "-12345" && (size(input2) < 4 || size(input2) > 4))
        {
            cout << "\nIncorrect. Enter a valid name: ";
            cin >> input2;
        }

        top_players(gamers, filename);
        for (auto& j : gamers)
            if (j.name == input2)
            {
                cout << "\nPosition: " << j.position << "  Name: " << j.name << "  Score: " << j.time_taken << endl;
                system("pause");
            }
        break;

    case 5:
        cout << "\nEnter your win time in seconds (only enter a valid time): ";
        while (timeinput < 1 || timeinput > 999)
            cin >> timeinput;

        ::auth = 0;
        File_part(timeinput);
        ::auth = 1;
        break;

    case exit_code:
        break;

    default:
        cout << "Invalid Input, please re-enter: ";
        return admin_powers();
    }

    return game_menu();
}
