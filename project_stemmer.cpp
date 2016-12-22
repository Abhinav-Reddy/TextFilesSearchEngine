#include <string.h>  /* for memmove */
using namespace std;

#define TRUE 1
#define FALSE 0

/* The main part of the stemming algorithm starts here. b is a buffer
   holding a word to be stemmed. The letters are in b[k0], b[k0+1] ...
   ending at b[k]. In fact k0 = 0 in this demo program. k is readjusted
   downwards as the stemming progresses. Zero termination is not in fact
   used in the algorithm.

   Note that only lower case sequences are stemmed. Forcing to lower case
   should be done before stem(...) is called.
*/

static char * word;       /* buffer for word to be stemmed */
static int final_len,zero,offset;     /* j is a general offset into the string */

/* cons(i) is TRUE <=> b[i] is a consonant. */

static int cons(int i)
{  
   switch (word[i])
   {  case 'a': case 'e': case 'i': case 'o': case 'u': return FALSE;
      case 'y': return (i==zero) ? TRUE : !cons(i-1);
      default: return TRUE;
   }
}

/* m() measures the number of consonant sequences between zero and j. if c is
   a consonant sequence and v a vowel sequence, and <..> indicates arbitrary
   presence,

      <c><v>       gives 0
      <c>vc<v>     gives 1
      <c>vcvc<v>   gives 2
      <c>vcvcvc<v> gives 3
      ....
*/

static int m()
{  
   int n = 0;
   int i = zero;
   while(TRUE)
   {  if (i > offset) return n;
      if (! cons(i)) break; i++;
   }
   i++;
   while(TRUE)
   {  while(TRUE)
      {  if (i > offset) return n;
            if (cons(i)) break;
            i++;
      }
      i++;
      n++;
      while(TRUE)
      {  if (i > offset) return n;
         if (! cons(i)) break;
         i++;
      }
      i++;
   }
}

/* vowelinstem() is TRUE <=> zero,...j contains a vowel */

static int vowelinstem()
{  
   int i; for (i = zero; i <= offset; i++) if (! cons(i)) return TRUE;
   return FALSE;
}

/* doublec(j) is TRUE <=> j,(j-1) contain a double consonant. */

static int doublec(int offset)
{  
   if (offset < zero+1) return FALSE;
   if (word[offset] != word[offset-1]) return FALSE;
   return cons(offset);
}

/* cvc(i) is TRUE <=> i-2,i-1,i has the form consonant - vowel - consonant
   and also if the second c is not w,x or y. this is used when trying to
   restore an e at the end of a short word. e.g.

      cav(e), lov(e), hop(e), crim(e), but
      snow, box, tray.

*/

static int cvc(int i)
{ 
   if (i < zero+2 || !cons(i) || cons(i-1) || !cons(i-2)) return FALSE;
   {  int ch = word[i];
      if (ch == 'w' || ch == 'x' || ch == 'y') return FALSE;
   }
   return TRUE;
}

/* ends(s) is TRUE <=> zero,...final_len ends with the string s. */

static int ends(char * s)
{ 
   int length = s[0];
   if (s[length] != word[final_len]) return FALSE; /* tiny speed-up */
   if (length > final_len-zero+1) return FALSE;
   if (memcmp(word+final_len-length+1,s+1,length) != 0) return FALSE;
   offset = final_len-length;
   return TRUE;
}

/* setto(s) sets (j+1),...final_len to the characters in the string s, readjusting
   final_len. */

static void setto(char * s)
{
  int length = s[0];
   memmove(word+offset+1,s+1,length);
   final_len = offset+length;
}

/* r(s) is used further down. */

static void r(char * s) { if (m() > 0) setto(s); }

/* step1ab() gets rid of plurals and -ed or -ing. e.g.

       caresses  ->  caress
       ponies    ->  poni
       ties      ->  ti
       caress    ->  caress
       cats      ->  cat

       feed      ->  feed
       agreed    ->  agree
       disabled  ->  disable

       matting   ->  mat
       mating    ->  mate
       meeting   ->  meet
       milling   ->  mill
       messing   ->  mess

       meetings  ->  meet

*/

static void step1ab()
{
  if (word[final_len] == 's')
   {  if (ends("\04" "sses")) final_len -= 2; else
      if (ends("\03" "ies")) setto("\01" "i"); else
      if (word[final_len-1] != 's') final_len--;
   }
   if (ends("\03" "eed")) { if (m() > 0) final_len--; } else
   if ((ends("\02" "ed") || ends("\03" "ing")) && vowelinstem())
   {  final_len = offset;
      if (ends("\02" "at")) setto("\03" "ate"); else
      if (ends("\02" "bl")) setto("\03" "ble"); else
      if (ends("\02" "iz")) setto("\03" "ize"); else
      if (doublec(final_len))
      {  final_len--;
         {  int ch = word[final_len];
            if (ch == 'l' || ch == 's' || ch == 'z') final_len++;
         }
      }
      else if (m() == 1 && cvc(final_len)) setto("\01" "e");
   }
}

/* step1c() turns terminal y to i when there is another vowel in the stem. */

static void step1c() { if (ends("\01" "y") && vowelinstem()) word[final_len] = 'i'; }


/* step2() maps double suffices to single ones. so -ization ( = -ize plus
   -ation) maps to -ize etc. note that the string before the suffix must give
   m() > 0. */

