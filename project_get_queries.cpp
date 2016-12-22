#include "project_indexing.cpp"
#include "project_get_position.cpp"
#include<vector>
#include<algorithm>
#include<map>
string corrected_string;
int correct_flag;
vector< pair<double,double> >rank;
int prev_mid=-1;


streampos get_pos(int k)
{
    ifstream in,input;
    streampos pos;
    in.open(file_indexpos.c_str());
    input.open(file_indexes.c_str());
    in.seekg(k);
    in>>k;
    input.seekg(k);
    pos=input.tellg();
    in.close();
    input.close();
    return pos;
}
    

void process(string s)
{
     int idx,k,a,b;
     string tmp;
     streampos pos;
     pos=get_position(s,file_words,0);
     ifstream in,input,inp;
     in.open(file_words.c_str());
     in.seekg(pos);
     in>>s;
     s="";
     in>>s;
     //cout<<s<<endl;
     in.close();
     k=(maxsz+1)*(toint(s)-1);
     pos=get_pos(k);
     input.open(file_indexes.c_str());
     input.seekg(pos);
     while(input>>tmp && tmp==s)
     {
         input>>a>>b;
         rank.push_back(make_pair(a,b));
     }    
     input.close();
}
  
  
void give_rank()
{
     int idx,tmp,len;
     sort(rank.begin(),rank.end());  
     streampos pos;
     for(idx=0,len=0;idx<rank.size();len++)
     {
            rank[len]=rank[idx];                               
            tmp=idx+1;
            while(tmp<rank.size() && rank[idx]==rank[tmp])
            {
                rank[len].second+=rank[tmp].second;
                tmp++;
            }
            idx=tmp;
            swap(rank[len].first,rank[len].second);
     }
     if(len!=rank.size())
     rank.erase(rank.begin()+len,rank.end());
     ifstream in;
     in.open(file_fileid.c_str());
     for(idx=0;idx<rank.size();idx++)
     {
          in.seekg((maxsz+1)*(rank[idx].second-1));
          in>>tmp;
          rank[idx].first/=tmp;
     }     
     in.close();
     sort(rank.begin(),rank.end());     
}


int get_p(int k)
{
    ifstream in;
    in.open(file_trigrampos.c_str());
    in.seekg(k);
    in>>k;
    in.close();
    return k;
}

    
string corrected_word(string s)
{
     int idx,k,i,j,r,len,stlen=s.length();
     string tmp;
     node_info info;
     double d=0;
     vector< pair<int,int> > v;
     ifstream in;
     map <int,int> mp;
     map< int,int>::iterator it;
     streampos pos,pos1;
     in.open(file_trigrams.c_str());
     pos1=in.tellg();
     for(idx=0;idx<s.length()-2;idx++)
     {
         for(j=idx,k=0;j<idx+3;j++)
            k=k*26+s[j]-'a'+1;
         mp[k]++;
     }
     it=mp.begin();
     while(it!=mp.end())
     {
         k=it->first;                          
         i=get_p((maxsz+1)*(k-1));
         in.seekg(i);
         while(in>>info.data && k==info.data)
         {
              in>>info.prev>>info.cntr;
              v.push_back(make_pair(info.prev,min(info.cntr,it->second))); 
         }
         it++;
     }
     mp.clear();
     in.close();
     sort(v.begin(),v.end());
     ifstream input;
     input.open(file_wordlen.c_str(),ios::binary);
     for(idx=0,k=0,len=100;idx<v.size();k++)
     {
          v[k]=v[idx];                         
          j=idx+1;
          while(j<v.size() && v[j]==v[idx])
          {
               v[k].second+=v[j].second;
               j++;
          }
          idx=j;
          j=3*(v[k].first-1);
          input.seekg(j);
          input>>j;
          if(d<(double)v[k].second/(stlen-v[k].second-2+j-2))
          {                                               
               r=v[k].first;
               len=j;
               d=(double)v[k].second/(stlen-v[k].second-2+j-2);
          }                                     
     }
     input.close();
     pos=get_position(tostring(r),file_idwords,1);
     fstream inp;
     inp.open(file_idwords.c_str());
     inp.seekg(pos);
     inp>>tmp;
     inp>>tmp;
     if(tmp!=s)
     {
       correct_flag=max(correct_flag,1);
       s=tmp;
     } 
     inp.close();      
     corrected_string=corrected_string+" "+s;
     return s;
}


void get_queries()
{
     string s,tmp;
     int i,j,k;
     char ch[100];
     ifstream in;
     in.open(file_fileid.c_str());
     streampos pos;
     while(true)
     {
        rank.clear();
        corrected_string="";
        correct_flag=0;        
        cout<<"enter search word/words or press enter to exit"<<endl;
        getline(cin,s);
        if(s=="")
          exit(0);
        i=0;
        while(i<s.length())
        {
           while(i<s.length() && s[i]==' ')
              i++;
           j=i+1;
           while(j<s.length() && s[j]!=' ')
              j++; 
           if(j-i>2)     
           {
              tmp=s.substr(i,j-i);
              for(k=0;k<tmp.length();k++)
								ch[k]=tmp[k];
              tmp=tmp.substr(0,stemword(ch,k));                      
              process(corrected_word(tmp));
           }
           i=j+1;
        }
        give_rank();
        if(correct_flag<=1  && rank.size())
        {                
           if(correct_flag==1)                
              cout<<"showing results for"<<" "<<corrected_string<<endl;
           for(i=rank.size()-1,j=0;i>=0 && j<=10;i--,j++)
           {
              pos=get_position(tostring(rank[i].second),file_fileid,1);
              in.seekg(pos);     
              in>>s;                    
              getline(in,s);
              cout<<s<<endl;
           }   
        }
        else
            cout<<"does not match any documents"<<endl;  
        cout<<endl;    
     }
}      
