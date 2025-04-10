#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *genre;
    char *artist;
    char *title;
    char *album;
    char *trackid;
    short popularity;
    bool shown;
} Song;

typedef struct SongArrayList {
    Song *songs;         
    size_t size;         
    size_t capacity;     
} SongArrayList;

typedef struct {
    char *genre;         
    int count;
} GenreInfo;

typedef struct LinkedListNode {
    GenreInfo data;
    struct LinkedListNode *next;
} LinkedListNode;

bool readInSongs(SongArrayList *songList);
void printSongList(SongArrayList * songList, int starting_index);
void generateUniqueGenres(LinkedListNode **ptrUniqueGenresLL, SongArrayList songList);
void printGenresAndCounts(LinkedListNode *ptrUniqueGenresLL);
bool isNumber(const char *input);
void loadSongsByGenre(SongArrayList *originalList, SongArrayList *filteredList, char *genre);

int main(){
    SongArrayList songList;
    songList.size = 0;
    songList.capacity = 10;
    songList.songs = (Song *)malloc(sizeof(Song) * songList.capacity);
    // CODE: probably need to initialize songList

    readInSongs(&songList);

    LinkedListNode *headUniqueGenreLL = NULL;

    

    generateUniqueGenres(&headUniqueGenreLL, songList);
    
    printf("Genre Totals:\n");
    printGenresAndCounts(headUniqueGenreLL);
    
    int index = 1;
    printSongList(&songList, 1);

    // CODE: Need browsing and navigation loop
    bool quit = false;
    int filter = -1;
    
    int genres = 0;
    char genreslist[genres][256];
    LinkedListNode *node = headUniqueGenreLL;

    while (node  != NULL){
        strcpy(genreslist[genres], node->data.genre);
        genres++;
        node = node->next;
    }
    SongArrayList filteredList;
    filteredList.size = 0;
    filteredList.capacity = 10;
    filteredList.songs = (Song *)malloc(sizeof(Song) * filteredList.capacity);
    bool to_clean = false;
    do{
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("Navigation\tn for next, p for previous, and q to quit, g for start, e for end, r for reset filter.\n");
        printf("Filters:\t");
        int genreNumber = 0;
        LinkedListNode *currentNode = headUniqueGenreLL;
        while (currentNode != NULL) {
            if (genreNumber != 0){
                printf(" - ");
            }
            printf("[%d] %s (%d)", genreNumber, currentNode->data.genre, currentNode->data.count);
            currentNode = currentNode->next;
            genreNumber++;
        }
        printf("\n");
        char choice[256];
        scanf("%s", choice);
        if (strcmp(choice, "q") == 0){
            quit = true;
        }
        else if (strcmp(choice, "n") == 0){
            if (filter == -1){
                if (index + 10 <= songList.size){
                    index += 10;
                    
                }else{
                    if ((int)songList.size + 1 - 10 > 0){
                        index = songList.size + 1 - 10;
                    }else{
                        index = 1;
                    }
                }
                printSongList(&songList, index);
            }else{
                if (index + 10 <= filteredList.size){
                    index += 10;
                    
                }else{
                    if ((int)filteredList.size + 1 - 10 > 0){
                        index = filteredList.size + 1 - 10;
                    }else{
                        index = 1;
                    }
                }
                printSongList(&filteredList, index);
            }
        }
        else if (strcmp(choice, "p") == 0){
            if (filter == -1){
                if (index - 10 > 0){
                    index -= 10;   
                }else{
                    index = 1;
                }
                printSongList(&songList, index);
            }else{
                if (index - 10 > 0){
                    index -= 10;
                }else{
                    index = 1;
                }
                printSongList(&filteredList, index);
            }
        }
        else if (strcmp(choice, "g") == 0){
            index = 1;
            
            if (filter == -1){
                printSongList(&songList, index);
            }else{
                printSongList(&filteredList, index);
            }
        }
        else if (strcmp(choice, "e") == 0){
            if (filter == -1){
                if (songList.size + 1 - 10 > 0){
                    index = songList.size + 1 - 10;
                }else{
                    index = 1;
                }
                printSongList(&songList, index);
            }else{
                if (((int)filteredList.size + 1 - 10) > 0){
                    index = filteredList.size + 1 - 10;
                }else{
                    index = 1;
                }
                printSongList(&filteredList, index);
            }
        }
        else if (isNumber(choice)){
            int selection = atoi(choice);
            
            if (selection >= 0 && selection < genres){
                filter = selection;
                index = 1;
                if (!to_clean){
                    to_clean = true;
                }else{
                    for (size_t i = 1; i <= filteredList.size; ++i) {
                        free(filteredList.songs[i].genre);
                        free(filteredList.songs[i].artist);
                        free(filteredList.songs[i].title);
                        free(filteredList.songs[i].album);
                        free(filteredList.songs[i].trackid);
                    }
                    filteredList.size = 0;
                    filteredList.capacity = 10;
                }
                loadSongsByGenre(&songList, &filteredList, genreslist[filter]);
                printSongList(&filteredList, index);
            }
        }
        else if (strcmp(choice, "r") == 0){
            index = 1;
            filter = -1;
            printSongList(&songList, index);
        }
    }while(!quit);
    

    free(songList.songs);
    free(filteredList.songs);
    return 0;
}

