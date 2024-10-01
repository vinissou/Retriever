/***********************************************************
* Name: search.h
*
* Function: 
* This file holds all the text search functions 
*
* Observations: 
* [INSERT BMH ALGOL REFS]
* [uses Boyer-Moore-Horspool..]
* All the Boyer-Moore style functions were adapted from
* the examples here: 
* http://www-igm.univ-mlv.fr/~lecroq/string/node1.html
*
* Creator: Vinícius Souza
* Site:    https://github.com/vinissou
***********************************************************/
//With the search term list option the file will do a brute force search
// TODO create case sensitive option
// TODO change the checking to true and false

/* TODO TODO CREATE CALLER FUNCTIONS TO CALL INIT_TERM AND TODO TODO*/
/* TODO TODO THE OTHER FUNCTIONS                           TODO TODO*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 256 //TODO search alphabetsizes >>> review this
#define MAX(a, b) ((a < b) ? b : a) //what is this???



/***********************************************************
* Term control variables: 
* TERM and LENGHT: 
* Carries the the respectives values all around the program
* ,so each funtion won't need to checks the same values 
* multiples times for the same search term.
*
* TERM_CHECK: 
* informs the function if it needs to call InitTerm.
***********************************************************/
// char  *TERM        = NULL; allready declared in app.c??
//long long   TERM.LENGHT = 0;
//short  TERM_CHECK  = 1; //TODO check to see if it is needed
//TERM.CHECK  = 1; //TODO check to see if it is needed


/* static tables used by the Boyer-Moore style functions*/
static int  *good_suffix_ptr = NULL;//pass this to a optional variable
static int   bad_char_table[ALPHABET_SIZE];    


/* prototypes*/
static void BadCharacters(const char *term, const int term_size);
static void GoodSuffix(char *term, int  term_lenght);
long long NextPosition(FILE *file, const char *term);



/***********************************************************
* Name: InitTerm
*
* Function: 
* Initialize all the functions and variables related to the 
* new search term. 
*
* Observations: 
* Prevent calling any function more than one time to the 
* same term to avoid slow down the program.
* TODO rename some of the "term"s, so it can be more 
*      readable
***********************************************************/
void InitTerm(const char *term)
{
    if (term != NULL){
        if (TERM.STR == NULL || strcmp(term , TERM.STR) != 0){
            TERM.LENGHT = strlen(term);
            TERM.STR = realloc(TERM.STR, TERM.LENGHT);
            strcpy(TERM.STR, term);

            BadCharacters(TERM.STR, TERM.LENGHT);
            GoodSuffix(TERM.STR, TERM.LENGHT);

        }
    }
}

//TODO maybe integrate this on a call function
//TODO that will choose the best function for the job???


/***********************************************************
* Name: BadCharacters
*
* Function: 
* Preprocesses the pattern to produce a table containing, 
* for each symbol in the alphabet, the number of characters 
* that can safely be skipped on the Boyer-Moore style
* functions.
***********************************************************/
static void BadCharacters(const char *term, 
                          const int   term_size)
{
    for (int tmp = 0; tmp < ALPHABET_SIZE; tmp++) 
        bad_char_table[tmp] = term_size;
 
    for (int tmp = 0; tmp < term_size - 1; tmp++)
        bad_char_table[(int)term[tmp]] = term_size - tmp - 1;
}


/***********************************************************
* Name: Suffixes 
*
* Function: 
* Recognize the substrings suffixes on the search term to 
* to allow the GoodSuffix function to generate a table with 
* how many characters are safe to jump in the case of a 
* match om the Boyer-Moore style functions.
***********************************************************/
static void Suffixes(const char *term, 
                     const int   term_lenght,
                           int  *suffix_table) 
{
    int f, g, i;
  
    suffix_table[term_lenght - 1] = term_lenght;
    g = term_lenght - 1;
    for (i = term_lenght - 2; i >= 0; --i) {
        if (i > g && suffix_table[i + term_lenght - 1 - f] < i - g)
            suffix_table[i] = suffix_table[i + term_lenght - 1 - f];
        else {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && term[g] == term[g + term_lenght - 1 - f])
                --g;
            suffix_table[i] = f - g;
        }
    }
}


