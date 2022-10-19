#include<iostream>
#include<vector>
#include <chrono>

using namespace std;

void CSP_1(){

    int sum = 0;
    int sum2 = 0;
    int sum3 = 0;
    int sum4 = 0;
    int sum5 = 0;
    int res[100][7];
    int res2[1000][2][7];
    int res3[1000][3][7];
    int res4[1000][4][7];
    int res5[4000][7][7];


    int c[10];

    //每个工人每周必须休息2天或2天以上
    //每个工人每周不可以连续休息3天(不考虑跨周情况)
    for (c[0] = 0; c[0] <= 1; c[0]++)
        for (c[1] = 0; c[1] <= 1; c[1]++)
            for (c[2] = 0; c[2] <= 1; c[2]++)
                for (c[3] = 0; c[3] <= 1; c[3]++)
                    for (c[4] = 0; c[4] <= 1; c[4]++)
                        for (c[5] = 0; c[5] <= 1; c[5]++)
                            for (c[6] = 0; c[6] <= 1; c[6]++) {
                                if (!c[0] && !c[1] && !c[2])
                                    continue;
                                if (!c[1] && !c[2] && !c[3])
                                    continue;
                                if (!c[2] && !c[3] && !c[4])
                                    continue;
                                if (!c[3] && !c[4] && !c[5])
                                    continue;
                                if (!c[4] && !c[5] && !c[6])
                                    continue;
                                int count = 0;
                                for (int i = 0; i < 7; i++)
                                    if (c[i])
                                        count++;
                                if (count <= 5) {
                                    for (int i = 0; i < 7; i++)
                                        res[sum][i] = c[i];
                                    sum++;
                                }
                            }
    //工人1和工人2是senior，每天至少有一个senior
    for (int i = 0; i < 73; i++)
        for (int j = 0; j < 73; j++) {
            int sign = 1;
            for (int k = 0; k < 7; k++) {
                if (!res[i][k] && !res[j][k])
                    sign = 0;
                if (sign && k == 6) {
                    for (int l = 0; l < 7; l++) {
                        res2[sum2][0][l] = res[i][l];
                        res2[sum2][1][l] = res[j][l];
                    }
                    sum2++;
                }
            }
        }
    //工人2和工人3不想在同一天工作
    for (int i = 0; i < 832; i++)
        for (int j = 0; j < 73; j++) {
            int sign = 1;
            for (int k = 0; k < 7; k++) {
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
    //工人1和工人4不想在同一天工作
    for (int i = 0; i < 777; i++)
        for (int j = 0; j < 73; j++) {
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
    //工人3和工人6不想在同一天工作
    for (int i = 0; i < 600; i++)
        for (int j = 0; j < 73; j++) {
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
    //cout << "sum5  " << sum5 << endl ;
    //每天安排至少 4 个人值班
    for (int i = 0; i < 4576; i++)
        for (int j = 0; j < 73; j++)
            for (int l = 0; l < 73; l++) {
                int p;
                for (p = 0; p < 7; p++)
                    if (res5[i][0][p] + res5[i][1][p] + res5[i][2][p] + res5[i][3][p] + res5[i][5][p] + res[j][p] +
                        res[l][p] < 4)
                        break;
                if (p == 7) {
                    for (int k = 0; k < 7; k++) {
                        cout << res5[i][0][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res5[i][1][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res5[i][2][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res5[i][3][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res[j][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res5[i][5][k] << " ";
                    }
                    cout << endl;
                    for (int k = 0; k < 7; k++) {
                        cout << res[l][k] << " ";
                    }
                    cout << endl << endl;
                    return;
                }
            }
}

void CSP_2() {

    int sum = 0;
    int sum2 = 0;
    int sum4 = 0;
    int sum5 = 0;
    int sum6 = 0;
    int res[100][7];
    int res2[200][2][7];
    int res4[1000][4][7];
    int res5[4000][7][7];//todo

    int c[10];

    //每个工人每周必须休息2天或2天以上
    //每个工人每周不可以连续休息3天(不考虑跨周情况)
    for (c[0] = 0; c[0] <= 1; c[0]++)
        for (c[1] = 0; c[1] <= 1; c[1]++)
            for (c[2] = 0; c[2] <= 1; c[2]++)
                for (c[3] = 0; c[3] <= 1; c[3]++)
                    for (c[4] = 0; c[4] <= 1; c[4]++)
                        for (c[5] = 0; c[5] <= 1; c[5]++)
                            for (c[6] = 0; c[6] <= 1; c[6]++) {
                                if (!c[0] && !c[1] && !c[2])
                                    continue;
                                if (!c[1] && !c[2] && !c[3])
                                    continue;
                                if (!c[2] && !c[3] && !c[4])
                                    continue;
                                if (!c[3] && !c[4] && !c[5])
                                    continue;
                                if (!c[4] && !c[5] && !c[6])
                                    continue;
                                int count = 0;
                                for (int i = 0; i < 7; i++)
                                    if (c[i])
                                        count++;
                                if (count <= 5) {
                                    for (int i = 0; i < 7; i++)
                                        res[sum][i] = c[i];
                                    sum++;
                                }
                            }

    for (int i = 0; i < 73; i++)
        for (int j = 0; j < 73; j++) {
            int k = 0;
            for (k = 0; k < 7; k++)
                if (res[i][k] && res[j][k])
                    break;
            if (k == 7) {
                for (int l = 0; l < 7; l++) {
                    res2[sum2][0][l] = res[i][l];
                    res2[sum2][1][l] = res[j][l];
                }
                sum2++;
            }
        }
    //cout << sum2;

    int sign = 0;
    int res_1, res_2, res_3, res_4;
    for (int i1 = 0; i1 < 73; i1++){
        for (int i2 = 0; i2 < 73; i2++) {
            for (int i3 = 0; i3 < 73; i3++) {
                for (int i4 = 0; i4 < 73; i4++) {
                    int p;
                    for (p = 0; p < 7; p++)
                        if (res[i1][p] + res[i2][p] + res[i3][p] + res[i4][p] < 2)
                            break;
                    if (p == 7) {
                        res_1 = i1;
                        res_2 = i2;
                        res_3 = i3;
                        res_4 = i4;
                        sign = 1;
                        break;
                    }
                }
                if (sign)
                    break;
            }
            if (sign)
                break;
        }
        if (sign)
            break;
    }

    for(int i1 = 0; i1 < 138; i1++)
        for(int i2 = 0; i2 < 138; i2++)
            for(int i3 = 0; i3 < 138; i3++){
                int p;
                for (p = 0; p < 7; p++)
                    if (res2[i1][0][p] + res2[i1][1][p] + res2[i2][0][p] + res2[i2][1][p] + res2[i3][0][p] + res2[i3][1][p] < 3)
                        break;
                if (p == 7){

                    //cout << "工人1";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i1][0][k] << " ";
                    }
                    cout << endl;

                    //cout << "工人2";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i2][0][k] << " ";
                    }
                    cout << endl;

                    for (int k = 0; k < 7; k++) {
                        cout << res[res_1][k] << " ";
                    }
                    cout << endl;

                    for (int k = 0; k < 7; k++) {
                        cout << res[res_2][k] << " ";
                    }
                    cout << endl;


                    //cout << "工人5";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i1][1][k] << " ";
                    }
                    cout << endl;

                    //cout << "工人6";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i2][1][k] << " ";
                    }
                    cout << endl;


                    for (int k = 0; k < 7; k++) {
                        cout << res[res_3][k] << " ";
                    }
                    cout << endl;


                    //cout << "工人8";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i3][0][k] << " ";
                    }
                    cout << endl;

                    for (int k = 0; k < 7; k++) {
                        cout << res[res_4][k] << " ";
                    }
                    cout << endl;
                    

                    //cout << "工人10";
                    for (int k = 0; k < 7; k++) {
                        cout << res2[i3][1][k] << " ";
                    }
                    cout << endl;

                    cout << endl;
                    return;
                }
            }


    //工人1，工人2和工人8是senior，每天至少有一个senior
//    for (int i1 = 0; i1 < 73; i1++)
//        for (int i2 = 0; i2 < 73; i2++)
//            for (int i3 = 0; i3 < 73; i3++){
//                int k = 0;
//                for (k = 0; k < 7; k++)
//                    if (!res[i1][k] && !res[i2][k] && !res[i3][k])
//                        break;
//                if (k == 7) {
//                    for (int l = 0; l < 7; l++) {
//                        res3[sum3][0][l] = res[i1][l];
//                        res3[sum3][1][l] = res[i2][l];
//                        res3[sum3][2][l] = res[i3][l];
//                    }
//                    sum3++;
//                    if(sum3 % 100 == 0)
//                        cout << sum3;
//                }
//            }
//
//    cout << sum3;
}

int main() {

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    CSP_1();
    cout << endl;
    CSP_2();

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    cout << "Search Timing: " << chrono::duration_cast<chrono::duration<double>>(t2 - t1).count()
         << "s";



    return 0;
}
