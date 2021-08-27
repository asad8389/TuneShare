#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SONGS_PER_USER   10
#define MAX_USERS    		  5
#define MAX_ARTISTS		    100


#define NUM_TEST_USERS 		7


// This struct represents a song in the system
typedef struct {
	char      *title;
	char      *artist;
	short int  duration;
} Song;

// This struct represents a user in the system
typedef struct {
	char       *name;
	char        online; 				   // 1 = YES, 0 = NO
	Song       *songs[MAX_SONGS_PER_USER]; // songs this user has
	short int   numSongs;  				   // number of songs this user has
} User;

// This structure represents the Tune Share system
typedef struct {
	User  		*users[MAX_USERS];     // An array of all registered users
	short int	 numUsers;
} TuneShare;

//This structure keeps track of downloads per artist
typedef struct{
    char *artistName;
    short int numdownload;
}ArtistDownloads;

//this structure represent artists
typedef struct{
    ArtistDownloads *arr[MAX_ARTISTS];
    short int numArtists;
}Artist;

//Global structure variable declaration of Artist
Artist artists={NULL,0};
//artists->numArtists=0;



/* ADD ALL YOUR FUNCTIONS HERE */

//This is the function that adds a user
int registerUser(TuneShare *t, char *name){
    if(t->numUsers>=MAX_USERS)                  //checking whether max user not reached
        return 0;
    for(int i=0;i<t->numUsers;i++){
        if(strcmp(t->users[i]->name, name)==0)    //checking for the same name
            return 0;
    }
    User *x=(User *)malloc(sizeof(User ));         //allocating memory for user
    x->name=name;
    x->online='0';
    x->numSongs=0;
    t->users[t->numUsers]=x;                    //adding the user
    t->numUsers++;                              //increasing the user count
    return 1;
}

int addSong(User *u, char *title, char *artist, short int *duration){
    if(u->numSongs>=MAX_SONGS_PER_USER)         //checking for max songs per user not reached
        return 0;
    Song *s =(Song *)malloc(sizeof(Song));      //allocating memory for Song
    s->title=title;
    s->artist=artist;
    s->duration=*duration;
    u->songs[u->numSongs]=s;                   //adding the song
    u->numSongs++;                              //increasing the songs count
    return 1;
}

User* userWithName(TuneShare *t, char *name){
    for(int i=0;i<t->numUsers;i++){
        if(strcmp(t->users[i]->name,name)==0)     //checking for the name
            return t->users[i];                 //returning the pointer tot he name
    }
    return NULL;                                //returning NULL when name is not in list
}

void logon(TuneShare *t, char *name){
    User *x=userWithName(t, name);             //retrieving the pointer for name
    if(x==NULL){
        return;                                 //return if name not found
    }
    x->online='1';                               //change status to on line
}

void logoff(TuneShare *t, char *name){
    User *u=userWithName(t, name);             //retrieving the pointer for name
    if(u==NULL)
        return;                                 //return if name not found
    u->online='0';                              //change status to off line
}

User** onlineUsers(TuneShare *t, short int *numOnLine){
    User **ptr;
    short int n=0;
    for(int i=0;i<t->numUsers;i++){                             //loop for finding online users
        if(t->users[i]->online=='1'){
            int j=0; n++;                                       //increasing user count

            User **new_ptr = (User **)malloc(n*sizeof(User));   //allocating increased size memory to store online users

            if(new_ptr==NULL){
                printf("Memory not available\n");
                exit(0);
            }

            for(;j<n-1;j++)
                new_ptr[j]=ptr[j];                               //transferring data from old small pointer

            new_ptr[j]=t->users[i];                              //adding the new online user
            ptr=(User **)malloc(n*sizeof(User));
            for(int k=0;k<n;k++)
                ptr[k]=new_ptr[k];
            free(new_ptr);                                      //de-allocate the new_ptr memory
        }
    }

    *numOnLine=n;
    return ptr;
}

