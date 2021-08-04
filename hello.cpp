#include <iostream>
#include <fstream>
#include <vector>
#include<sstream>
#include <string>
#include <numeric>
using namespace std;

class SecShift {
    public:
        string name;
        int minutes;
        vector<string> notfollowthis;

        SecShift(string nombre, int tiempo, vector<string> notfoll) {
            name = nombre;
            minutes = tiempo;
            notfollowthis = notfoll;
        }
};
// ID, MaxShifts, MaxTotalMinutes, MinTotalMinutes, MaxConsecutiveShifts, MinConsecutiveShifts, MinConsecutiveDaysOff, MaxWeekends
class SecStaff {
    public:
        string id;
        vector<int> MaxShifts;
        int MaxTM;
        int MinTM;
        int MaxCS;
        int MinCS;
        int MinCDO;
        int MaxW;

        SecStaff(string a, vector<int> b, int c, int d,int e,int f,int g,int h){
            id = a;
            MaxShifts = b;
            MaxTM = c;
            MinTM = d;
            MaxCS = e;
            MinCS = f;
            MinCDO = g;
            MaxW = h;
        }
};

class SecDayOff {
    public:
        string id;
        vector<int> DO;
        SecDayOff(string a, vector<int> Days){
            id = a;
            DO = Days;
        }
};

class SecOnReq {
    public:
        string empId;
        int day;
        string shiftId;
        int weigth;

        SecOnReq(string a,int b, string c, int d){
            empId = a;
            day = b;
            shiftId = c;
            weigth = d;
        }
};

class SecOffReq {
    public:
        string empId;
        int day;
        string shiftId;
        int weigth;

        SecOffReq(string a,int b, string c, int d){
            empId = a;
            day = b;
            shiftId = c;
            weigth = d;
        }
};

class SecCover {
    public:
        int day;
        string shiftID;
        int req;
        int under;
        int over;

        SecCover(int a, string b, int c, int d, int e) {
            day = a;
            shiftID = b;
            req = c;
            under = d;
            over = e;
        }
};

vector<string> temp;
int days;
// DATA PARSEADA
vector<SecShift> SECTION_SHIFTS;
vector<SecStaff> SECTION_STAFF;
vector<SecDayOff> SECTION_DAYS_OFF;
vector<SecOnReq> SECTION_SHIFT_ON_REQUESTS;
vector<SecOffReq> SECTION_SHIFT_OFF_REQUESTS;
vector<SecCover> SECTION_COVER;


int empIndex(string x){
    if (x.length() < 2){
        return (int)x[0] - 65;
    }else {
        return (int)x[0] + (int)x[1] - 105;
    }
}

int shiftIndex(string x){
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

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        if (substr != ""){
            result.push_back(substr);
        }
    }
    
    string nombre = result[0];
    int tiempo = stoi(result[1]);

    if (result.size() >= 3) {
        stringstream s_stream(result[2]); //create string stream from the string
        while(s_stream.good()) {
            string substr;
            getline(s_stream, substr, '|'); //get first string delimited by comma
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
    stringstream s_stream(text); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, '='); //get first string delimited by comma
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return stoi(result[1]);
}

SecStaff createSecStaff(string myText){
    vector<string> result;
    vector<int> result2;

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        if (substr != ""){
            result.push_back(substr);
        }
    }
    string id = result[0];
    stringstream s_stream2(result[1]); //create string stream from the string
    while(s_stream2.good()) {
        string substr;
        getline(s_stream2, substr, '|'); //get first string delimited by comma
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

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
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

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return SecOnReq(result[0],stoi(result[1]),result[2],stoi(result[3]));
}

SecOffReq createSecOffReq(string myText){
    vector<string> result;

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        if (substr != ""){
            result.push_back(substr);
        }
    }
    return SecOffReq(result[0],stoi(result[1]),result[2],stoi(result[3]));
}

SecCover createSecCover(string myText){
    vector<string> result;

    stringstream s_stream(myText); //create string stream from the string
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
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

int eval_func(vector<vector<vector<int>>> x_edt, vector<vector<vector<int>>> q_edt, vector<int> l_t, vector<int> b_e, vector<int> o_e,vector<int> f_e, vector<vector<vector<int>>> p_edt,vector<vector<int>> s_dt,vector<vector<int>> u_dt, vector<vector<int>> v_dt){
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
    temp = 0;
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j < x_edt[i].size(); j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                temp = temp + x_edt[i][j][k]*l_t[k];
            }
        }
        if (b_e[i] > temp){
            suma3 = suma3 + (b_e[i] - temp)*10;
        }
    }
    temp = 0;
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j < x_edt[i].size(); j++){
            if (accumulate(x_edt[i][j].begin(), x_edt[i][j].end(), 0) == 0){
                temp++;
            }else{
                if (temp < o_e[i]){
                    suma4 = suma4 + (o_e[i] - temp)*10;
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
                    suma5 = suma5 + (f_e[i] - temp)*10;
                }
                temp = 0;
            }
        }
    }
    return suma1 + suma2 + suma3 + suma4 + suma5;
}

