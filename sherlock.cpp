#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
ll pre[100000];

int main() {
  int T;
  cin >> T;
  while (T--) {
    ll n, x;
    cin >> n;
    for (ll i = 0; i < n; i++) {
      cin >> x;
      if (i == 0)
        pre[i] = x;
      else
        pre[i] = pre[i - 1] + x;
    }
    bool flag = false;
    for (ll i = 1; i < n - 1; i++) {
      if (pre[i - 1] == pre[n - 1] - pre[i]) {
        flag = true;
        break;
      }
    }
    if (flag == true || n == 1)
      cout << "YES"<<endl;

    else
      cout << "NO"<<endl;
  }
  return 0;
}