/***********************************************************
* Name: GoodSuffix
*
* Function: 
* Uses the suffix table generated by the Suffixes function 
* to generate a table with how many characters are safe to 
* jump in the case of a match om the Boyer-Moore style 
* functions.
***********************************************************/
static void GoodSuffix(char *term, 
                       int   term_lenght)
{
    int i = 0;
    int j = 0; 
    //int suffix_table[term_lenght];
    //int g_suffix_table[term_lenght];
    int *suffix_table   = malloc(sizeof(int) * term_lenght); 
    int *g_suffix_table = malloc(sizeof(int) * term_lenght);
  
    Suffixes(term, term_lenght, suffix_table);
  
    for (i = 0; i < term_lenght; ++i)
        g_suffix_table[i] = term_lenght;
    for (i = term_lenght - 1; i >= 0; --i)
        if (suffix_table[i] == i + 1)
            for (; j < term_lenght - 1 - i; ++j)
                if (g_suffix_table[j] == term_lenght)
                    g_suffix_table[j] = term_lenght - 1 - i;
    for (i = 0; i <= term_lenght - 2; ++i)
        g_suffix_table[term_lenght - 1 - suffix_table[i]] = term_lenght - 1 - i;
 
    good_suffix_ptr = realloc(good_suffix_ptr, sizeof(g_suffix_table));
    memcpy(good_suffix_ptr, g_suffix_table, (int) sizeof(g_suffix_table));
    //TODO pass this pointer to a optional variable
}



/***********************************************************
* Name: BoyerMooreStr
*
* Function: 
* Implementation of the Boyer-Moore Algorithm for strings.
***********************************************************/
long long BoyerMooreStr(const char *term, 
                    const int   term_lenght, //TODO change this
                    const char *text,
                    const int   text_size,
                          int   position)
{
    int i = 0; //TODO change names
 
    if(TERM.CHECK == 1)
        InitTerm(term);
 
    while (position <= text_size - term_lenght) {
        for (i = term_lenght - 1; i >= 0 && term[i] == text[i + position]; --i);
        if (i < 0) {
            return position;
        }
        else{
            position += MAX(good_suffix_ptr[i] 
                           ,bad_char_table[(int)text[i + position]] 
                           -term_lenght + 1 + i);
        }
    }
 
    return -1;
}




/*
Buffer()
{
    position = 0; 
    buffer_size = 10 * term_lenght;
    buffer[buffer_size]
    buffer[3 * term_lenght];

    fread(buffer, 1, buffer_size,file);
    BoyerMooreStr(term, term_lenght, buffer, 1000);
}
*/
/***********************************************************
* Name: BoyerMooreFile
*
* Function: 
* Implementation of the Boyer-Moore Algorithm that searches 
* directly on the source file. 
***********************************************************/
long long BoyerMooreFile(char *term, 
                    FILE *file,
                    long long  file_size)//add term_lenght again
{
    int i = 0; //
    long long position = ftell(file); 
    char * buffer = NULL;
    char *text = "TEMP";
 
    if(TERM.CHECK == 1)
        InitTerm(term);
 
    while (position <= file_size - TERM.LENGHT) {
        fseek(file, position, SEEK_SET);
        fread(buffer, 1, (TERM.LENGHT -1), file); 
        for (i = TERM.LENGHT - 1
            ;i >= 0 && term[i] == text[i + position]; --i); //TODO fix this
        if (i < 0) {
            /* MATCH_COUNT++; //TODO tirar daqui*/
            fseek(file, position, SEEK_SET);
            return position;
        }
        else
            position += MAX(good_suffix_ptr[i] 
                           ,bad_char_table[(int)text[i + position]] //TODO fix this
                           -TERM.LENGHT + 1 + i);
    }
 
    return -1;
}


/***********************************************************
* Name: HorspoolStr
*
* Function: 
* Implementation of the Boyer-Moore-Horspool Algorithm for
* strings.
***********************************************************/
long long HorspoolStr(const char *term, 
                 const int   term_lenght, 
                 const char *text, 
                 const int   text_size,
                       int   position)
{
  
   if(TERM.CHECK == 1)
       InitTerm(term);

   while (position <= text_size - term_lenght) {
       char current_char = text[position + term_lenght - 1];
       if (term[term_lenght - 1] == current_char && 
           memcmp(term, text + position, term_lenght - 1) == 0){
           return position;
       }

       position += bad_char_table[(int)current_char];
   }

   return -1;
}


