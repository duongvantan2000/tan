#include<iostream>
#include<vector>
using namespace std;
void napvector(vector<int>&dayso, int tong)
{
    int so;
    for ( int i = 0; i < tong; i++ )
    {
        cin >> so;
        dayso.push_back(so);
    }
    return ;
}
void demphantu(vector<long>&dem, vector<int>dayso)
{
    for ( int i = 0; i < dayso.size(); i++ )
    {
        dem[dayso[i]] ++ ;
    }
}
void missingnumber(vector<int>dayso1, vector<int>dayso2)
{
    vector<long>dem1(1000000);
    vector<long>dem2(1000000);
    for ( int i = 0; i < dem1.size(); i++ )
    {
        dem1[i] = 0;
        dem2[i] = 0;
    }
    demphantu(dem1, dayso1);
    demphantu(dem2, dayso2);
    for ( int i = 0; i < dem1.size(); i++ )
    {
        if (dem1[i] != dem2[i])
        {
            cout << i << " ";
        }
    }
    return ;
}
int main()
{
    int tong1, tong2;
    cin >> tong1;
    vector<int>dayso1;
    napvector(dayso1, tong1);
    cin >> tong2;
    vector<int>dayso2;
    napvector(dayso2, tong2);
    missingnumber(dayso1, dayso2);
    return 0;
}
