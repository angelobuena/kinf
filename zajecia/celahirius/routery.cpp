#include <iostream>

using namespace std;

int main()
{
	int n,w;
	int k=1;					
	
	cin>>n;

	for(int i=1; i<n; i=i+k)
	{		
		k++;
		//cout<<i<<endl<<endl;
	}

	cout<<k<<endl;
}
