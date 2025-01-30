#include "functions.hpp"

int main(int argc, char* argv[]){
    sqlite3 *Movie; //Pointer towards Movie database
    char* zErrMsg = 0; //character to reference any sql errors
    int rc; //return statement for sqlite3 executions
    int functioncheck; //return for executions of functions
    const char* sql; //sql string
    sqlite3_stmt* stmt; //prepared sql statement  pointer

    printf("Currently loading the Movie Database...\n");
    
    system("sleep 1");

    //Opening the database
    rc = sqlite3_open("MovieDB.db", &Movie);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(Movie));
        return -1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    //Sql creations of each table within the Movie Database, using abstracted calls
    sql = "CREATE TABLE IF NOT EXISTS User(ID INTEGER PRIMARY KEY CHECK (ID BETWEEN 1000 AND 9999), Username VARCHAR(80) NOT NULL, Movie_count INTEGER);";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS Genre(Genre_ID INTEGER PRIMARY KEY, Genre VARCHAR(80) NOT NULL, Sub_Genre VARCHAR(80));";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS Category(Movie_ID INTEGER REFERENCES Movie(ID), Watched BOOL, Favorited BOOL, Watchlisted BOOL);";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS Movie(ID INTEGER PRIMARY KEY, Movie_Name VARCHAR(40) NOT NULL, Description TEXT, Director VARCHAR(20), Length DOUBLE, Date_Featured VARCHAR(30), Watched_Category BOOL REFERENCES Category(Watched), Footnote_Title VARCHAR(30) REFERENCES Footnote(Title), User_ID INTEGER REFERENCES User(ID));";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS Footnote(User_ID INTEGER REFERENCES User(ID), Title VARCHAR(40) PRIMARY KEY, Description TEXT, Movie_ID INTEGER REFERENCES Movie(id));";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS User_Tags(User_ID INTEGER REFERENCES User(ID), Def_Trait VARCHAR(30) PRIMARY KEY, Description TEXT, Movie_ID INTEGER REFERENCES Movie(ID));";
    createTable(sql, rc, Movie, zErrMsg);
    sql = "CREATE TABLE IF NOT EXISTS Movie_Is_Genre(Movie_ID INTEGER REFERENCES Movie(ID), Genre_ID INTEGER REFERENCES Genre(ID));";
    createTable(sql, rc, Movie, zErrMsg);
    //End of table verification/creation 
    
    /*
    executeQuery("INSERT INTO Genre VALUES(6, \'Sci-Fi\', \'Surreal/Philosphical\');", rc, Movie, zErrMsg, stmt);
    executeQuery("INSERT INTO Genre VALUES(7, \'Horror\', \'Atmospheric\');", rc, Movie, zErrMsg, stmt);
    executeQuery("INSERT INTO Genre VALUES(8, \'Horror\', \'Surrealist/Psychological\');", rc, Movie, zErrMsg, stmt);
    executeQuery("INSERT INTO Genre VALUES(9, \'Thriller\', \'Noir\');", rc, Movie, zErrMsg, stmt);
    executeQuery("INSERT INTO Genre VALUES(10, \'Drama\', \'Serious Drama\');", rc, Movie, zErrMsg, stmt);
    */

    /* sql to insert my user as a initial value, since already done
    sql = "INSERT INTO User VALUES(1001, \'TestUser\', 0);";
    functioncheck = executeQuery(sql, rc, Movie, zErrMsg, stmt);
    if(functioncheck == 1){
        printf("An error occured, exiting application...\n");
        return -1;
    }
    */
    /*
    sql = "INSERT INTO Movie VALUES(1, 'The Lighthouse', 'A movie about two men in a lighthouse', 'Robert Eggers', 1.83, '2019', 1, NULL, 1000);";
    functioncheck = executeQuery(sql, rc, Movie, zErrMsg, stmt);
    if(functioncheck == 1){
        printf("An error occured, exiting application... \n");
        return -1;
    }
    */

    bool menu = true;
    int options;
    int UserID;
    std::string promptUserCreate;
    
    //Main while for user prompting to load user "profile"
    do {
        system("clear");
        std::cout << "Please enter one of the login options... \n\t1 : Enter User Identification\n\t2 : Create a new user\nAwaiting User Input : "; 
        std::cin >> options;
        try {   
            if(std::cin.fail())
                throw std::runtime_error("User attempted to input string/char");
            if(options == 1) {
                system("clear");
                std::cout << "Please enter your User ID (A 4 digit integer) : ";
                std::cin >> UserID;
                if(std::cin.fail())
                    throw std::runtime_error("Not a valid User ID");
                sql = "SELECT ID FROM User WHERE (ID = ?)";
                functioncheck = login(UserID, sql, rc, Movie, zErrMsg, stmt);
                if(functioncheck){
                    std::cout << "User validated, entering main menu..." << std::endl;
                    sleep(3);
                    menu = false;
                } else {
                    std::cout << "User selection not valid\n";
                    sleep(3);
                }
            } else if(options == 2) {
                system("clear");
                std::cout << "Please enter a non used User ID (A 4 digit integer) : ";
                std::cin >> UserID;
                if(std::cin.fail())
                    throw std::runtime_error("Not a valid User ID");
                std::cout << "Please enter your username for this ID : ";
                std::cin >> promptUserCreate; 
                sql = "INSERT INTO User VALUES(?, ?, 0);";
                functioncheck = Create_User(UserID, promptUserCreate.c_str(), sql, rc, Movie, zErrMsg, stmt);
                if(functioncheck){
                    system("clear");
                    std::cout << "User creation validated, welcome to the database!\n";
                    sleep(3);
                    menu = false;
                } else {
                    std::cout << "\nResetting menu...";
                    sleep(1);
                }
            } else {
                system("clear");
                std::cout << "Invalid integer input by user, please try entering only 1 or 2" << std::endl;
                sleep(2);
                for(int iter = 0; iter < 3; iter++){ 
                    std::cout << '.'; 
                    std::cout.flush();
                    sleep(.5); 
                }
            }
        } catch(std::runtime_error err) { 
            system("clear");
            std::cout << "Please enter an integer of 1 or 2 : " << err.what() << std::endl; 
            sleep(3);
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
    } while(menu);
    
    std::cout.flush();
    system("clear");
    menu = true;
    std::string dummyval;
    
    do {
        system("clear");
        std::cout << "Welcome to the Movie Database! Please select one of these options:" << std::endl;
        std::cout << "| | 1 : Add a Movie | | 2 : Edit Movie | | 3 : Delete a Movie | | 4 : Search a Movie | | 5 : Display All Movies\n";
        std::cout << "| | 6 : Set a genre | | 7 : Search a genre | | 8 : Display Movies Categories | | 9 : Change movies category\n| | 10 : List User Tags ";
        std::cout << "| | 11 : Create a user tag | | 12 : Search a user tag | | 13 : Add a movie footnote \n| | 14 : Delete a movie footnote | | 15 : Display unwatched movies\n";
        std::cout << "(To exit, enter 0) : ";
        std::cin >> options;
        try {
            if(std::cin.fail())
                throw std::runtime_error("Invalid integer input by user, please enter within the desired ranged.");
            switch(options){
                case 1:
                    std::cout.flush();
                    sql = "INSERT INTO Movie VALUES(?, ?, 'N/A', 'N/A', 0.0, 'A year', ?, NULL, ?);";
                    Add_Movie(UserID, sql, rc, Movie, zErrMsg, stmt);
                    sleep(3);
                    break;
                case 2:
                    Edit_Movie(sql, rc, Movie, zErrMsg, stmt);
                    break;
                case 3:
                    sql = "DELETE FROM Movie WHERE (ID = ?);";
                    Delete_Movie(UserID, sql, rc, Movie, zErrMsg, stmt);
                    break;
                case 4:
                    std::cout.flush();
                    std::cout << "Please input the movies ID that you want to search: ";
                    std::cin >> options;
                    if(std::cin.fail())
                        throw std::runtime_error("Invalid integer input by user, please enter a valid integer.");
                    sql = "SELECT * FROM Movie WHERE (Movie.ID = ?);";
                    Search_Movie(options, sql, rc, Movie, zErrMsg, stmt);
                    std::cout << "Enter any value to continue: ";
                    std::cin >> dummyval;
                    break;
                case 5: //Display All
                    std::cout.flush();
                    Display_All(UserID, std::cout, rc, Movie, zErrMsg, stmt) << std::endl;
                    std::cout << "Enter any value to continue: " << std::endl;
                    std::cin >> dummyval;
                    break;
                case 6:
                    Set_Genre(rc, sql, Movie, zErrMsg, stmt);
                    break;
                case 7:
                    Search_Genre(UserID, sql, rc, Movie, zErrMsg, stmt);
                    std::cout << "Enter any key to continue: ";
                    std::cin >> dummyval;
                    break;
                case 8:
                    sql = "SELECT Watched, Favorited, Watchlisted FROM Category WHERE (Movie_ID = ?);";
                    Display_Category(rc, sql, Movie, zErrMsg, stmt);
                    std::cout << "Enter any key to continue: ";
                    std::cin >> dummyval;
                    break;
                case 9:
                    Change_Category(UserID, rc, Movie, zErrMsg, stmt);
                    break;
                case 10:
                    List_User_Tags(UserID, sql, rc, Movie, zErrMsg, stmt);
                    std::cout << "Enter any input to continue: ";
                    std::cin >> dummyval;
                    break;
                case 11:
                    Create_User_Tag(UserID, sql, rc, Movie, zErrMsg, stmt);
                    break;
                case 12:
                    Search_User_Tag(UserID, sql, rc, Movie, zErrMsg, stmt);
                    std::cin.ignore();
                    std::cout << "Enter any input to continue: ";
                    std::cin >> dummyval;
                    break;
                case 13:
                    Add_Footnote(UserID, rc, Movie, zErrMsg, stmt);
                    break;
                case 14:
                    Delete_Footnote(UserID, rc, Movie, zErrMsg, stmt);
                    break;
                case 15:
                    Display_Unwatched(UserID, rc, Movie, zErrMsg, stmt);
                    std::cout << "Enter any value to continue: " << std::endl;
                    std::cin >> dummyval;
                    break;
                case 0:
                    std::cout << "Thank you for checking out the Movie Database, have a good day!\n";
                    menu = false;
                    break;
                default:
                    std::cout << "Please enter a value within the desired range.\n" << std::endl;
                    break;
            }
        } catch(std::runtime_error &err) {
            system("clear");
            std::cout << "Please enter an integer between the range of 0-15 : \n" << err.what() << std::endl;
            sleep(3);
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
    } while(menu);

    sqlite3_close(Movie);

    return 0;
}

