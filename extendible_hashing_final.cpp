#include <bits/stdc++.h>
using namespace std;

void datacreate(int n)
{
	ofstream o;
	o.open("testdata.txt");
	if(!o)
		cout<<"Some Error in file";

	for(int i=1;i<=n;i=i+1)
		o<< i<<endl;

	o.close();	
}



class Bucket {
        int depth;
		int size; 
		
        map<int, string> values;
        
    public:
        Bucket(int depth, int size)
		{
		    this->depth=depth;
		    this->size=size;
		}
		int insert(int key,string value)
		{
		    map<int,string>::iterator it;
		    
		    it=values.find(key);
		    if(it!=values.end())
		        return -1;
		    if(isFull())
		        return 0;
		    values[key]=value;
		    return 1;
		}

		int isFull()
		{
		    if(values.size()!=size)
		        return 0;
		    else
		        return 1;
		}
		int isEmpty()
		{
		    if(values.size()!=0)
		        return 0;
		    else
		        return 1;
		}
        int getDepth()
		{
		    return depth;
		}
		
		int increaseDepth()
		{
		    depth=depth+1;
		    return depth;
		}
		
		int decreaseDepth()
		{
		    depth=depth-1;
		    return depth;
		}
		
		map<int, string> copy()
		{
		    map<int, string> temp(values.begin(),values.end());
		    return temp;
		}
		
		void clear()
		{
		    values.clear();
		}
		
		void display()
		{
		    map<int,string>::iterator it;
		    
		    for(it=values.begin();it!=values.end();it++)
		        cout<<it->first<<" ";
		        
		    cout<<endl;
		}
};


class Directory {
	    int bucket_size;
        int global_depth;
		
        vector<Bucket*> buckets;
        
		int hash(int n)
		{
			return (((1 << global_depth) - 1)&(n >> (8-global_depth)));
		}
		
        int pairIndex(int bn,int d)
		{
		    return bn^(1<<(d-1));
		}
		
		void grow()
		{
		    for(int i=0;i<(1<<global_depth);i=i+1)
		        buckets.push_back(buckets[i]);
		        
		    global_depth=global_depth+1;
		}
		
        void merge(int bn)
		{
		    int ld; 
			int pi;
			int id;
			int ds;
			int i;	
		    ld=buckets[bn]->getDepth();
		    pi=pairIndex(bn,ld);
		    id= 1<<ld;
		    ds= 1<<global_depth;
		
		    if( buckets[pi]->getDepth()==ld )
		    {
		        buckets[pi]->decreaseDepth();
		        delete(buckets[bn]);
		        buckets[bn] = buckets[pi];
		        for(i=bn-id;i>=0;i=i-id)
		            buckets[i] = buckets[pi];
		        for(i=bn+id;i<ds;i=i+id )
		            buckets[i]=buckets[pi];
		    }
		}
        void shrink()
		{
		   int flag=1; 
		   int i;
			
		    for( i=0 ; i<buckets.size() ; i=i+1)
		    {
		        if(buckets[i]->getDepth()==global_depth)
		        {
		            flag=0;
		            return;
		        }
		    }
		    global_depth=global_depth-1;
		    for(i = 0 ; i < (1<<global_depth) ; i=i+1 )
		        buckets.pop_back();
		}

