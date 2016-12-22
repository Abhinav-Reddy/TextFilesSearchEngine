#ifndef INC_FILENAME_H
#define INC_FILENAME_H

#include<fstream>
#include<string>
#include<iostream>
using namespace std;

string filedir,file_stopwords,file_filenames,file_words,file_idwords,file_wordlen;
string file_indexpos,file_indexes,temporary_folder,file_fileid,file_trigrams,file_trigrampos,file_filelen;
int temporary_no=1,maxsz=12;


class node_info{
	public:
	int cntr;
	int prev;
	int data;
};


string tostring(int k)
{
	string s;
	int i;
	if(k==0)
		return "0";
	while(k>0)
	{
		s=s+char(k%10+48);
		k=k/10;
	}
	reverse(s.begin(),s.end());
	return s;
}


int toint(string s)
{
    int idx,tmp=0;
    for(idx=0;idx<s.length();idx++)
       tmp=tmp*10+s[idx]-'0';
    return tmp;
}

   
#endif
