#include<iostream>
#include<vector>
#include <chrono>
#include<set>

using namespace std;
int main()
{
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    /*int state[7][7];
    for (auto & i : state)
        for (auto & j : i)
            j = 1;
    for (auto & i : state) {
        for (auto & j : i) {
            cout << j << " ";
        }
        cout << endl;
    }
    auto a1 = make_pair(make_pair(1, 2), 2);
    auto a2 = make_pair(make_pair(1, 2), 1);
    if(a1 == a2)
        cout << "==";
    else if(a1 < a2)
        cout << "<";
    else
        cout << ">";


    vector<vector<int>> a(2, vector<int>(2));
    for(auto &i : a)
        for(auto &j : i){
            j = 1;
        }
    set<vector<vector<int>>> test;
    if(test.find(a)==test.end())
        cout << "1" ;
    test.insert(a);

    vector<vector<int>> b(2, vector<int>(2));
    for(auto &i : b)
        for(auto &j : i){
            j = 1;
        }

    if(test.find(b)==test.end())
        cout << "2" ;

    cout << endl << endl;
     */
    int sum = 0;
    int sum4=0;
    int sum5=0;
    int sum6=0;
    int res[100][7];
    int res2[1000][2][7];
    int res3[1000][3][7];
    int res4[1000][4][7];
    int res5[5000][7][7];
    //int res6[1000][6][7];


    int c[10];
    for(c[0]=0; c[0]<=1; c[0]++)
        for(c[1]=0; c[1]<=1; c[1]++)
            for(c[2]=0; c[2]<=1; c[2]++)
                for(c[3]=0; c[3]<=1; c[3]++)
                    for(c[4]=0; c[4]<=1; c[4]++)
                        for(c[5]=0; c[5]<=1; c[5]++)
                            for(c[6]=0; c[6]<=1; c[6]++){
                                if(!c[0]&&!c[1]&&!c[2])
                                    continue;
                                if(!c[1]&&!c[2]&&!c[3])
                                    continue;
                                if(!c[2]&&!c[3]&&!c[4])
                                    continue;
                                if(!c[3]&&!c[4]&&!c[5])
                                    continue;
                                if(!c[4]&&!c[5]&&!c[6])
                                    continue;
                                int count = 0;
                                for(int i=0; i<7; i++)
                                    if(c[i])
                                        count++;
                                if(count<=5){
                                    //for(int i=0; i<7; i++)
                                    //    cout << c[i] << " ";
                                    //cout << endl;
                                    for(int i=0; i<7; i++)
                                        res[sum][i] = c[i];
                                    sum++;

                                }

                                else
                                    continue;


                            }
    //cout << sum << endl;
    int sum2=0;
    int sum3=0;
    /*for(int i = 0; i < 73; i++)
        for(int j = 0; j < 73; j++){
            int sign=1;
            for(int k = 0; k < 7; k++){
                if(res[i][k] && res[j][k])
                    sign = 0;
                if(sign && k==6){
                    for(int l = 0; l < 7; l++){
                        res3[sum3][0][l] = res[i][l];
                        res3[sum3][1][l] = res[j][l];
                    }
                    sum3++;
                }
            }
        }
    for(int i=0; i<148; i++){
        for(int j = 0; j < 7; j++) {
            cout << res3[i][0][j] << " ";
        }
        cout << endl;
        for(int j = 0; j < 7; j++) {
            cout << res3[i][1][j] << " ";
        }
        cout << endl<< endl;
    }
*/


    for(int i = 0; i < 73; i++)
        for(int j = 0; j < 73; j++){

            int sign=1;
            for(int k = 0; k < 7; k++){
                if(!res[i][k] && !res[j][k])
                    sign = 0;
                if(sign && k==6){
                    for(int l = 0; l < 7; l++){
                        res2[sum2][0][l] = res[i][l];
                        res2[sum2][1][l] = res[j][l];
                    }
                    sum2++;
                }
            }
        }

    for(int i = 0; i < 832; i++)
        for(int j = 0; j < 73; j++){
            int sign=1;
            for(int k = 0; k < 7; k++) {
                if (res2[i][1][k] && res[j][k])
                    sign = 0;
                if (sign && k == 6) {
                    for (int l = 0; l < 7; l++) {
                        res3[sum3][0][l] = res2[i][0][l];
                        res3[sum3][1][l] = res2[i][1][l];
                        res3[sum3][2][l] = res[j][l];
                    }
                    sum3++;
                }
            }

        }
    for(int i = 0; i < 777; i++)
        for(int j = 0; j < 73; j++) {
            int sign = 1;
            for (int k = 0; k < 7; k++) {
                if (res3[i][0][k] && res[j][k])
                    sign = 0;
                if (sign && k == 6) {
                    for (int l = 0; l < 7; l++) {
                        res4[sum4][0][l] = res3[i][0][l];
                        res4[sum4][1][l] = res3[i][1][l];
                        res4[sum4][2][l] = res3[i][2][l];
                        res4[sum4][3][l] = res[j][l];
                    }
                    sum4++;
                }
            }
        }

    for(int i = 0; i < 600; i++)
        for(int j = 0; j < 73; j++){
            int sign = 1;
            for (int k = 0; k < 7; k++) {
                if (res4[i][2][k] && res[j][k])
                    sign = 0;
                if (sign && k == 6) {
                    for (int l = 0; l < 7; l++) {
                        res5[sum5][0][l] = res4[i][0][l];
                        res5[sum5][1][l] = res4[i][1][l];
                        res5[sum5][2][l] = res4[i][2][l];
                        res5[sum5][3][l] = res4[i][3][l];
                        res5[sum5][5][l] = res[j][l];
                    }
                    sum5++;
                }
            }
        }

    for(int i = 0; i < 4576; i++)
        for(int j = 0; j < 73; j++)
            for(int l = 0; l < 73; l++){
                int p;
                for(p = 0; p < 7; p++)
                    if(res5[i][0][p]+res5[i][1][p]+res5[i][2][p]+res5[i][3][p]+res5[i][5][p]+res[j][p]+res[l][p] < 4)
                        break;
                if(p==7){
                    for(int k = 0; k < 7; k++) {
                        cout << res5[i][0][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res5[i][1][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res5[i][2][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res5[i][3][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res[j][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res5[i][5][k] << " ";
                    }
                    cout << endl;
                    for(int k = 0; k < 7; k++) {
                        cout << res[l][k] << " ";
                    }
                    cout << endl<< endl;
                    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
                    cout << "Search Timing: " << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count() << "s";
                    return 0;
                }

            }

    /*for(int i = 0; i < 4576; i++){
        for(int j = 0; j < 7; j++) {
            cout << res5[i][0][j] << " ";
        }
        cout << endl;
        for(int j = 0; j < 7; j++) {
            cout << res5[i][1][j] << " ";
        }
        cout << endl;
        for(int j = 0; j < 7; j++) {
            cout << res5[i][2][j] << " ";
        }
        cout << endl;
        for(int j = 0; j < 7; j++) {
            cout << res5[i][3][j] << " ";
        }
        cout << endl;
        for(int j = 0; j < 7; j++) {
            cout << res5[i][5][j] << " ";
        }
        cout << endl<< endl;

    }
    */



    cout << sum2 <<endl;
    cout << sum3 <<endl;
    cout << sum4 <<endl;
    cout << sum5 <<endl;

    return 0;
}
