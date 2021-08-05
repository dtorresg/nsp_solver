#include <iostream>
#include <fstream>
#include <vector>
#include<sstream>
#include <string>
#include <numeric>
#include <chrono>
#include "functions.h"
#include "SecShift.h"
#include "SecDayOff.h"
#include "SecStaff.h"
#include "SecOnReq.h"
#include "SecOffReq.h"
#include "SecCover.h"

using namespace std;
// Permite encontrar el índice del empleado según su id
int empIndex(string x){
    if (x.length() < 2){
        return (int)x[0] - 65;
    }else {
        return (int)x[0] + (int)x[1] - 105;
    }
}

// Permite encontrar el índice del turno según su id
int shiftIndex(string x, vector<SecShift> SECTION_SHIFTS){
    for (int i = 0;i<SECTION_SHIFTS.size();i++){
        if (SECTION_SHIFTS[i].name == x){
            return i;
        }
    }
    return -1;
}


SecShift createSecShift(string myText){
    vector<string> result;
    vector<string> result2;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    
    string nombre = result[0];
    int tiempo = stoi(result[1]);

    if (result.size() >= 3) {
        stringstream s_stream(result[2]);
        while(s_stream.good()) {
            string substr;
            getline(s_stream, substr, '|');
            if (substr != ""){
                result2.push_back(substr);
            }
        }
    }
    SecShift resultado = SecShift(nombre,tiempo,result2);
    return resultado;
}

int equalSeparator(string text){
    vector<string> result;
    stringstream s_stream(text);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, '=');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return stoi(result[1]);
}

SecStaff createSecStaff(string myText){
    vector<string> result;
    vector<int> result2;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    string id = result[0];
    stringstream s_stream2(result[1]);
    while(s_stream2.good()) {
        string substr;
        getline(s_stream2, substr, '|');
        if (substr != ""){
            result2.push_back(equalSeparator(substr));
        }
    }
    int MaxTM = stoi(result[2]);
    int MinTM = stoi(result[3]);
    int MaxCS = stoi(result[4]);
    int MinCS = stoi(result[5]);
    int MinCDO = stoi(result[6]);
    int MaxW = stoi(result[7]);

    return SecStaff(id, result2, MaxTM, MinTM, MaxCS, MinCS, MinCDO, MaxW);
}

SecDayOff createSecDayOff(string myText){
    vector<string> result;
    vector<int> result2;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    string id = result[0];
    for(int i = 0; i < result.size(); i++){
        if(i == 0){
            continue; 
        } else{
            result2.push_back(stoi(result[i]));
        }
    }
    return SecDayOff(id,result2);
}

SecOnReq createSecOnReq(string myText){
    vector<string> result;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return SecOnReq(result[0],stoi(result[1]),result[2],stoi(result[3]));
}

SecOffReq createSecOffReq(string myText){
    vector<string> result;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return SecOffReq(result[0],stoi(result[1]),result[2],stoi(result[3]));
}

SecCover createSecCover(string myText){
    vector<string> result;

    stringstream s_stream(myText);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return SecCover(stoi(result[0]), result[1], stoi(result[2]), stoi(result[3]), stoi(result[4]));
}

int captureNumber(string myText,int currentFlag){
    if(myText == "SECTION_HORIZON"){
        return 1;
    }
    if(myText == "SECTION_SHIFTS"){
        return 2;
    }
    if(myText == "SECTION_STAFF"){
        return 3;
    }
    if(myText == "SECTION_DAYS_OFF"){
        return 4;
    }
    if(myText == "SECTION_SHIFT_ON_REQUESTS"){
        return 5;
    }
    if(myText == "SECTION_SHIFT_OFF_REQUESTS"){
        return 6;
    }
    if(myText == "SECTION_COVER"){
        return 7;
    }
    return currentFlag;
}
// Funcion de evaluación del algoritmo.
int eval_func(int days, vector<SecShift> SECTION_SHIFTS, vector<vector<vector<int>>> x_edt, vector<vector<vector<int>>> q_edt, vector<int> l_t, vector<vector<vector<int>>> p_edt,vector<vector<int>> s_dt,vector<vector<int>> u_dt, vector<vector<int>> v_dt){
    int suma1 = 0;
    int suma2 = 0;
    int suma3 = 0;
    int suma4 = 0;
    int suma5 = 0;
    int temp = 0;
    vector<vector<int>> y_dt(days,vector<int>(SECTION_SHIFTS.size()));//below
    vector<vector<int>> z_dt(days,vector<int>(SECTION_SHIFTS.size()));//above
    for(int i = 0; i<x_edt.size();i++){
        for(int j = 0;j<x_edt[i].size();j++){
            for(int k = 0; k < x_edt[i][j].size();k++){
                suma1 = suma1 + (q_edt[i][j][k] *(1-x_edt[i][j][k])) + (p_edt[i][j][k]*x_edt[i][j][k]);
            }
        }
    }
    for (int j = 0; j< x_edt[0].size(); j++){
        for(int k = 0; k<x_edt[0][0].size();k++){
            for(int i = 0; i< x_edt.size();i++){
                temp = temp + x_edt[i][j][k];
            }
            if(s_dt[j][k]-temp > 0 ){
                y_dt[j][k] = s_dt[j][k]-temp;
            }
            if(s_dt[j][k]-temp < 0 ){
                z_dt[j][k] = temp - s_dt[j][k];
            }
            temp = 0;
        }
    }
    for (int j = 0; j< y_dt.size(); j++){
        for(int k = 0; k<y_dt[j].size();k++){
            suma2 = suma2 + (y_dt[j][k]*u_dt[j][k]) + (z_dt[j][k]*v_dt[j][k]);
        }
    }
    return suma1 + suma2 + suma3;
}