vector<vector<vector<int>>> movimiento(int empleado, int dia, int turno, vector<vector<vector<int>>> x_edt){
    bool swap = false;
    bool bitflip = false;
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

bool valid(vector<vector<vector<int>>> x_edt, vector<vector<int>> R_t, vector<vector<int>> m_et, vector<int> l_t, vector<int> a_e, vector<int> c_e, vector<int> g_e, vector<vector<int>> N_e){
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
                    if (j == 12 || j ==13){
                                //cout << "aca 5" << endl;
                            }
                    return false;
                }
            } else {
                if (suma > g_e[i]){
                    if (j == 12 || j ==13){
                                //cout << "aca 5.1" << endl;
                            }
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
    return true;
}

bool inTabu(int i,int j, int k,vector<vector<int>> tabu_list){
    for (int a = 0; a < tabu_list.size();a++){
        if (tabu_list[a][0] == i && tabu_list[a][1] == j && tabu_list[a][2] == k){
            return true;
        }
    }
    return false;
}

int main() {
    string myText;
    int captureFlag = 0;
    int oldFlag = 0;
    //Inicio Capturador de Instancia
    ifstream MyReadFile("instance1.txt");
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
    vector<vector<vector<int>>> x_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // 1 Si el empleado e es asignado el dia d al turno t
    vector<vector<int>> R_t(SECTION_SHIFTS.size(),vector<int>(1)); // Conjunto de los tipos de turno que no pueden ser asignados inmediatamente luego del turno de tipo t
    vector<vector<int>> N_e(SECTION_STAFF.size(),vector<int>(days)); // Dias en que el empleado e no puede ser asignado
    vector<int> l_t(SECTION_SHIFTS.size()); //largo del turno t en minutos
    vector<vector<int>> m_et(SECTION_STAFF.size(),vector<int>(SECTION_SHIFTS.size())); // numero maximo de turnos del tipo t que pueden ser asignados al empleado i
    vector<int> b_e(SECTION_STAFF.size());
    vector<int> c_e(SECTION_STAFF.size());
    vector<int> f_e(SECTION_STAFF.size());
    vector<int> g_e(SECTION_STAFF.size());
    vector<int> o_e(SECTION_STAFF.size());
    vector<int> a_e(SECTION_STAFF.size());
    vector<vector<vector<int>>> q_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // penalizacion por no asignar el empleado e en el dia d al turno t
    vector<vector<vector<int>>> p_edt(SECTION_STAFF.size(), vector<vector<int>>(days, vector<int>(max))); // penalizacion por asignar el empleado e en el dia d al turno t
    vector<vector<int>> s_dt(days,vector<int>(SECTION_SHIFTS.size()));
    vector<vector<int>> u_dt(days,vector<int>(SECTION_SHIFTS.size()));
    vector<vector<int>> v_dt(days,vector<int>(SECTION_SHIFTS.size()));
    //RELLENO ELEMENTOS
    int empindex;
    int shiftindex;
    vector<int> temp;
    for (int i = 0; i < SECTION_SHIFT_OFF_REQUESTS.size(); i++){
        empindex = empIndex(SECTION_SHIFT_OFF_REQUESTS[i].empId);
        shiftindex = shiftIndex(SECTION_SHIFT_OFF_REQUESTS[i].shiftId);
        p_edt[empindex][SECTION_SHIFT_OFF_REQUESTS[i].day][shiftindex] = SECTION_SHIFT_OFF_REQUESTS[i].weigth;
    }
    for (int i = 0; i < SECTION_SHIFT_ON_REQUESTS.size(); i++){
        empindex = empIndex(SECTION_SHIFT_ON_REQUESTS[i].empId);
        shiftindex = shiftIndex(SECTION_SHIFT_ON_REQUESTS[i].shiftId);
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
                shiftindex = shiftIndex(SECTION_SHIFTS[i].notfollowthis[j]);
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
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID);
        s_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].req;
    }
    for (int i = 0; i < SECTION_COVER.size(); i++){
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID);
        u_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].under;
    }
    for (int i = 0; i < SECTION_COVER.size(); i++){
        shiftindex = shiftIndex(SECTION_COVER[i].shiftID);
        v_dt[SECTION_COVER[i].day][shiftindex] = SECTION_COVER[i].over;
    }
    
    int global_best_score = eval_func(x_edt, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
    vector<vector<vector<int>>> global_solution;
    int local_best_score = eval_func(x_edt, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
    int current_score;
    
    cout << global_best_score << endl;
    //CONSTRUCCION DE SOLUCION INICIAL CON GREEDY
    vector<vector<vector<int>>> current_move;
    vector<vector<vector<int>>> next_move;
    for (int i = 0; i < x_edt.size(); i++){
        for (int j = 0;j <  x_edt[i].size(); j++){
            for (int k = 0;k < x_edt[i][j].size(); k++){
                current_move = movimiento(i, j, k, x_edt);
                if (valid(current_move, R_t, m_et, l_t, a_e, c_e, g_e, N_e)){
                    current_score = eval_func(current_move, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
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
    for (int k = 0; k < x_edt[0][0].size();k++){
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j <  x_edt[i].size(); j++){
                cout << x_edt[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << global_best_score << endl;


    int tabu_size = (x_edt.size() * x_edt[0].size() * x_edt[0][0].size())/2;
    vector<vector<int>> tabu_list;
    int iter = 1000;


    // TABUU SEARCH
    for (int n = 0; n < iter; n++){
        local_best_score = 99999999;
        for (int i = 0; i < x_edt.size(); i++){
            for (int j = 0;j <  x_edt[i].size(); j++){
                for (int k = 0;k < x_edt[i][j].size(); k++){
                    current_move = movimiento(i, j, k, x_edt);
                    if (valid(current_move, R_t, m_et, l_t, a_e, c_e, g_e, N_e) && !inTabu(i,j,k,tabu_list)){
                        current_score = eval_func(current_move, q_edt,l_t,b_e, o_e, f_e, p_edt, s_dt, u_dt, v_dt);
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
    }

    for (int k = 0; k < global_solution[0][0].size();k++){
        for (int i = 0; i < global_solution.size(); i++){
            for (int j = 0;j <  global_solution[i].size(); j++){
                cout << global_solution[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << global_best_score << endl;

    return 0;
}