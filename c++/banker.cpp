#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Banker
{
	public:
		Banker(){}
		~Banker(){deallocate();}
		void get_data()
		{
			cout<<"process,source:"<<endl;
			cin>>num_p>>num_s;
			init();
			cout<<"allocation,need:"<<endl;
			for (int i=0;i<num_p;++i)
				for (int j=0;j<num_s*2;++j)
					cin>>table[i][j];
			cout<<"available:"<<endl;
			for (int i=0;i<num_s;++i)
				cin>>available[i];
		}
		bool is_safe()
		{
			is_find=false;
			for (int i=0;i<num_p;++i)
				if (is_enough(i))
					allocate_for(i);
			if (!is_find)
				return false;
			return true;
		}
		bool request(int no,int* request)
		{
			int *tmp=new int[num_s];
			bool result;
			int i;
			for (i=0;i<num_s;++i)tmp[i]=table[no][i];
			for (i=0;i<num_s;++i)
			{
				if (request[i]<=table[no][num_s+i]&&request[i]<=available[i])
				else
					break;
			}
			if (i==num_s)
			{
				for (i=0;i<num_s;++i)
				{
					table[no][i]+=tmp[i];
					table[no][num_s+i]-=tmp[i];
				}
				if (is_safe())
					result=true;
				else
					result=false;
			}

			for (i=0;i<num_s;++i)
			{
				table[no][i]-=tmp[i];
				table[no][num_s+i]+=tmp[i];
			}
			return result;
		}
		bool request_change(int no,int *request)
		{

		}

	private:
		bool is_find;
		bool is_getResult()
		{
			for (int i=0;i<num_p;++i)
				if (!finish[i])
					return false;
			is_find=true;
			return true;
		}
		void allocate_for(int i)
		{
			finish[i]=true;
			result.push_back(i);
			for (int j=0;j<num_s;++j)
				available[j]+=table[i][j];
			if (is_getResult())
			{
				for_each(result.begin(),result.end(),[](const int& no){cout<<no<<' ';});
				cout<<endl;
			}	
			for (int j=0;j<num_p;++j)
			{
				if (finish[j])continue;
				if (is_enough(j))
				{
					allocate_for(j);
				}
			}
			for (int j=0;j<num_s;++j)
				available[j]-=table[i][j];
			result.pop_back();
			finish[i]=false;
		}
		bool is_enough(int i)
		{
			int j;
			for (j=0;j<num_s;++j)
				if (table[i][j+num_s]>available[j])
					break;
			return j==num_s; 
		}
		void init()
		{
			table=new int*[num_p];
			available=new int[num_s];
			finish=new int[num_p];
			for (int i=0;i<num_p;++i)
			{
				table[i]=new int[num_s*2];
				finish[i]=false;
			}
		}
		void deallocate()
		{
			delete[]available;
			delete[]finish;
			for (int i=0;i<num_p;++i)
				delete[](table[i]);
			delete[]table;
		}
		int num_p,num_s;
		int **table,*available,*finish;
		vector<int>result;
};
int main()
{
	Banker banker;
	banker.get_data();
	banker.is_safe();
	return 0;
}