vector<vector<vector<int>>> movimiento(int empleado, int dia, int turno, vector<vector<vector<int>>> x_edt){
    for (int k = 0; k < x_edt[empleado][dia].size();k++){
        if (x_edt[empleado][dia][k] == 1){
            if (k == turno){
                x_edt[empleado][dia][k] = 0;
                return x_edt;
            }else{
                x_edt[empleado][dia][k] = 0;
                x_edt[empleado][dia][turno] = 1;
                return x_edt;
            }
        }
    }
    x_edt[empleado][dia][turno] = 1;
    return x_edt;
}
// Validación de restricciones duras
bool valid(bool greedy, vector<vector<vector<int>>> x_edt, vector<vector<int>> R_t, vector<vector<int>> m_et, vector<int> l_t, vector<int> a_e,vector<int> b_e, vector<int> o_e,vector<int> f_e, vector<int> c_e, vector<int> g_e, vector<vector<int>> N_e){
    int suma = 0;

    //RESTRICCION DE ROTACION DE TURNOS (EJ: TURNO A NO PUEDE SER ASIGNADO LUEGO DE B AL DIA SIGUIENTE)
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j <  x_edt[i].size() - 1; j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                if (R_t[k][0] != -1){
                    for (int r = 0; r < R_t[k].size();r++){
                        if (x_edt[i][j][k] + x_edt[i][j+1][R_t[k][r]] > 1){
                            return false;
                        }
                    }
                }
            }
        }
    }
    // DIAS LIBRES 
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j <  N_e[i].size(); j++){
            for (int k = 0;k <  x_edt[i][j].size(); k++){
                if(x_edt[i][N_e[i][j]][k] == 1){
                    return false;
                }
            }
        }
    }
    // NUMERO MAXIMO DE TURNOS DE UN TIPO QUE PUEDE SER ASIGNADO A UN EMPLEADO
    for (int i = 0; i < x_edt.size(); i++){
        for (int k = 0;k <  x_edt[0][0].size(); k++){
            for (int j = 0;j < x_edt[i].size(); j++){
                suma = suma + x_edt[i][j][k];
            }
            if (suma >  m_et[i][k]){
                //cout << "aca 3" << endl;
                return false;
            }
            suma = 0;
        }
    }
    // RESTRICCION MAXIMO DE TIEMPO DE TRABAJO
    suma = 0;
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j < x_edt[i].size(); j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                suma = suma + x_edt[i][j][k]*l_t[k];
            }
        }
        if (suma > c_e[i]){
            //cout << "aca 4" << endl;
            return false;
        }
        suma = 0;
    }
    suma = 0;
    // REATRICCION MAXIMOS TURNOS CONSECUTIVOS
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j <  x_edt[i].size(); j++){
            if (accumulate(x_edt[i][j].begin(), x_edt[i][j].end(), 0) == 1){
                suma = suma + 1;
                if (suma > g_e[i]){
                    return false;
                }
            } else {
                if (suma > g_e[i]){
                    return false;
                }
                suma = 0;
            }
        }
    }

    //RESTRICCION NUMERO MAXIMO DE FINES DE SEMANA TRABAJADOS
    suma = 0;
    for (int i = 0; i < x_edt.size();i++){
        for (int j = 0; j < x_edt[i].size()/7; j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                if (x_edt[i][j*7+5][k] == 1 || x_edt[i][j*7+6][k] == 1){
                    suma++;
                    break;
                }
            }
        }
        if (suma > a_e[i]){
            //cout << "aca 6" << endl;
            return false;
        }
        suma = 0;
    }
    if (!greedy){
        int temp = 0;
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j < x_edt[i].size(); j++){
                for (int k = 0;k < x_edt[i][j].size(); k++){
                    temp = temp + x_edt[i][j][k]*l_t[k];
                }
            }
            if (b_e[i] > temp){
                return false;
            }
            temp = 0;
        }
        temp = 0;
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j < x_edt[i].size(); j++){
                if (accumulate(x_edt[i][j].begin(), x_edt[i][j].end(), 0) == 0){
                    temp++;
                }else{
                    if (temp < o_e[i] && temp != 0){
                        return false;
                    }
                    temp = 0;
                }
            }
        }
        temp = 0;
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j < x_edt[i].size(); j++){
                if (accumulate(x_edt[i][j].begin(), x_edt[i][j].end(), 0) == 1){
                    temp++;
                }else{
                    if (temp < f_e[i] && temp != 0){
                        return false;
                    }
                    temp = 0;
                }
            }
        }
    }
    return true;
}
// Comprueba si el movimiento se encuentra en la lista tabu.
bool inTabu(int i,int j, int k,vector<vector<int>> tabu_list){
    for (int a = 0; a < tabu_list.size();a++){
        if (tabu_list[a][0] == i && tabu_list[a][1] == j && tabu_list[a][2] == k){
            return true;
        }
    }
    return false;
}