Song** allAvailableSongs(TuneShare *t, short int *numSongs){
    Song **ptr2;
    short int n1=0,n2=0,numonline;
    User **u= onlineUsers(t, &numonline);                       //getting the pointer to the online users

    for(int i=0;i<numonline;i++){
        if(u[i]->numSongs>0){
            n1+=u[i]->numSongs;

            Song **new_ptr= (Song **)malloc(n1*sizeof(Song));   //allocating memory for increased number of songs

            if(new_ptr==NULL){
                printf("Memory not available\n");
                exit(0);
            }

            int j=0;
            for(;j<n2;j++)
                new_ptr[j]=ptr2[j];                             //transferring songs from old pointer

            for(int k=0;k<u[i]->numSongs;k++)
                new_ptr[j++]=u[i]->songs[k];                    //adding new available songs
            n2=n1;
            ptr2=(Song **)malloc(n1*sizeof(Song));              //transferring all songs to old pointer by increasing its memory size
            for(int k=0;k<n2;k++)
                ptr2[k]=new_ptr[k];
            free(new_ptr);                                      //de-allocate the new_ptr memory

        }
    }
    *numSongs=n1;
    return ptr2;

}

void displayStats(TuneShare *t){
    short int numSong,numOnline;
    User **u;
    Song **s;
    u = onlineUsers(t, &numOnline);                 //getting online users pointer
    s = allAvailableSongs(t, &numSong);             //getting available songs pointer

    printf("\nTune Share Center (%d registered users)\n",t->numUsers);

    printf("\n%d Online Users:\n",numOnline);
    if(numOnline>0){                                //checking for online users
        for(int i=0;i<numOnline;i++){
            printf("\t%s with %d songs as follows:\n",u[i]->name,u[i]->numSongs);
            for(int j=0;j<u[i]->numSongs;j++)
                printf("\t\t\"%s\" by %s\n",u[i]->songs[j]->title,u[i]->songs[j]->artist);
        }
        free(u);
    }
    else{
        printf("\tNONE\n");                         //returning NULL if no user online
    }

    printf("\n%d Available Songs:\n",numSong);
    if(numSong!=0){                                 //checking for available songs
        for(int i=0;i<numSong;i++){
            printf("\t\"%s\" by %s (%d:%d)\n",s[i]->title,s[i]->artist,s[i]->duration/60,s[i]->duration%60);
        }
        printf("\n");
        free(s);
    }
    else{
        printf("\tNONE\n");                         //returning NULL when no song available
    }



}

void displayAvailableSongsByArtist(TuneShare *t, char *artist){
    Song **s;
    short int numSongs,i=0,c=0;
    s = allAvailableSongs(t, &numSongs);             //getting all available songs
    for(;i<numSongs;i++){
        if(strcmp(s[i]->artist,artist)==0){          //checking for our song with given artist name
            printf("\t\"%s\" by %s (%d:%d)\n",s[i]->title,s[i]->artist,s[i]->duration/60,s[i]->duration%60);
            c++;
        }
    }
    if(c==0)
        printf("\tNONE\n");                          //returning NULL if no song available with given artist name
    free(s);
}


Song* getSong(TuneShare *t, char *title, char *ownerName){
    User *u = userWithName(t, ownerName);               //getting pointer to user

    if(u==NULL || u->online=='0')
        return NULL;                                    //returning NULL if user is not available or offline
    else{
        for(int i=0;i<u->numSongs;i++){
            if(strcmp(u->songs[i]->title,title)==0)
                return u->songs[i];                     //returning the required song
        }
    }
    return NULL;                                        //returning NULL if song is not available in users list
}

