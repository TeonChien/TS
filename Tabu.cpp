#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

using namespace std;

int count=0;
vector<vector<float> > Dataset;     //存讀到的資料
int center = 3;     //群中心個數
vector<vector<float> > Meanpoint;   //存群中心位置
float SSE;
const int magicnumber=7;
float tabulist[magicnumber]={0};


int check(float list[7]){
    for(int i=0;i<7;i++){
        if(list[i]==0){
            return i;
            break;
        }
    }
    return -1;
}

void cal_distance(vector<vector<float> > vrr,vector<vector<float> > arr,int group[]){
    for(int i=0;i<count;i++) {
        int min = 9999;

        for (int k = 0; k < center; k++){
            float total=0;
            for (int j = 0; j < vrr[0].size(); j++) {
                double distance = (vrr[i][j] - arr[k][j]) * (vrr[i][j] - arr[k][j]);
                total += distance;
            }

            if (total < min) {
                min=total;
                group[i]=k;
            }
        }
    }
}

void random() {
    for(int i=0; i < center; i++){
        vector<float> cell;
        int x=rand()%(count);
        for(int j=0;j<Dataset[0].size();j++){
            float r = Dataset[x][j];
            cell.push_back(r);
        }
        Meanpoint.push_back(cell);

    }
    /*
    cout<<"First random means point:"<<endl;
    for(int r = 0; r < Meanpoint.size(); r++){
        for(int c = 0; c < Meanpoint[r].size(); c++){
            cout << setw(3) << Meanpoint[r][c]<<" ";
        }
        cout<<endl;
    }
    */
}

double  cal_SSE(vector<vector<float> > vrr,vector<vector<float> > arr,int group[]){
    double total=0;
    for(int i=0;i<count;i++) {
        for (int j = 0; j < vrr[0].size(); j++) {
            double distance = (vrr[i][j] - arr[group[i]][j]) * (vrr[i][j] - arr[group[i]][j]);
            total += distance;
        }
    }

    return total;
}

int main() {
    clock_t start, end;
    start = clock();

    //char filename[FILENAME_MAX];
    srand(time(NULL));
    //cout<<"Enter the file name: ";
    //cin>>filename;
    ifstream in("IrisData.txt");
    string strLine;
    while (getline(in, strLine)){
        stringstream ss(strLine);
        vector<float> row;
        float data;
        string aa;
        while(getline(ss, aa ,',')){
            data = atof(aa.c_str());
            row.push_back(data);
        }
        Dataset.push_back(row);
        count++;
    }
    /*
    for(int r = 0; r < Dataset.size(); ++r){
        for(int c = 0; c < Dataset[r].size(); ++c){
            cout << setw(3) << Dataset[r][c]<<" ";
        }
        cout<<endl;
    }
    */
    cout<<"count: "<<count<<endl;

    int group[count];
    int final_group[count];
    float Final_Mean[magicnumber][center][Dataset[0].size()];


    for(int c=0;c<10000;c++) {
        random();
        cal_distance(Dataset, Meanpoint, group);
        /*
        for (int i = 0; i < count; i++) {
            cout << setw(6) << i << "[" << group[i] << "] ";
        }
        cout << endl;
        */
        SSE = cal_SSE(Dataset, Meanpoint, group);

        int a = check(tabulist);
        if (a != -1) {
            tabulist[a] = SSE;

            for (int r = 0; r < Meanpoint.size(); r++) {
                for (int c = 0; c < Meanpoint[r].size(); c++) {
                    Final_Mean[a][r][c] = Meanpoint[r][c];
                }
            }
        }
        else {
            int max = -9999;
            int index;
            for (int i = 0; i < magicnumber; i++) {
                if (tabulist[i] > max) {
                    max = tabulist[i];
                    index = i;
                }

            }
            if (SSE < max) {
                tabulist[index] = SSE;
                for (int r = 0; r < center; r++) {
                    for (int c = 0; c < Meanpoint[r].size(); c++) {
                        Final_Mean[index][r][c] = Meanpoint[r][c];
                    }
                }
            }
        }
        Meanpoint.clear();
    }

    for(int i=0;i<magicnumber;i++){
        cout<< tabulist[i]<< endl;
    }
    double min=9999;
    int index;
    for (int i = 0; i < magicnumber; i++) {
        if (tabulist[i] < min) {
            min = tabulist[i];
            index = i;
        }
    }
    cout<<"Final random means point:"<<endl;
    for (int r = 0; r < center; r++) {
        for (int c = 0; c < Meanpoint[r].size(); c++) {
            cout<<setw(3)<<Final_Mean[index][r][c]<<" ";
        }
        cout<<endl;
    }
    cout<<"SSE=" <<min<<endl;
    for(int i=0;i<count;i++) {
        int min = 9999;

        for (int k = 0; k < center; k++){
            double total=0;
            for (int j = 0; j < Dataset[0].size(); j++) {
                double distance = (Dataset[i][j] - Final_Mean[index][k][j]) * (Dataset[i][j] - Final_Mean[index][k][j]);
                total += distance;
            }

            if (total < min) {
                min=total;
                group[i]=k;
            }
        }
    }
    for (int i = 0; i < count; i++) {
        cout << setw(6) << i << "[" << group[i] << "] ";
    }
    cout << endl;

//    for (int i = 0; i < count; i++) {
//        cout << setw(6) << i << "[" << final_group[i] << "] ";
//    }
//    cout << endl;

//    cout <<"SSE= "<<tabulist[0]<<endl;
    end = clock();
    double diff = end - start; // ms
    printf("Program execute time: %.3f  sec", diff / CLOCKS_PER_SEC );

}