/***********************************************************
* Name: HorspoolFile
*
* Function: 
* Implementation of the Boyer-Moore-Horspool Algorithm that 
* searches directly on the source file.
*
* Observations: 
* This function still in experimental state still have 
* inferior performance than BruteForce.
***********************************************************/
long long HorspoolFile(FILE *file, 
            const long long  file_size,
            const char *term, 
            const long long  term_lenght)
{
    long long position = ftell(file);
    char current_char = 0; //char can't be bellow zero
                           //change it to unsigned ???
    static char *buffer = NULL;//TODO search about this
    
    if(TERM.CHECK == 1)
        InitTerm(term);
 
    buffer = malloc(term_lenght);
 
    while (position <= file_size - term_lenght) {
        fseek(file, ((position + term_lenght) - 1), SEEK_SET);
        current_char = fgetc(file);
        

        //open watcom says this will always result in zero, 
        //search the reference
        if (current_char < 0)      //explain this on the header
            current_char = (256 - current_char);
 
        if (term[term_lenght - 1] == current_char){
            fseek(file, position, SEEK_SET);
            fread(buffer, 1, (term_lenght -1), file); 
 
            if (memcmp(term, buffer, term_lenght - 1) == 0){
                /* MATCH_COUNT++;//TODO tirar daqui*/
                //PxSITION = position;//TODO change all this
                return position;
            }
        }
      
        position += bad_char_table[(int)current_char];
    }
 
    return -1;
}



/***********************************************************
* Name: GetPositionChar TODO eliminate this???
*
* Function: 
* Returns the character in the specified position and rewind
* the file to the initial position.
***********************************************************/
char GetPositionChar(FILE *file, 
                     const long long position)
{
    const long long init_position = ftell(file);
          char position_char = 0;

    fseek(file, position, SEEK_SET);
    position_char = fgetc(file);
    fseek(file, init_position, SEEK_SET);

    return position_char;
}



/* TODO FindNextFile and FindNextStr????*/
//TODO: FindIgnoreCase
//TODO apply the 32 rule to see if works
//TODO >>> rename this to bruteforce???
/***********************************************************
* Name: FindNext
*
* Function: 
* Finds the next occurrence of the search term in the file, 
* if didn't find any it returns -1
*
* Observations: 
*
* Used functions and variables from other files:
***********************************************************/
long long FindNext(FILE *file, 
        const char *term)
{
    const long long term_size     = strlen(term);//TODO CHANGE THIS TO A 
          long long position      = ftell(file); //TODO CALL VARIABLE
          char current_char  = 0;          

    if(TERM.CHECK == 1)
       InitTerm(term);

    while(position < SOURCE.SIZE){ //TODO take all the global variables
        current_char = getc(file); //out of the functions
        if(term[0] == current_char){
            int match = 1; //TODO change it to long???
            for(int tmp = 1; tmp < term_size; tmp++){
                if(term[tmp] == getc(file))
                    match++;
                else{
                    fseek(file, position, SEEK_SET);
                    break;
                }
            }
            if(match == term_size){
                /* MATCH_COUNT++;*/
                fseek(file, position, SEEK_SET);
                return position;
            }
        }
        position++; 

    }

    return -1;
}

/*
long long FindNextStr()
{ 
    char * strchr (       char * str, int character );
    int main ()
    {
      char str[] = "This is a sample string";
      char * pch;
      printf ("Looking for the 's' character in \"%s\"...\n",str);
      pch=strchr(str,'s');
      while (pch!=NULL)
      {
        printf ("found at %d\n",pch-str+1);
        pch=strchr(pch+1,'s');
      }
      return 0;
    }
}
*/


/***********************************************************
* Name: FindPrevious
*
* Function: 
* Find the previous occurrence of a search term in the file, 
* if none occurrence were found it returns -1.
*
* Used functions and variables from other files:
* file.h: RewindPosition 
***********************************************************/
long long FindPrevious(FILE *file,
            const char *term)
{
    const long long init_position    = ftell(file);
          long long current_position = init_position;
          long long next_position    = 0;
          int  term_lenght      = strlen(term);
   
    while(current_position > 0){
        current_position = ftell(file);
        next_position = NextPosition(file, term);

        if(next_position >= init_position){
            RewindFile(file, term_lenght);
            continue;
        }
        if(next_position < init_position) //integrar com MATCH???
            break;
    }

    return next_position;
}


/***********************************************************
* Name: FindFirst
*
* Function: 
* Find the first occurrence of a term in the file.
***********************************************************/
long long FindFirst(FILE *file, 
         const char *term)
{   
    long long first_position = 0;

    rewind(file);
    first_position = FindNext(file, term);

    return first_position;
}


/***********************************************************
* Name: FindLast
*
* Function: 
* Find the last occurrence of a term in the file.
***********************************************************/
long long FindLast(FILE *file, 
        const char *term)
{
    fseek(file, 0, SEEK_END);
    const long long current_position = FindPrevious(file, term);

    return current_position;
}


/***********************************************************
* Name: NextPosition
*
* Function: 
* Returns the position of the next occurrence of term on the
* file and reset the current position to initial position.
***********************************************************/
long long NextPosition(FILE *file, 
            const char *term)
{
    const long long current_position = ftell(file);
    const long long next_position    = FindNext(file, term);
    
    fseek(file, current_position, SEEK_SET);
    return next_position;
}