void downloadSong(TuneShare *t, char *downloadToName, char *title, char *downloadFromName){
    User *from = userWithName(t, downloadFromName);         //pointer to downloadfromname
    User *to = userWithName(t, downloadToName);             //pointer to downloadtoname
    Song *s = getSong(t, title, downloadFromName);          //pointer to the song to download

    if( from==NULL || to==NULL || s==NULL || from->online=='0' || to->online=='0')
        return;                                             //checking for all the conditions

    else{
        if(to->numSongs<MAX_SONGS_PER_USER){
            to->songs[to->numSongs++]=s;                    //copying the song to user
            short int c=0;
            for(int i=0;i<artists.numArtists;i++){          //checking for artists
                if(strcmp(artists.arr[i]->artistName,s->artist)==0){
                    c++;
                    artists.arr[i]->numdownload++;          //increasing the download number of artist
                }
            }
            if(c==0){                                       //if artist not already present adding the artist
                ArtistDownloads *a =(ArtistDownloads *)malloc(sizeof(ArtistDownloads));
                a->artistName = s->artist;
                a->numdownload = 1;
                artists.arr[artists.numArtists]=a;
                artists.numArtists++;

            }
        }
    }
}

void displayRoyalties(TuneShare *t){
    for(int i=0;i<artists.numArtists;i++)                   //calculating royalties for all artists
        printf("$  %.2f %s\n",artists.arr[i]->numdownload*0.25,artists.arr[i]->artistName);
}

void shutDown(TuneShare *t){
    short int numonline;
    User **u = onlineUsers(t,&numonline);
    for(int i=0; i<numonline;i++)           //logging of all the users
        logoff(t, u[i]->name);
    free(u);
//    free(t);
}


