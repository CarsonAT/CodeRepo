#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <sqlite3.h>

static int callback(void *Notused, int argc, char **argv, char **azColName){
    for(int iter = 0; iter<argc; iter++){
        printf("%s = %s\n", azColName[iter], argv[iter] ? argv[iter] : "NULL");
    }
    printf("\n");
    return 0;
}

int createTable(const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg){
    rc = sqlite3_exec(Movie, sql, callback, NULL, &zErrMsg);

    if(rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table Initialized\n");
        system("sleep .5");
    }
    return 0;
}

int executeQuery(const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_close(Movie);
        return 1;
    }

    rc = sqlite3_step(stmt);

    if(rc != SQLITE_DONE){
        std::cerr << "SQL error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(Movie);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int login(int &UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt *stmt){
    
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        //sqlite3_close(Movie);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, UserID);

    int id;
    while(sqlite3_step(stmt) == SQLITE_ROW){
        id = sqlite3_column_int(stmt, 0);
        //std::cout << id << std::endl;
        if(id == UserID){
            sqlite3_finalize(stmt);
            return 1;
        }
    }

    sqlite3_finalize(stmt);

    return 0;
}

int Create_User(int &UserID, const char* Username, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt *stmt){
    
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, UserID);
    sqlite3_bind_text(stmt, 2, Username, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if(rc != SQLITE_DONE){
        std::cerr << "User input is invalid or already taken, please input another User ID that is not taken.";
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);

    return 1;
}

