#include<vector>
#include "project_search_header.h"
#include<algorithm>
#include<set>
#include<direct.h>
#include<stdio.h>

void sort_words()
{
  ifstream in;
  ofstream out;   
	vector< pair<string,int> >v;
	int idx,tmp;
	string s="";
	in.open(file_words.c_str());
	while(in>>s)
	{
		in>>idx;
		v.push_back(make_pair(s,idx));
		s="";
	}
	in.close();
	sort(v.begin(),v.end());
	out.open(file_words.c_str());
	for(idx=0;idx<v.size();idx++)
		out<<v[idx].first<<" "<<v[idx].second<<'\n';
	v.clear();	
	out.close();
}


bool cmp1(node_info a,node_info b)
{
	if(a.data!=b.data)
		return (a.data<b.data);
	if(a.prev!=b.prev)	
		return (a.prev<b.prev);
	return 1;	
}


void sort_temporary_files(string s)
{
	char ch[100];
	int idx,i,j;
	node_info tmp;
	vector< node_info > v;
	ofstream out;
	ifstream in;
	s=temporary_folder+s+".txt";	
	in.open(s.c_str());
  tmp.data=0;
  while(in>>tmp.data)
	{
		in>>tmp.prev>>tmp.cntr;
		v.push_back(tmp);
	}
	in.close();
	sort(v.begin(),v.end(),cmp1);
	out.open(s.c_str());
	for(i=0;i<v.size();i++)
		out<<v[i].data<<" "<<v[i].prev<<" "<<v[i].cntr<<'\n';
	out.close();	
}


void split_indexes(string name)
{
  ifstream in;   
	int a,b,c,idx,i,j;
	char ch[111];
	string s;
	in.open(name.c_str());
	ofstream out[111];
	for(i=0;i<111;i++)
	{
		s=tostring(i);
		s=temporary_folder+s+".txt";
		out[i].open(s.c_str());
	}
	i=0,temporary_no=1;
	while(in>>a)
	{
		in>>b>>c;
		i++;
		out[temporary_no-1]<<a<<" "<<b<<" "<<c<<'\n';
		temporary_no+=(i%2000000==0);
	}		
	for(i=0;i<111;i++)
		out[i].close();
	in.close();		
}


node_info tmp_arr_info[98];


struct cmp{
       
       bool operator() (int a,int b)
       {
            if(tmp_arr_info[a].data!=tmp_arr_info[b].data)
                 return (tmp_arr_info[a].data<tmp_arr_info[b].data);
            if(tmp_arr_info[a].prev!=tmp_arr_info[b].prev)	
                 return (tmp_arr_info[a].prev<tmp_arr_info[b].prev);
            return 1;
       }	
};


void  merge_temporary_files(string name,string pos_name,int fg)
{
     ifstream in[temporary_no],input;
     ofstream indexpos;
     set< int,cmp>st;
     set<int>::iterator it;
     int i,j,idx;
     int sz;
     string s,tmp;
     streampos pos;
     tmp="000000000000";
     tmp+=tmp;
     char ch[100];
     indexpos.open(pos_name.c_str());
     if(fg)
     {
     for(i=0;i<temporary_no;i++)
     {
          s=temporary_folder+tostring(i)+".txt";
          in[i].open(s.c_str());    
          in[i]>>tmp_arr_info[i].data>>tmp_arr_info[i].prev>>tmp_arr_info[i].cntr;
          st.insert(i);
     }
     ofstream out;
     out.open(name.c_str());
     idx=1;
     while(st.size()>0)
     {
          it=st.begin();
          i=*it;
          st.erase(it);                            
          out<<tmp_arr_info[i].data<<" "<<tmp_arr_info[i].prev<<" "<<tmp_arr_info[i].cntr<<'\n';       
          if(in[i]>>tmp_arr_info[i].data)
          {
                in[i]>>tmp_arr_info[i].prev;
                in[i]>>tmp_arr_info[i].cntr;
                st.insert(i);
          }         
     }
     for(i=0;i<temporary_no;i++)
       in[i].close();
     for(i=0;i<111;i++)
          remove((temporary_folder+tostring(i)+".txt").c_str());  
     out.close(); 
     }
     input.open(name.c_str(),ios::binary); 
     pos=input.tellg();
     input.seekg(0,ios::end);
     sz=input.tellg()-pos;
     input.seekg(0,ios::beg);
     node_info info;
     i=-1,idx=1;
     while(input>>info.data)
     {
          while(idx<info.data)                  
          {
              s=tostring(sz);
              s=tmp.substr(0,maxsz-s.length())+s+" ";
              indexpos<<s;
              idx++;
          }                                  
          if(idx==info.data)
          {
                if(idx==1825)
                  cout<<i+1<<endl;                      
                idx=info.data;
                s=tostring(i+1);
                s=tmp.substr(0,maxsz-s.length())+s+" ";
                indexpos<<s;
                idx++;
          }
          input>>info.prev>>info.cntr;
          i=input.tellg()-pos;
     }    
     s=tostring(sz);
     s=tmp.substr(0,maxsz-s.length())+s+" ";
     while(idx<=18278)
     {
         indexpos<<s;
         idx++;
     }                                           
     indexpos.close();
     input.close();
}


void generate_trigrams()
{
     int idx,i,j,k;
     vector< int > v;
     ifstream in;
     string s;
     ofstream out;
     in.open(file_idwords.c_str());
     out.open(file_trigrams.c_str());
     while(in>>idx)
     {
          in>>s;
          if(s.length()<3)
             continue;
          v.clear();
          for(i=0;i<s.length()-2;i++)
          {
              for(j=i,k=0;j<i+3;j++)
                 k=k*26+s[j]-'a'+1;
              v.push_back(k);  
          }
          sort(v.begin(),v.end());
          for(i=0;i<v.size();)
          {
                j=i+1;
                while(j<v.size() && v[i]==v[j])
                   j++;
                out<<v[i]<<" "<<idx<<" "<<j-i<<'\n';
                i=j;
          }                          
     }
     in.close();
     out.close();
}
     
                  
void sort_files()
{
	int i;
	sort_words();
	split_indexes(file_indexes);
	for(i=0;i<temporary_no;i++)
		sort_temporary_files(tostring(i));
	merge_temporary_files(file_indexes,file_indexpos,1);
  generate_trigrams(); 
  split_indexes(file_trigrams);	
  for(i=0;i<temporary_no;i++)
		sort_temporary_files(tostring(i));	
  merge_temporary_files(file_trigrams,file_trigrampos,1);	
}

