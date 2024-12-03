#include <iostream>

using namespace std;

void solve(){
    long long a, b, c, d;
    a = 0;
    cin>>b>>c>>d;
    int a_i, b_i, c_i, d_i;
    for(int i = 0; i < 64; ++i){
        // if the ith bit in d is 1
        b_i = 1 & (1 << i);
        c_i = 1 & (1 << i);
        d_i = 1 & (1 << i);

        if(d_i == 1){
            if((b_i | c_i) == 0){
                a_i = 1;
            }else if(b_i == 1){
                a_i = 0;
            }else{
                a = -1;
                std::cout << i << " There\n";
                break;
            }
        }else{
            if((b_i & c_i) == 1){
                a_i = 1;
            }else if(b_i == 0){
                a_i = 0;
            }else{
                std::cout << i << " Here\n";
                a = -1;
                break;
            }
        }
        std::cout << "a_i " << a_i << "\n";
        a += (a_i * (1 << i));
    }
    std::cout << a << '\n';


}
int main() {
    int t;
    cin>>t;
    while(t--){
        solve();
    }
    return 0;
}