int Add_Movie(int UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt *stmt){
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    int movieID;
    std::string moviename;
    bool categ;
    
    std::cout << "Please Enter your movie ID for the added movie: " << std::endl;
    std::cin >> movieID;
    std::cout << "Enter the title of your movie: " << std::endl;
    std::cin.ignore();
    getline(std::cin, moviename);
    std::cout << "Did you watch the movie? (1 for yes, 0 for no): " << std::endl;
    std::cin >> categ;

    sqlite3_bind_int(stmt, 1, movieID);
    sqlite3_bind_text(stmt, 2, moviename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, categ);
    sqlite3_bind_int(stmt, 4, UserID);

    rc = sqlite3_step(stmt);

    if(rc != SQLITE_DONE){
        std::cerr << "User input is invalid or already taken, please input another Movie ID that is not taken.";
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    std::cout << "Movie Succesfully Added" << std::endl;
    executeQuery(("UPDATE User SET Movie_count = Movie_count + 1 WHERE (User.ID = " + std::to_string(UserID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    executeQuery(("INSERT INTO category VALUES(" + std::to_string(movieID) + ", " + std::to_string(categ) + ", 0, 0);").c_str(), rc, Movie, zErrMsg, stmt);
    return 1;
}


int Edit_Movie(const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    //Utilize exec since its just updating.
    system("clear");
    int edit;
    bool editing = true;
    int MovieID;
    std::string value;
    std::cout << "Enter the Movie ID that youd like to modify: " << std::endl;
    std::cin >> MovieID;
    do {
        system("clear");
        std::cout << "Please Pick one of these to modify of your chosen movie: " << std::endl;
        std::cout << "1 : Add Description\n2 : Add Director\n3 : Add Movie Length\n4 : Add feature Date" << std::endl;
        std::cout << "\nAwaiting user input (enter 0 to exit): ";
        std::cin >> edit;
        if(edit != 0){
            std::cout << "Enter the value you would like to insert: ";
            std::cin.ignore();
            getline(std::cin, value);
        }
        switch(edit){
            case 1: //Description
                executeQuery(("UPDATE Movie SET description = \'" + value + "\' WHERE (Movie.ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 2: //Director
                executeQuery(("UPDATE Movie SET director = \'" + value + "\' WHERE (Movie.ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 3: //Length
                executeQuery(("UPDATE Movie SET length = " + value + " WHERE (Movie.ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 4: //Date_Featured
                executeQuery(("UPDATE Movie SET date_featured = \'" + value + "\' WHERE (Movie.ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 0: //Exit
                std::cout << "Returning to menu..." << std::endl;
                editing = false;
                sleep(1);
                break;
            default:
                std::cout << "Make sure to choose a valid option." << std::endl;
                sleep(1);
                break;
        }
    } while(editing);
    return 1;
}


int Delete_Movie(int UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    system("clear");

    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3.close(Movie);
        return 0;
    }  

    int MovieID;

    std::cout << "Please enter the Movies Id that you would like to delete: ";
    std::cin >> MovieID;

    executeQuery(("DELETE FROM Category WHERE(Movie_ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    executeQuery( ("DELETE FROM Movie_Is_Genre WHERE(Movie_ID) = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);

    sqlite3_bind_int(stmt, 1, MovieID);
    sqlite3_bind_int(stmt, 2, UserID);

    rc = sqlite3_step(stmt);

    if(rc != SQLITE_DONE){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    std::cout << "Movie Successfully Deleted." << std::endl;
    sleep(5);
    return 1;
}


int Search_Movie(int movieID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    system("clear");

    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, movieID);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }
    //sleep(5);
    sqlite3_finalize(stmt);

    return 1;
}


int Search_Genre(int UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    
    sql = "SELECT * FROM Movie WHERE (Movie.User_ID = ? AND Movie.ID = (SELECT Movie_ID FROM Movie_Is_Genre WHERE (Genre_ID = ?)));";

    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    int Genre_ID;
    std::cout << "Please Enter a Genre ID:";
    std::cin >> Genre_ID;

    sqlite3_bind_int(stmt, 1, Genre_ID);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }
    //sleep(5);
    sqlite3_finalize(stmt);
    return 1;
}


int Set_Genre(int &rc, const char* sql, sqlite3 *Movie, char* zErrMsg, sqlite3_stmt* stmt){
    int GenreID;
    int MovieID;

    std::cout << "Please input a movies id you like to use: ";
    std::cin >> MovieID;
    std::cout << "Please input a genres id you like to attach it with: ";
    std::cin >> GenreID;
    
    executeQuery(("INSERT INTO Movie_Is_Genre VALUES(" + std::to_string(MovieID) + ", " + std::to_string(GenreID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    return 1;
}


int Display_Category(int &rc, const char* sql, sqlite3 *Movie, char* zErrMsg, sqlite3_stmt* stmt){
    system("clear");
    int MovieID;
    std::cout << "Input the movies ID to show its category: ";
    std::cin >> MovieID;
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, MovieID);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }
    //sleep(5);
    sqlite3_finalize(stmt);
    return 1;
}


int Change_Category(int UserID, int &rc, sqlite3 *Movie, char* zErrMsg, sqlite3_stmt* stmt){
    int MovieID;
    bool stay = true;
    int option;

    std::cout << "Enter the movies id you would like to modify: ";
    std::cin >> MovieID;

    do{
        system("clear");
        std::cout << "What would you like to do?\n 1 : Toggle Watched\n 2 : Toggle Favorite\n 3 : Toggle Watchlist\n -"; 
        std::cin >> option;
        switch(option){
            case 1:
                executeQuery(("UPDATE Category SET Watched = !Watched WHERE Movie_ID IN (SELECT Movie_ID FROM Movie WHERE Movie.ID = ?" + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 2:
                executeQuery(("UPDATE Category SET Favorited = !Favorited WHERE (Movie_ID = ?" + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 3:
                executeQuery(("UPDATE Category SET Watchlisted = !Watchlisted WHERE (Movie_ID = ?" + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
                break;
            case 0:
                std::cout << "returning to main...";
                sleep(3);
                stay = false;
                break;
            default:
                std::cout << "Invalid input, try one of the 4 cases displayed.";
                break;
        }
    } while(stay);
    return 1;
}


int List_User_Tags(int UserID, const char* sql, int &rc, sqlite3* Movie, char* zErrMsg, sqlite3_stmt* stmt){
    sql = "SELECT * FROM User_Tags WHERE(User_ID = ?)";
    
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, UserID);
    
    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }

    sqlite3_finalize(stmt);
    
    return 1;
}


int Search_User_Tag(int UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    sql = "SELECT * FROM Movie WHERE Movie.ID IN (SELECT Movie_ID FROM User_Tags WHERE(User_ID = ? AND Def_Trait = \'?\'))";
    
    rc = sqlite3_prepare_v2(Movie, sql, -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cerr << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    std::string trait;

    std::cout << "Enter the defining trait for your tag: ";
    getline(std::cin, trait);
    std::cin.ignore();

    sqlite3_bind_int(stmt, 1, UserID);
    sqlite3_bind_text(stmt, 2, trait.c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }
    //sleep(5);
    sqlite3_finalize(stmt);
    
    return 1;
}


int Create_User_Tag(int UserID, const char* sql, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    std::string trait;
    std::string desc;
    int MovieID;

    std::cout << "Enter the movies Id you would like to add the tag to: ";
    std::cin >> MovieID;
    std::cin.ignore();
    std::cout << "Enter the tags main trait/title: ";
    getline(std::cin, trait);
    std::cout << "Enter a description for the tag: ";
    getline(std::cin, desc);
    
    executeQuery(("INSERT INTO User_Tags VALUES(" + std::to_string(UserID) + ", \'" + trait + "\', \'" + desc + "\', " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    
    return 1;
}


int Add_Footnote(int UserID, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    std::string title;
    std::string description;
    int MovieID;
    
    std::cout << "Enter the movie ID you'd like to add the footnote to: ";
    std::cin >> MovieID;
    std::cin.ignore();
    std::cout << "Enter your title for the footnote: ";
    getline(std::cin, title);
    std::cout << "Enter a description for your footnote: ";
    getline(std::cin, description);
    executeQuery(("INSERT INTO Footnote VALUES(" + std::to_string(UserID) + ", \'" + title + "\', \'" + description + "\', " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    executeQuery(("UPDATE Movie SET Footnote_Title = \'" + title + "\' WHERE Movie.ID = " + std::to_string(MovieID)).c_str(), rc, Movie, zErrMsg, stmt);
    return 1;
}


int Delete_Footnote(int UserID, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    std::string title;
    int MovieID;
    
    std::cout << "Enter the movie ID you'd like to add the footnote to: ";
    std::cin >> MovieID;
    executeQuery(("DELETE FROM Footnote WHERE (User_ID = " + std::to_string(UserID) + " AND Movie_ID = " + std::to_string(MovieID) + ");").c_str(), rc, Movie, zErrMsg, stmt);
    executeQuery(("UPDATE Movie SET Footnote_Title = \'N/A\' WHERE Movie.ID = " + std::to_string(MovieID)).c_str(), rc, Movie, zErrMsg, stmt);
    
    return 1;
}


int Display_Unwatched(int UserID, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    system("clear");
    rc = sqlite3_prepare_v2(Movie, "SELECT * FROM Movie WHERE (Movie.Watched_Category = 0 AND Movie.User_ID = ?);", -1, &stmt, 0);

    if(rc != SQLITE_OK){
        std::cout << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, UserID);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                std::cout << "NULL | ";
            else
                std::cout << sqlite3_column_text(stmt, iter) << " | ";
        }
        std::cout << std::endl;
    }

    sqlite3_finalize(stmt);

    return 1;
}


std::ostream& Display_All(int UserID, std::ostream &os, int &rc, sqlite3 *Movie, char *zErrMsg, sqlite3_stmt* stmt){
    system("clear");
    rc = sqlite3_prepare_v2(Movie, "SELECT * FROM Movie WHERE (Movie.User_ID = ?)", -1, &stmt, 0);

    if(rc != SQLITE_OK){
        os << "SQL Error: " << sqlite3_errmsg(Movie) << "\n";
        sqlite3_free(zErrMsg);
        //sqlite3_close(Movie);
        return os;
    }

    sqlite3_bind_int(stmt, 1, UserID);

    while(sqlite3_step(stmt) != SQLITE_DONE){
        int count = sqlite3_column_count(stmt);
        for(auto iter = 0; iter < count; iter++){
            if(sqlite3_column_type(stmt, iter) == SQLITE_NULL)
                os << "NULL | ";
            else
                os << sqlite3_column_text(stmt, iter) << " | ";
        }
        os << std::endl;
    }

    sqlite3_finalize(stmt);
    
    return os;
}

