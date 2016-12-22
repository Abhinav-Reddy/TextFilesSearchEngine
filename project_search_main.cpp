#include "project_sort_files.cpp"
#include "project_get_queries.cpp"


void getfilenames(string *str)
{
    filedir=str[0];
    file_stopwords=str[1];
    file_filenames=str[2];
    mkdir((filedir+"search engine").c_str());
    filedir+="search engine/";
    file_words=filedir+"words.txt";
    file_indexes=filedir+"indexes.txt";  
    file_fileid=filedir+"file_ids.txt"; 
    file_indexpos=filedir+"index_pos.txt";
    file_trigrams=filedir+"trigrams.txt";
    file_idwords=filedir+"id_to_word.txt";
    file_trigrampos=filedir+"trigram_pos.txt"; 
    file_wordlen=filedir+"word_lengths.txt";   
    file_filelen=filedir+"file_lengths.txt";
    temporary_folder=filedir+"temporary folder";
    mkdir(temporary_folder.c_str());  
    temporary_folder+='/';
}




int main()
{
    int i,j;
    string str[3];
    while(1){
        cout<<"enter the choice"<<endl;     
        cin>>i;
        if(i==1)
        {
          cin>>str[0]>>str[1]>>str[2];      
          getfilenames(str);
	        indexing();
	        sort_files();
        }
        else
        {
            if(i==2)            
	             get_queries();
	          else
               break;
        }
    }   
	  system("pause");
	  return 0;
}