/***********************************************************
* Name: PreviousPosition
*
* Function: 
* Returns the position of the previous occurrence of the 
* term on the file and reset the current position to 
* initial position.
***********************************************************/
long long PreviousPosition(FILE *file, 
                const char *term)
{
    const long long current_position  = ftell(file);
    const long long previous_position = FindPrevious(file, term);

    fseek(file, current_position, SEEK_SET);
    return previous_position;
}


/***********************************************************
* Name: FirstPosition
*
* Function: 
* Returns the position of the first occurrence of a term in 
* the file and reset the current position to initial 
* position.
***********************************************************/
long long FirstPosition(FILE *file,
             const char *term)
{
    const long long current_position = ftell(file);
    const long long first_position   = FindFirst(file, term);
    
    fseek(file, current_position, SEEK_SET);
    return first_position;
}


/***********************************************************
* Name: LastPosition
*
* Function: 
* Returns the position of the last occurrence of a term in 
* the file and reset the current position to initial 
* position.
***********************************************************/
long long LastPosition(FILE *file,
            const char *term)
{
    const long long current_position = ftell(file);
    const long long last_position    = FindLast(file, term);

    fseek(file, current_position, SEEK_SET);
    return last_position;
}


/***********************************************************
* Name: ReturnPositionRange
*
* Function: 
* Return a string with the characters of the selected 
* position range and return to the initial position
*
* Observations: 
* it adds one number to the range before send it to  fread, 
* otherwise it trims the last character
* TODO >>> move all theses "return" functions to copy.h
***********************************************************/
char *ReturnPositionRange(FILE *file, 
                          const long long begin_position,
                          const long long end_position)
{
    const  long long  current_position = ftell(file);
    const  long long  range_size       = ((end_position - begin_position) + 1); 
    static char *buffer           = NULL;
    
    buffer = malloc(range_size);
    fseek(file, begin_position, SEEK_SET);
    fread(buffer, 1, range_size , file); 
    fseek(file, current_position, SEEK_SET);
    
    return buffer;
}


/***********************************************************
* Name: ReturnLimitersRange
*
* Function: 
* Returns a string with the characters present between the 
* selected limiters
*
* Observations: 
* begin and end positions are initialized on the
* current position to prevent ReturnPositionRange to
* return the file content since the beginning or the 
* the end position be smaller than the begin
***********************************************************/
char *ReturnLimitersRange(FILE *file, 
                          const long long position, 
                          const char *begin_limiter,  
                          const char *end_limiter)
{
    const long long current_position = ftell(file);
          long long begin_position   = current_position;
          long long end_position     = current_position;

    fseek(file, position, SEEK_SET);
    begin_position = FindPrevious(file, begin_limiter);
    AdvanceFile(file, 1);
    end_position   = FindNext(file, end_limiter);
    
    if(begin_position == position &&
       end_position   == (position + 1))
        return NULL;
    
    char *buffer = ReturnPositionRange(file, 
                                       begin_position, 
                                       end_position);
    fseek(file, current_position, SEEK_SET);
    
    return buffer;
}


/***********************************************************
* Name: ReturnLine
*
* Function: 
* Return a whole line trimming the newline character and 
* returning to the original position 
*
* Observations: 
* if the function don't find a new line characters it puts 
* the defined amount of characters of STANDART_ROWS before 
* or after each positions.
*
* Used functions and variables from other files:
*
***********************************************************/
char *ReturnLine(FILE *file, 
                 const long long position)
{
    const long long current_position = ftell(file);
          long long begin_position   = 0;
          long long end_position     = 0;
    
    fseek(file, position, SEEK_SET);
    begin_position = FindPrevious(file, "\n");
    AdvanceFile(file, 1);
    end_position = FindNext(file, "\n");
    
    //ask for confirmation when this happen???
    if(begin_position < 0 && end_position < 0){
        begin_position = RewindPosition(begin_position, STANDART_ROWS);
        end_position   = (end_position + STANDART_ROWS); //TODO bring back
    }                                                    //AdvancePosition???   

    else if(begin_position < 0 && end_position > position){
        begin_position = 0;
        end_position   = RewindPosition(end_position, 1);
    }

    else if(begin_position >= 0 && end_position < 0){
        begin_position = (begin_position + 1);
        end_position   = (SOURCE.SIZE - 1);
    }

    else{
        begin_position = (begin_position + 1);
        end_position   = RewindPosition(end_position, 1);
    }
    
    char *buffer = ReturnPositionRange(file, 
                                       begin_position, 
                                       end_position);
    fseek(file, current_position, SEEK_SET);
    
    
    return buffer;
}
