#include "project_stemmer.cpp"
#include "trie.cpp"

void getstopwords()
{
	int i,j;
	string s;
	stop_node=new trie_node(26);
	fstream input;
	node_info tmp;
	input.open(file_stopwords.c_str());
	while(input>>s)
		tmp=process_string(s,stop_node);
	input.close();	
}


string filter(string s)
{
	int i,j=s.length(),k=0;
	string tmp;
	tmp=s;
	for(i=0;i<j;i++)
		if(isalnum(s[i]))
			tmp[k++]=s[i];
		else
		{
			if(!(s[i]=='=' || s[i]=='.' || s[i]==','))
				tmp[k++]=' ';
		}	
	return tmp.substr(0,k);	
}


int is_stop_word(string s)
{
	int i,j;
	if(search_trie(s,stop_node))
		return 0;
	return 1;	
}


int contains_both(string s)
{
	int i,j=s.length();
	for(i=0;i<j;i++)
		if(!isalpha(s[i]))
			return 0;
	return 1;		
}


void indexing()
{	
	string s,tmp,filename;
	int i,j,k,n,p=0,pq=1,w_cnt,len;
	char ch[200];
	string zero="00000000";
	zero+=zero;
	node_info tmp_info;
	alpha_root=new trie_node(26);
	ifstream fileinput;
  ofstream out,index_out,fileid,id_out,out_len,out_filelen;
	getstopwords();
	fileinput.open(file_filenames.c_str());
	out.open(file_words.c_str());
	fileid.open(file_fileid.c_str());
	index_out.open(file_indexes.c_str());
	id_out.open(file_idwords.c_str());
	out_len.open(file_wordlen.c_str());
	out_filelen.open(file_filelen.c_str());
  if(!index_out.is_open() || !out.is_open())
	{
     cout<<"failed"<<endl;
     exit(0);
  }
	fcnt=0;
	while(getline(fileinput,filename) && fcnt<100000)
	{
    ifstream in;
		in.open(filename.c_str());
		if(!in.is_open())
		{
         ;
    }
		else
		{	
      w_cnt=0;
			fcnt++;
			s="";
			while(in>>s )
			{            
				for(i=s.length()-1;i>=0;i--)
					s[i]=tolower(s[i]);	
				s=filter(s);
				n=s.length();
				i=0;
				while(i<n)
				{
					while(i<n && s[i]==' ')
						i++;
					j=i;	
					while(j<n && s[j]!=' ')
						j++;
					tmp=s.substr(i,j-i);
					i=j+1;
					k=contains_both(tmp);	
					if(k==0)
						;
					else
					{			
						if(tmp.length()<30 && tmp.length()>2 && is_stop_word(tmp))
						{
              w_cnt++;                 
							for(j=0;j<tmp.length();j++)
								ch[j]=tmp[j];
							if(tmp.length()>3)
								tmp=tmp.substr(0,stemword(ch,j));
							tmp_info=process_string(tmp,alpha_root);
							if(tmp_info.data==0)
							{
								out<<tmp<<" "<<cnt_no<<'\n';
								id_out<<cnt_no<<" "<<tmp<<'\n';
								cnt_no++;
								len=tmp.length();
								if(len<10)
								  out_len<<"0";
								out_len<<len<<" ";  
							}
							else
							{
								if(tmp_info.prev!=fcnt)
									index_out<<tmp_info.data<<" "<<tmp_info.prev<<" "<<tmp_info.cntr<<'\n';
							}
						}
					}
					tmp="";	
				}
			}
			in.close();
		  fileid<<fcnt<<" "<<filename<<'\n';
		  s=tostring(w_cnt);
		  s=zero.substr(0,maxsz-s.length())+s+" ";
      out_filelen<<s;
    }
    filename="";
	}
	index_out.close();
	out.close();
	fileinput.close();
	fileid.close();
	id_out.close();
	out_len.close();
	out_filelen.close();
  transverse_trie(alpha_root);	
}
