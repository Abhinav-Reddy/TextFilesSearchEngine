#include "project_search_header.h"


class trie_node{
	public:
	node_info info;	
	trie_node **arr;
	trie_node(int k)
	{
		arr=new trie_node *[k];
		for(int i=0;i<k;i++)
			arr[i]=NULL;
		info.cntr=0;
		info.prev=0;
		info.data=0;	
	} 
};

int cnt_no=1,fcnt=1;
trie_node *stop_node;
trie_node *alpha_root;


node_info process_string(string process_s,trie_node *root)
{
	int idx=0,len=process_s.length();
	trie_node *tmp;
	while(idx<len)
	{
		if(root->arr[process_s[idx]-'a']==NULL)
		{
			tmp=new trie_node(26);
			root->arr[process_s[idx]-'a']=tmp;
			root=tmp;
		}		
		else
			root=root->arr[process_s[idx]-'a'];
		idx++;	
	}
	node_info tmp_info=root->info;
	if(root->info.prev==fcnt)
		root->info.cntr++;
	else
	{
		if(root->info.data==0)
			root->info.data=cnt_no;
		root->info.prev=fcnt;
		root->info.cntr=1;
	}
	return tmp_info;
}


void transverse_trie(trie_node *root)
{
  ofstream index_out,indexpos;  
  index_out.open(file_indexes.c_str(),std::fstream::app);
 	int idx;
	if(root->info.data>0) 
		index_out<<root->info.data<<" "<<root->info.prev<<" "<<root->info.cntr<<'\n';
  index_out.close();
  for(idx=0;idx<26;idx++)
		if(root->arr[idx])
			transverse_trie(root->arr[idx]);	    			
}


int search_trie(string process_s,trie_node *root)
{
	int idx=0,len=process_s.length();
	while(idx<len)
	{
		if(root->arr[process_s[idx]-'a']==NULL)
			return 0;
		else
			root=root->arr[process_s[idx]-'a'];
		idx++;	
	}
	return root->info.cntr;
}

