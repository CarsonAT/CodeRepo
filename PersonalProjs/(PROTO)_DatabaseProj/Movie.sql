CREATE TABLE IF NOT EXISTS User(
    ID INTEGER PRIMARY KEY CHECK (ID BETWEEN 1000 AND 9999), 
    Username VARCHAR(80) NOT NULL, 
    Movie_count INTEGER
);
CREATE TABLE IF NOT EXISTS Genre(
    Genre_ID INTEGER PRIMARY KEY, 
    Genre VARCHAR(80) NOT NULL, 
    Sub_Genre VARCHAR(80)
);
CREATE TABLE IF NOT EXISTS Category(
    Movie_ID INTEGER REFERENCES Movie(ID), 
    Watched BOOL PRIMARY KEY, 
    Favorited BOOL, 
    Watchlisted BOOL
);
CREATE TABLE IF NOT EXISTS Movie(
    ID INTEGER PRIMARY KEY, 
    Movie_Name VARCHAR(40) NOT NULL, 
    Description TEXT, 
    Director VARCHAR(20), 
    Length DOUBLE, 
    Date_Featured VARCHAR(30), 
    Watched_Category BOOL REFERENCES Category(Watched), 
    Footnote_Title VARCHAR(30) REFERENCES Footnote(Title), 
    User_ID INTEGER REFERENCES User(ID)
);
CREATE TABLE IF NOT EXISTS Footnote(
    User_ID INTEGER REFERENCES User(ID), 
    Title VARCHAR(40) PRIMARY KEY, 
    Description TEXT, 
    Movie_ID INTEGER REFERENCES Movie(id)
);
CREATE TABLE IF NOT EXISTS User_Tags(
    User_ID INTEGER REFERENCES User(ID), 
    Def_Trait VARCHAR(30) PRIMARY KEY, 
    Description TEXT, 
    Movie_ID INTEGER REFERENCES Movie(ID)
);
CREATE TABLE IF NOT EXISTS Movie_Is_Genre(
    Movie_ID INTEGER REFERENCES Movie(ID), 
    Genre_ID INTEGER REFERENCES Genre(ID)
);

INSERT INTO User VALUES(1000, 'CarsonA_', 1);

INSERT INTO Movies VALUES(1, 'The Lighthouse', 'A story about 2 men on the brink', 'Robert Eggers', 1.83, '2019', 1, NULL, 1000);

INSERT INTO Genre VALUES(1, 'Sci Fi', 'Philosophical');
INSERT INTO Genre VALUES(2, 'Horror', 'Atmospheric Horror');
INSERT INTO Genre VALUES(3, 'Thriller', 'Noir Thriller');
INSERT INTO Genre VALUES(4, 'Horror', 'Surrealist Horror');
INSERT INTO Genre VALUES(5, 'Drama', 'Serious Drama');