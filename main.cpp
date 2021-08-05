#include <iostream>
#include <fstream>
#include <vector>
#include<sstream>
#include <string>
#include <numeric>
#include <chrono>
#include "SecShift.h"
#include "SecDayOff.h"
#include "SecStaff.h"
#include "SecOnReq.h"
#include "SecOffReq.h"
#include "SecCover.h"
#include "functions.h"

using namespace std;

vector<string> temp;
int days;
// DATA PARSEADA
vector<SecShift> SECTION_SHIFTS;
vector<SecStaff> SECTION_STAFF;
vector<SecDayOff> SECTION_DAYS_OFF;
vector<SecOnReq> SECTION_SHIFT_ON_REQUESTS;
vector<SecOffReq> SECTION_SHIFT_OFF_REQUESTS;
vector<SecCover> SECTION_COVER;

int main(int argc, char const *argv[]) {
    string myText;
    int captureFlag = 0;
    int oldFlag = 0;
    //Inicio Capturador de Instancia
    ifstream MyReadFile(argv[1]);
    while (getline (MyReadFile, myText)) {
        if(myText.find("#") != 0 and !(myText.empty())){
            oldFlag = captureFlag;
            captureFlag = captureNumber(myText,captureFlag);
            if(oldFlag != captureFlag){
                continue;
            }
            // Captura SECTION_HORIZON
            if (captureFlag == 1){
                days = stoi(myText);
            }
            if (captureFlag == 2){
                SECTION_SHIFTS.push_back(createSecShift(myText));
            }
            if (captureFlag == 3){
                SECTION_STAFF.push_back(createSecStaff(myText));
            }
            if (captureFlag == 4){
                SECTION_DAYS_OFF.push_back(createSecDayOff(myText));
            }
            if (captureFlag == 5){
                SECTION_SHIFT_ON_REQUESTS.push_back(createSecOnReq(myText));
            }
            if (captureFlag == 6){
                SECTION_SHIFT_OFF_REQUESTS.push_back(createSecOffReq(myText));
            }
            if (captureFlag == 7){
                SECTION_COVER.push_back(createSecCover(myText));
            }
        }
    }
    MyReadFile.close();
    //Fin Capturador de Instancia
    int c = 0;
    int max = -1;
    int now = -1;
    for (int i =0;i < SECTION_COVER.size();i++){
        if (now != SECTION_COVER[i].day){
            if (c > max){
                max = c;
            }
            c = 1;
            now = SECTION_COVER[i].day;
        }else{
            c++;
        }
    }

    //ELEMENTOS DEL MODELO
    vector<vector<vector<int>>> x_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // 1 si el empleado e es asignado el dia d al turno t
    vector<vector<int>> R_t(SECTION_SHIFTS.size(),vector<int>(1)); // Conjunto de los tipos de turno que no pueden ser asignados inmediatamente luego del turno de tipo t
    vector<vector<int>> N_e(SECTION_STAFF.size(),vector<int>(days)); // Dias en que el empleado e no puede ser asignado
    vector<int> l_t(SECTION_SHIFTS.size()); // Largo del turno t en minutos
    vector<vector<int>> m_et(SECTION_STAFF.size(),vector<int>(SECTION_SHIFTS.size())); // Número Máximo de turnos del tipo t que pueden ser asignados al empleado i
    vector<int> b_e(SECTION_STAFF.size()); // Tiempo minimo en munitos que deben ser asignados al empleado e
    vector<int> c_e(SECTION_STAFF.size()); // Tiempo maximo en munitos que pueden ser asignados al empleado e
    vector<int> f_e(SECTION_STAFF.size()); // Mínimo de turnos consecutivos que deben ser asignados al empleado e
    vector<int> g_e(SECTION_STAFF.size()); // Máximo de turnos consecutivos a los cuales el empleado e puede ser asignado
    vector<int> o_e(SECTION_STAFF.size()); // Número mínimo de dias libres consecutivos que deben ser asignados al empleado
    vector<int> a_e(SECTION_STAFF.size()); // Número máximo de fines de semana los cuales el empleado e puede ser asignado
    vector<vector<vector<int>>> q_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // Penalización por no asignar el empleado e en el dia d al turno t
    vector<vector<vector<int>>> p_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // Penalización por asignar el empleado e en el dia d al turno t
    vector<vector<int>> s_dt(days,vector<int>(SECTION_SHIFTS.size())); // Numero preferido de empleados para ser asignado el dia d en el turno t
    vector<vector<int>> u_dt(days,vector<int>(SECTION_SHIFTS.size())); // Penalización si esque se esta debajo de la covertura preferida
    vector<vector<int>> v_dt(days,vector<int>(SECTION_SHIFTS.size())); // Penalización si esque se esta sobre la covertura preferida

    //RELLENO ELEMENTOS
    int empindex;
    int shiftindex;
    vector<int> temp;
    for (int i = 0; i < SECTION_SHIFT_OFF_REQUESTS.size(); i++){
        empindex = empIndex(SECTION_SHIFT_OFF_REQUESTS[i].empId);
        shiftindex = shiftIndex(SECTION_SHIFT_OFF_REQUESTS[i].shiftId, SECTION_SHIFTS);
        p_edt[empindex][SECTION_SHIFT_OFF_REQUESTS[i].day][shiftindex] = SECTION_SHIFT_OFF_REQUESTS[i].weigth;
    }
    for (int i = 0; i < SECTION_SHIFT_ON_REQUESTS.size(); i++){
        empindex = empIndex(SECTION_SHIFT_ON_REQUESTS[i].empId);
        shiftindex = shiftIndex(SECTION_SHIFT_ON_REQUESTS[i].shiftId, SECTION_SHIFTS);
        q_edt[empindex][SECTION_SHIFT_ON_REQUESTS[i].day][shiftindex] = SECTION_SHIFT_ON_REQUESTS[i].weigth;
    }
    temp.clear();
    for (int i = 0; i < SECTION_DAYS_OFF.size(); i++){
        empindex = empIndex(SECTION_DAYS_OFF[i].id);
        for (int j = 0; j < SECTION_DAYS_OFF[i].DO.size(); j++){
            temp.push_back(SECTION_DAYS_OFF[i].DO[j]);
        }
        N_e[empindex] = temp;
        temp.clear();
    }
    temp.clear();
    for (int i = 0; i < SECTION_SHIFTS.size(); i++){
        if (SECTION_SHIFTS[i].notfollowthis.size() == 0){
            temp.push_back(-1);
        } else {
            for (int j = 0; j < SECTION_SHIFTS[i].notfollowthis.size(); j++){
                shiftindex = shiftIndex(SECTION_SHIFTS[i].notfollowthis[j], SECTION_SHIFTS);
                temp.push_back(shiftindex);
            }
        }
        R_t[i] = temp;
        temp.clear();
    }
    for (int i = 0; i<l_t.size(); i++){
        l_t[i] = SECTION_SHIFTS[i].minutes;
    }
    for (int i = 0; i< SECTION_STAFF.size(); i++){
        for (int j = 0; j<SECTION_STAFF[i].MaxShifts.size();j++){
            m_et[i][j] = SECTION_STAFF[i].MaxShifts[j];
        }
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        b_e[i] = SECTION_STAFF[i].MinTM;
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        c_e[i] = SECTION_STAFF[i].MaxTM;
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        f_e[i] = SECTION_STAFF[i].MinCS;
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        g_e[i] = SECTION_STAFF[i].MaxCS;
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        o_e[i] = SECTION_STAFF[i].MinCDO;
    }
    for (int i = 0; i < SECTION_STAFF.size(); i++){
        a_e[i] = SECTION_STAFF[i].MaxW;
    }
    for (int i = 0; i < SECTION_COVER.size(); i++){
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID, SECTION_SHIFTS);
        s_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].req;
    }
    for (int i = 0; i < SECTION_COVER.size(); i++){
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID, SECTION_SHIFTS);
        u_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].under;
    }
    for (int i = 0; i < SECTION_COVER.size(); i++){
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID, SECTION_SHIFTS);
        v_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].over;
    }
    

    vector<vector<vector<int>>> global_solution;
    vector<vector<vector<int>>> current_move;
    vector<vector<vector<int>>> next_move;
    vector<vector<int>> tabu_list;
    int global_best_score = eval_func(days, SECTION_SHIFTS, x_edt, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
    int local_best_score = eval_func(days, SECTION_SHIFTS, x_edt, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
    int current_score;
    int tabu_size = (x_edt.size() * x_edt[0].size() * x_edt[0][0].size())/2;
    if (argc == 4){
        tabu_size = stoi(argv[3]);
    }
    int iter = stoi(argv[2]);
    auto begin = std::chrono::high_resolution_clock::now();

    //CONSTRUCCION DE SOLUCION INICIAL CON GREEDY
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j <  x_edt[i].size(); j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                current_move = movimiento(i, j, k, x_edt);
                if (valid(current_move, R_t, m_et, l_t, a_e, c_e, g_e, N_e)){
                    current_score = eval_func(days, SECTION_SHIFTS, current_move, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
                    if (current_score < local_best_score){
                        x_edt = current_move;
                        local_best_score = current_score;
                    }
                    if (current_score < global_best_score){
                        global_solution = current_move;
                        global_best_score = current_score;
                    }
                }
            }
        }
    }

    // TABUU SEARCH
    for (int n = 0; n < iter; n++){
        local_best_score = 999999999;
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j <  x_edt[i].size(); j++){
                for (int k = 0;k < x_edt[i][j].size(); k++){
                    current_move = movimiento(i, j, k, x_edt);
                    if (valid(current_move, R_t, m_et, l_t, a_e, c_e, g_e, N_e) && !inTabu(i,j,k,tabu_list)){
                        current_score = eval_func(days, SECTION_SHIFTS, current_move, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
                        if (current_score < local_best_score){
                            temp.clear();
                            temp.push_back(i);
                            temp.push_back(j);
                            temp.push_back(k);
                            next_move = current_move;
                            local_best_score = current_score;
                        }
                        if (current_score < global_best_score){
                            global_solution = current_move;
                            global_best_score = current_score;
                        }
                    }
                }
            }
        }
        if (tabu_list.size() >= tabu_size){
            tabu_list.erase(tabu_list.begin());
        }
        tabu_list.push_back(temp);
        x_edt = next_move;
        auto now =  std::chrono::high_resolution_clock::now();
        auto timeout = std::chrono::duration_cast<std::chrono::seconds>(now - begin);
        if (timeout.count() >= 7200){
            cout << "Timeout: Límite de 2 horas superado" << endl;
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    for (int i = 0; i < x_edt.size();i++){
        cout << SECTION_STAFF[i].id << ": ";
        for (int j = 0; j < x_edt[i].size();j++){
            for (int k = 0; k < x_edt[i][j].size();k++){
                if (x_edt[i][j][k] == 1){
                    cout << "(" << j << "," << SECTION_SHIFTS[k].name << ") ";
                }
            }
        }
        cout << endl;
    }
    int test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            for (int k = 0;k < global_solution[i][j].size(); k++){
                test = test + global_solution[i][j][k]*l_t[k];
            }
        }
        if (b_e[i] > test){
            global_best_score = global_best_score - (b_e[i] - test)*500;
        }
        test = 0;
    }
    test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            if (accumulate(global_solution[i][j].begin(), global_solution[i][j].end(), 0) == 0){
                test++;
            }else{
                if (test < o_e[i] && test != 0){
                    global_best_score = global_best_score - (o_e[i] - test)*500;
                }
                test = 0;
            }
        }
    }
    test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            if (accumulate(global_solution[i][j].begin(), global_solution[i][j].end(), 0) == 1){
                test++;
            }else{
                if (test < f_e[i] && test != 0){
                    global_best_score = global_best_score - (f_e[i] - test)*500;
                }
                test = 0;
            }
        }
    }

    cout << "Suma de penalizaciones: " << global_best_score << endl;
    printf("Tiempo total de ejecución: %.3f [s]\n", elapsed.count() * 1e-9);

    cout << "\n-------------------------------------\n Información Adicional \n-------------------------------------\n" << endl;
    test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            for (int k = 0;k < global_solution[i][j].size(); k++){
                test = test + global_solution[i][j][k]*l_t[k];
            }
        }
        if (b_e[i] > test){
            cout << "- No cumple con el mínimo de minutos de trabajo para el empleado " << SECTION_STAFF[i].id << endl;
        }
        test = 0;
    }
    test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            if (accumulate(global_solution[i][j].begin(), global_solution[i][j].end(), 0) == 0){
                test++;
            }else{
                if (test < o_e[i] && test != 0){
                    cout << "- No cumple con el mínimo de dias libres consecutivos para el empleado " << SECTION_STAFF[i].id << endl;
                }
                test = 0;
            }
        }
    }
    test = 0;
    for (int i = 0; i < global_solution.size(); i++){
        for (int j = 0;j < global_solution[i].size(); j++){
            if (accumulate(global_solution[i][j].begin(), global_solution[i][j].end(), 0) == 1){
                test++;
            }else{
                if (test < f_e[i] && test != 0){
                    cout << "- No cumple con el mínimo de turnos consecutivos para el empleado " << SECTION_STAFF[i].id << endl;
                }
                test = 0;
            }
        }
    }
    cout << endl;
    return 0;
}