static void step2() { switch (word[final_len-1])
{
    case 'a': if (ends("\07" "ational")) { r("\03" "ate"); break; }
              if (ends("\06" "tional")) { r("\04" "tion"); break; }
              break;
    case 'c': if (ends("\04" "enci")) { r("\04" "ence"); break; }
              if (ends("\04" "anci")) { r("\04" "ance"); break; }
              break;
    case 'e': if (ends("\04" "izer")) { r("\03" "ize"); break; }
              break;
    case 'l': if (ends("\03" "bli")) { r("\03" "ble"); break; } /*-DEPARTURE-*/

 /* To match the published algorithm, replace this line with
    case 'l': if (ends("\04" "abli")) { r("\04" "able"); break; } */

              if (ends("\04" "alli")) { r("\02" "al"); break; }
              if (ends("\05" "entli")) { r("\03" "ent"); break; }
              if (ends("\03" "eli")) { r("\01" "e"); break; }
              if (ends("\05" "ousli")) { r("\03" "ous"); break; }
              break;
    case 'o': if (ends("\07" "ization")) { r("\03" "ize"); break; }
              if (ends("\05" "ation")) { r("\03" "ate"); break; }
              if (ends("\04" "ator")) { r("\03" "ate"); break; }
              break;
    case 's': if (ends("\05" "alism")) { r("\02" "al"); break; }
              if (ends("\07" "iveness")) { r("\03" "ive"); break; }
              if (ends("\07" "fulness")) { r("\03" "ful"); break; }
              if (ends("\07" "ousness")) { r("\03" "ous"); break; }
              break;
    case 't': if (ends("\05" "aliti")) { r("\02" "al"); break; }
              if (ends("\05" "iviti")) { r("\03" "ive"); break; }
              if (ends("\06" "biliti")) { r("\03" "ble"); break; }
              break;
    case 'g': if (ends("\04" "logi")) { r("\03" "log"); break; } /*-DEPARTURE-*/

 /* To match the published algorithm, delete this line */

} }

/* step3() deals with -ic-, -full, -ness etc. similar strategy to step2. */

static void step3() { switch (word[final_len])
{
    case 'e': if (ends("\05" "icate")) { r("\02" "ic"); break; }
              if (ends("\05" "ative")) { r("\00" ""); break; }
              if (ends("\05" "alize")) { r("\02" "al"); break; }
              break;
    case 'i': if (ends("\05" "iciti")) { r("\02" "ic"); break; }
              break;
    case 'l': if (ends("\04" "ical")) { r("\02" "ic"); break; }
              if (ends("\03" "ful")) { r("\00" ""); break; }
              break;
    case 's': if (ends("\04" "ness")) { r("\00" ""); break; }
              break;
} }

/* step4() takes off -ant, -ence etc., in context <c>vcvc<v>. */

static void step4()
{
  switch (word[final_len-1])
    {  case 'a': if (ends("\02" "al")) break; return;
       case 'c': if (ends("\04" "ance")) break;
                 if (ends("\04" "ence")) break; return;
       case 'e': if (ends("\02" "er")) break; return;
       case 'i': if (ends("\02" "ic")) break; return;
       case 'l': if (ends("\04" "able")) break;
                 if (ends("\04" "ible")) break; return;
       case 'n': if (ends("\03" "ant")) break;
                 if (ends("\05" "ement")) break;
                 if (ends("\04" "ment")) break;
                 if (ends("\03" "ent")) break; return;
       case 'o': if (ends("\03" "ion") && offset >= 0 && (word[offset] == 's' || word[offset] == 't')) break;
                 if (ends("\02" "ou")) break; return;
                 /* takes care of -ous */
       case 's': if (ends("\03" "ism")) break; return;
       case 't': if (ends("\03" "ate")) break;
                 if (ends("\03" "iti")) break; return;
       case 'u': if (ends("\03" "ous")) break; return;
       case 'v': if (ends("\03" "ive")) break; return;
       case 'z': if (ends("\03" "ize")) break; return;
       default: return;
    }
    if (m() > 1) final_len = offset;
}

/* step5() removes a final -e if m() > 1, and changes -ll to -l if
   m() > 1. */

static void step5()
{
  offset = final_len;
   if (word[final_len] == 'e')
   {  int a = m();
      if (a > 1 || a == 1 && !cvc(final_len-1)) final_len--;
   }
   if (word[final_len] == 'l' && doublec(final_len) && m() > 1) final_len--;
}

/* In stem(p,i,j), p is a char pointer, and the string to be stemmed is from
   p[i] to p[j] inclusive. Typically i is zero and j is the offset to the last
   character of a string, (p[j+1] == '\0'). The stemmer adjusts the
   characters p[i] ... p[j] and returns the new end-point of the string, final_len.
   Stemming never increases word length, so i <= final_len <= j. To turn the stemmer
   into a module, declare 'stem' as extern, and delete the remainder of this
   file.
*/

int stem(char * p, int i, int offset)
{
  word = p; final_len = offset; zero = i; /* copy the parameters into statics */
   if (final_len <= zero+1) return final_len; /*-DEPARTURE-*/

   /* With this line, strings of length 1 or 2 don't go through the
      stemming process, although no mention is made of this in the
      published algorithm. Remove the line to match the published
      algorithm. */

   step1ab(); step1c(); step2(); step3(); step4(); step5();
   return final_len;
}

/*--------------------stemmer definition ends here------------------------*/

#include <stdio.h>
#include <stdlib.h>      /* for malloc, free */
#include <ctype.h>       /* for isupper, islower, tolower */

static char * orig_word;         /* a char * (=string) pointer; passed into word above */

#define INC 100           /* size units in which orig_word is increased */
static int i_max = INC;  /* maximum offset in orig_word */


int stemword(char *s,int len)
{
	orig_word= (char *) malloc(i_max+1);
	int i;
	for(i=0;i<len;i++)
		orig_word[i]=s[i];		
    i=stem(orig_word,0,i-1)+1;
    free(orig_word);
    return i;
}

