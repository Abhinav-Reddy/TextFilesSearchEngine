#include "project_search_header.h"


/* binary searches for a particular word in a file*/

bool compare(string a,string b,int flag)
{
     if(flag==0)
      return a>b;
     int i,j,c=0,d=0;
     c=toint(a);
     d=toint(b);
     return c>d;  
}

 
streampos binary_search(int start,int end,string find,string name,int flag)
{    
      ifstream in;
      int idx,mid=(start+end)/2;
      streampos pos;
      string s,tmp;
      char ch;
      in.open(name.c_str(),ios::binary);
      pos=in.tellg();
      if(start+3>=end)
      {
           in.seekg(0,ios::end);
           return in.tellg();
      }           
      in.seekg(mid);
      while((ch=in.get()) && ch!='\n')
      {               
           mid--;
           in.seekg(mid);
           if(mid<start)
             break;           
      } 
      idx=mid+1;
      in>>s;
      while((ch=in.get()) && ch!='\n')
           ;
      mid=in.tellg()-pos-1;
      in.seekg(start);
      in>>tmp;
      if(tmp==find)     
      {
          in.seekg(start);              
          return in.tellg();
      }
      while((ch=in.get()) && ch!='\n')
            ;
      start=in.tellg()-pos;            
      in.close();
      if(compare(s,find,flag))
        return binary_search(start,idx-1,find,name,flag);
      else
      {
         if(s==find)
            return binary_search(start,mid,find,name,flag);
         else
            return  binary_search(mid,end,find,name,flag);
      }     
}
                 

streampos get_position(string s,string name,int flag)
{   
     int idx,sz;
     streampos pos;
     ifstream in;
     in.open(name.c_str(),ios::binary);
     pos=in.tellg();
     in.seekg(0,ios::end);
     sz=in.tellg()-pos;
     in.close();
     return binary_search(0,sz-1,s,name,flag);
}
