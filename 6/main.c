/*
 * ============================================================================
 *
 *         Author:  Jordan Bayles (baylesj), baylesj@onid.orst.edu
 *        Company:  Oregon State University
 *        Created:  11/24/2012 01:17:06 AM
 *
 *    Environment:  vim + gdb + valgrind, gcc compiler on Arch Linux
 *    Description:  Main file of program, adds elements in an input file
 *                  to an implementation of a hash map.
 *
 * ============================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashMap.h"

/*
 the getWord function takes a FILE pointer and returns you a string which was
 the next word in the in the file. words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.

 when there are no more words in the input file this function will return NULL.

 this function will malloc some memory for the char* it returns. it is your job
 to free this memory when you no longer need it.
 */
char* getWord(FILE *file);

int main (int argc, const char * argv[]) {
    const char* filename;
    char *word;
    FILE *input;
    struct hashMap hashTable;
    int tableSize = 1000;
    clock_t timer;

    /*
     this part is using command line arguments, you can use them if you wish
     but it is not required. DO NOT remove this code though, we will use it for
     testing your program.

     if you wish not to use command line arguments manually type in your
     filename and path in the else case.
     */
    if(argc == 2)
        filename = argv[1];
    else
        filename = "input.txt"; /*specify your input text file here*/

    printf("opening file: %s\n", filename);
    input = fopen( filename, "r" );
    timer = clock();

    initMap(&hashTable, tableSize);

    /* Loop through entries */
    while( ( word = getWord( input ) ) )
    {
        /* If already in hash table, increment number of occurrences */
        if( containsKey( &hashTable, word ) )
        {
            ValueType *val = atMap( &hashTable, word );
            assert( val != NULL );
            ( *val )++;
            /* Not adding word to hash table, so free it here */
            free( word );
        }
        /* Else insert with occurrence count of 1 */
        else
        {
        insertMap( &hashTable, word, 1 );
        }
    }

    /* Print hash map */
    for( int i = 0; i < hashTable.tableSize; ++i )
    {
        struct hashLink *lnk = &hashTable.table[ i ][ 0 ];

        while( lnk != NULL )
        {
            printf( "%d, %d, %s\n", i, lnk->value, lnk->key );
            lnk = lnk->next;
        }
    }

    timer = clock() - timer;
    printf("concordance ran in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);

    printf("closing file...\n");
    fclose( input );
    freeMap( &hashTable );
    return 0;
}

char* getWord(FILE *file)
{
    int length = 0;
    int maxLength = 16;
    char character;

    char* word = (char*)malloc(sizeof(char) * maxLength);
    assert(word != NULL);

    while( (character = fgetc(file)) != EOF)
    {
        if((length+1) > maxLength)
        {
            maxLength *= 2;
            word = (char*)realloc(word, maxLength);
        }
        if((character >= '0' && character <= '9') || /*is a number*/
           (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
           (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
           character == 39) /*or is an apostrophy*/
        {
            word[length] = character;
            length++;
        }
        else if(length > 0)
            break;
    }

    if(length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}
