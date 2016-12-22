 Static search engine is built by using 
   ->  vector space model for ranking of the text documents
   ->  inverted index for indexing the text documents
   ->  tri-grams for spell checking.
 Its a static search engine so all the text files are to be given before hand and once created,
 no more files can be added.

 
HOW TO USE:
1) download the files.
2) Run the "project_search_engine_main.cpp" file.
3) Initially you are asked to enter a number
   -> zero for exit.
   -> one for building search engine.
   -> two for searching text documents.
4) To build a search engine enter three strings
          -> directory which can be used for storing indexes and other temporary files.
          -> name of text file with location's of all the text documents which are to be indexed.
          -> name of text file with stop words.


LIMITATIONS:
 ->  If adding files is really necessary then search engine has to built from scratch.