        void split(int bn)
		{
		    int ld;
			int pi;
			int id;
			int ds;
			int i;
		    map<int, string> temp;
		    map<int, string>::iterator it;
		
		    ld=buckets[bn]->increaseDepth();
		    
		    if(ld>global_depth)
		        grow();
		    pi=pairIndex(bn,ld);
		    buckets[pi] = new Bucket(ld,bucket_size);
		    temp=buckets[bn]->copy();
		    buckets[bn]->clear();
		    id=(1<<ld);
		    ds=(1<<global_depth);
		    for(i=pi-id;i>=0;i=i-id)
		        buckets[i]=buckets[pi];
		    for(i=pi+id;i<ds;i=i+id)
		        buckets[i] = buckets[pi];
		    for(it=temp.begin();it!=temp.end();it++)
		        insert((*it).first,(*it).second,1);
		}
        string bucket_id(int n)
		{
		    string str;int d;
		    d=buckets[n]->getDepth();
		    str="";
		    while(d>0 && n>0)
		    {
		    	if(n%2==0)
		    	   str="0"+str;
		    	else
		        	str="1"+str;
		        
		        n=n/2;
		        d=d-1;
		    }
		    while(d>0)
		    {
		        str="0"+str;
		        d=d-1;
		    }
		    return str;
		}  
    public:
        Directory(int d, int bucket_size)
        {
			this->bucket_size=bucket_size;
			this->global_depth=d;
			
			for(int i=0;i<(1<<d);i++)
			    buckets.push_back(new Bucket(d,bucket_size));
       }
	void display(bool duplicates)
	{
	    int i;
		int j;
		int d;
	    string s;
	    set<string> shown;
	    cout<<"Finally, Global depth : "<<global_depth<<endl;
	    for(i=0;i<buckets.size();i++)
	    {
	        d=buckets[i]->getDepth();
	        s=bucket_id(i);
	        if(shown.find(s)==shown.end() || duplicates)
	        {
	            shown.insert(s);
	            for(j=d;j<=global_depth;j=j+1)
	                cout<<" ";
	                
	            cout<<s<<" => ";
	            buckets[i]->display();
	        }
	    }
	}
       void insert(int key,string value,bool reinserted)
		{
		    int bucket_no=hash(key);
		    int status=buckets[bucket_no]->insert(key,value);
		    
		    if(status==1)
		    {
		        if(reinserted)
		            cout<<"key "<<key<<" has moved to bucket "<<bucket_id(bucket_no)<<endl;
		        else
		            cout<<"key "<<key<<" has inserted in bucket "<<bucket_id(bucket_no)<<endl;
		    }
		    else if(status==0)
		    {
		        split(bucket_no);
		        insert(key,value,reinserted);
		    }
		    else
		        cout<<"The Key "<<key<<" has already exists in the bucket "<<bucket_id(bucket_no)<<endl;

		}       
};


int main()
{
    string value,choice;
    int bucket_size;
	int initial_global_depth;
	int key;
	int mode;
	int n;
   
    bool show_duplicate_buckets=0;

    cout<<"Number of data, Bucket size,Initial global depth(always 1) : ";
    cin>>n>>bucket_size>>initial_global_depth;

    //directory object
    Directory d(initial_global_depth,bucket_size);
    cout<<endl<<"directory has been initialized"<<endl;
    string name;
	string binary;
	int Tid;
	int Samount;
	int category;
	char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	
	fstream datag;
	datag.open("data.txt", ios::out);
	int count=1;
	while(n--)
	{
		datag<<count<<" ";
		count++;
		datag<<rand()%500000+1<<" ";
		string name = "";
		for(int i=0; i<=2; i++)
		{
			name=name+alphabet[rand()%26];
		}
		datag<<name<<" ";
		datag<<rand()%1500 + 1;
		datag<<"\n";
	}
	datag.close();
	
	
	fstream bfile;
	ifstream inputfile ("data.txt");
	bfile.open("binary.txt", ios::out);
	
	while(inputfile>>Tid>>Samount>>name>>category)
	{
		binary = bitset<7>(Tid).to_string();
		bfile<<Tid<<" ";
		bfile<<binary;
		bfile<<"\n";
	}
	bfile.close();

    do
    {
		cout<<endl;
		cout<<"--------------------"<<endl;
    	cout<<"Enter queries(insert / display / exit) :"<<endl;
	    cout<<"--------------------"<<endl;
        cin>>choice; 
		if(choice=="display")
        {
            cout<<endl;
            d.display(show_duplicate_buckets);
        }        
        if(choice=="insert")
        {   
        	string binary, rbinary;
        	ifstream myfile ("binary.txt");
        	while(myfile>>key>>rbinary)
        	{
				d.insert(key,rbinary, 0);
			}
       }     
    } while(choice!="exit");

    return 0;
}