// DO NOT MODIFY THE MAIN FUNCTION
int main() {
	// This is test data
	static char *TITLES[] = {"Back 2 Life", "Loneliness For Love", "Desire 126", "Perfect", "In My Head",
							 "Old Skool Love", "Let's Go", "No Place", "We Gotta Go", "How You Get the Girl",
							 "Catch", "Here Tonight", "Details", "Dangerous", "Brighter Than the Sun",
				  			 "Photograph", "Thinking Out Loud", "If Heaven Were to Fall", "I Just Wanna Be With You",
				  			 "Song Sung Blue", "Outta Style", "Why", };

	static char *ARTISTS[] = {"E-Type", "Lovely the Band", "Hollerado", "Ed Sheeran", "Ryland James",
							  "Divine Brown", "Group 1 Crew", "Backstreet Boys", "E-Type", "Taylor Swift",
							  "Brett Young", "Brett Young", "Billy Currington", "Kardinal Offichall",
							  "Colbie Caillat", "Ed Sheeran", "Ed Sheeran", "E-Type", "E-Type", "Neil Diamond",
				   			  "Aaron Watson", "Frankie Avalon", };

	static short   DURATIONS[] = {217, 237, 187, 263, 205, 204, 256, 179, 213, 247, 196,
								216, 201, 251, 231, 202, 281, 223, 230, 185, 222, 161};


	static char *TEST_USER_NAMES[NUM_TEST_USERS] = {"Disco Stew", "Peter Punk", "Country Candy", "Ronnie Rocker",
													"Sleeping Sam", "Disco Stew", "Mellow Marvin"};

	static int  LIST_SIZES[NUM_TEST_USERS] = {7, 9, 9, 5, 1, 0, 0};
	static int  SONGNUMS[NUM_TEST_USERS][MAX_SONGS_PER_USER] = {
								   {1, 2, 4, 5, 12, 15, 21}, {0, 1, 3, 8, 9, 13, 14, 17, 20},
								   {6, 7, 8, 10, 11, 12, 13, 20, 21}, {0, 8, 16, 17, 18}, {19}, {}, {}};


	// Create the TuneShare Center
	TuneShare   tuneShareCenter;
	tuneShareCenter.numUsers = 0;

	// Attempt to register all test users
	for (int i=0; i<NUM_TEST_USERS; i++) {
		if (!registerUser(&tuneShareCenter, TEST_USER_NAMES[i]))
			printf("Error adding User: \"%s\"\n", TEST_USER_NAMES[i]);
		else
			printf("User: \"%s\" has been registered\n", TEST_USER_NAMES[i]);
	}

	// Display some stats
	displayStats(&tuneShareCenter);

	// Log on a user
	printf("\nLogging on a user...\n");
	logon(&tuneShareCenter, "Disco Stew");

	// Display some stats
	displayStats(&tuneShareCenter);

	// Now add all the test songs for these test users
	for (int i=0; i<tuneShareCenter.numUsers; i++) {
		for (int j=0; j<LIST_SIZES[i]; j++)
			addSong(tuneShareCenter.users[i], TITLES[SONGNUMS[i][j]], ARTISTS[SONGNUMS[i][j]], &DURATIONS[SONGNUMS[i][j]]);
	}

	// Display some stats
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Now bring two more users online
	printf("\nLogging on three users...\n");
	logon(&tuneShareCenter, "Disco Stew");    // Already logged on ... does nothing
	logon(&tuneShareCenter, "Mellow Marvin"); // Won't Work
	logon(&tuneShareCenter, "Peter Punk");
	logon(&tuneShareCenter, "Country Candy");

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Log the last two users online
	printf("\nLogging on two more users...\n");
	logon(&tuneShareCenter, "Ronnie Rocker");
	logon(&tuneShareCenter, "Sleeping Sam");

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Log off a couple of users
	printf("\nLogging off some users...\n");
	logoff(&tuneShareCenter, "Country Candy");
	logoff(&tuneShareCenter, "Mellow Marvin"); // Won't work
	logoff(&tuneShareCenter, "Peter Punk");

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Have Sleeping Sam download some songs from other online users
	printf("\nSleeping Sam downloading 2 songs from Disco Stew and 2 from Ronnie Rocker...\n");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Old Skool Love", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "In My Head", "Disco Stew");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "If Heaven Were to Fall", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "I Just Wanna Be With You", "Ronnie Rocker");
	downloadSong(&tuneShareCenter, "Sleeping Sam", "We Gotta Go", "Country Candy");

	// Have Ronnie Rocker download from Sleeping Sam and Disco Stew
	printf("\nRonnie Rocker downloading a songs from Sleeping Sam and a song from Disco Stew...\n");
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Sleeping Sam");
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Desire 126", "Disco Stew");

	// None of these should work
	downloadSong(&tuneShareCenter, "Sleeping Sam", "Back 2 Life", "Disco Stew");		// Won't work ... not a song of his
	downloadSong(&tuneShareCenter, "Sleeping Sam", "No Place", "Mellow Marvin"); 		// Won't work, not a user
	downloadSong(&tuneShareCenter, "Mellow Marvin", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not registered user
	downloadSong(&tuneShareCenter, "Ronnie Rocker", "Song Sung Blue", "Mellow Marvin"); // Won't work, not registered user
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Sleeping Sam"); 	// Won't work, not logged on
	downloadSong(&tuneShareCenter, "Sleeping Song", "Popsicles", "Ronnie Rocker"); 		// Won't work, song doesn't exist

	// Log on Country Candy and have her download a song from Ronnie
	printf("\nLogging on Country Candy...\n");
	logon(&tuneShareCenter, "Country Candy");
	downloadSong(&tuneShareCenter, "Country Candy", "Song Sung Blue", "Ronnie Rocker");

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Log off the last few users
	printf("\nLogging off three more users...\n");
	logoff(&tuneShareCenter, "Disco Stew");
	logoff(&tuneShareCenter, "Ronnie Rocker");
	logoff(&tuneShareCenter, "Sleeping Sam");
	logoff(&tuneShareCenter, "Sleeping Sam"); // Won't Work now

	// Display some stats again
	displayStats(&tuneShareCenter);

	// Display all songs by E-Type
	printf("Available Songs By E-Type: \n");
	displayAvailableSongsByArtist(&tuneShareCenter, "E-Type");

	// Now display the royalties for all downloaded songs at 25 cents per song
	printf("\nHere are the royalties to be paid:\n");
	displayRoyalties(&tuneShareCenter);

	// Shut down the Tune Share center
	shutDown(&tuneShareCenter);
}