void loadSongsByGenre(SongArrayList *originalList, SongArrayList *filteredList, char *genre){
    for (size_t i = 1; i <= originalList->size; i++){
        if (strcmp(originalList->songs[i].genre, genre) == 0){
            filteredList->size++;
            if (filteredList->size == filteredList->capacity){
                filteredList->capacity *= 2;
                Song *temp = realloc(filteredList->songs, sizeof(Song) * filteredList->capacity);

                if (temp == NULL){
                    fprintf(stderr, "Error in memory reallocation\n");
                    exit(EXIT_FAILURE);
                }
                filteredList->songs = temp;
            }

            filteredList->songs[filteredList->size].genre = strdup(originalList->songs[i].genre);
            filteredList->songs[filteredList->size].artist = strdup(originalList->songs[i].artist);
            filteredList->songs[filteredList->size].title = strdup(originalList->songs[i].title);
            filteredList->songs[filteredList->size].trackid = strdup(originalList->songs[i].trackid);
            filteredList->songs[filteredList->size].popularity = originalList->songs[i].popularity;
        }
    }
}

bool readInSongs(SongArrayList *songList){
    FILE *file = fopen("songs.txt", "r");

    if (file == NULL){
        printf("\n\nERROR OPENING FILE\n\n");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL){

        char field1[256], field2[256], field3[256], field4[256], popu[256];

        if (sscanf(line, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%s", field1, field2, field3, field4, popu) == 5 && strcmp(field1, "genre") != 0 && strcmp(field2, "artist_name") != 0){
            songList->size++;
        }

        if (songList->size == songList->capacity) {
            songList->capacity *= 2;
            Song *temp = realloc(songList->songs, sizeof(Song) * songList->capacity);

            if (temp == NULL) {
                fprintf(stderr, "Error in memory reallocation\n");
                exit(EXIT_FAILURE);
            }

            songList->songs = temp;
        }

        songList->songs[songList->size].genre = strdup(field1);
        songList->songs[songList->size].artist = strdup(field2);
        songList->songs[songList->size].title = strdup(field3);
        songList->songs[songList->size].trackid = strdup(field4);
        songList->songs[songList->size].popularity = (short)(atoi(strdup(popu)));
    }

    fclose(file);

    return true;
}

void printSongList(SongArrayList *songList, int starting_index){
    printf("Song List:\n");
    for (size_t i = starting_index; i < starting_index+10; ++i) {
        if (i <= songList->size){
            printf("\t%d. %s - %s - %s \n", (int)i, songList->songs[i].genre, songList->songs[i].artist, songList->songs[i].title);
        }
    }
}

void generateUniqueGenres(LinkedListNode **ptrUniqueGenresLL, SongArrayList songList){
    for (size_t i = 1; i <= songList.size; i++){
        char *genre = songList.songs[i].genre;

        LinkedListNode *prevNode = NULL;

        LinkedListNode *node = *ptrUniqueGenresLL;
        while (node != NULL){
            if (strcmp(node->data.genre, genre) == 0){
                node->data.count++;
                break;
            }
            prevNode = node;
            node = node->next;
        }

        if (node == NULL){
            LinkedListNode *newNode = malloc(sizeof(LinkedListNode));
            newNode->data.genre = strdup(genre);
            newNode->data.count = 1;
            newNode->next = NULL;


            if (prevNode != NULL) {
                prevNode->next = newNode;
            } else {
                *ptrUniqueGenresLL = newNode;
            }
        }
    }
}

void printGenresAndCounts(LinkedListNode *ptrUniqueGenresLL) {
    while (ptrUniqueGenresLL != NULL) {
        printf("\t%s\t\t%d\n", ptrUniqueGenresLL->data.genre, ptrUniqueGenresLL->data.count);
        ptrUniqueGenresLL = ptrUniqueGenresLL->next;
    }
}

bool isNumber(const char *input) {
    int length = strlen(input);

    for (int i = 0; i < length; i++) {
        if (!isdigit(input[i]) && (i == 0 && input[i] != '-')) {
            return false; 
        }
    }

    
    char *endptr;
    strtol(input, &endptr, 10);

    return (*endptr == '\0') ? true : false; 
}