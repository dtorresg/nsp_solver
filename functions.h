#include <string>
#include <vector>
#include "SecShift.h"
#include "SecDayOff.h"
#include "SecStaff.h"
#include "SecOnReq.h"
#include "SecOffReq.h"
#include "SecCover.h"

using namespace std;

int empIndex(string x);

int shiftIndex(string x, vector<SecShift> SECTION_SHIFTS);

SecShift createSecShift(string myText);

int equalSeparator(string text);

SecStaff createSecStaff(string myText);

SecDayOff createSecDayOff(string myText);

SecOnReq createSecOnReq(string myText);

SecOffReq createSecOffReq(string myText);

SecCover createSecCover(string myText);

int captureNumber(string myText,int currentFlag);

int eval_func(int days, vector<SecShift> SECTION_SHIFTS, vector<vector<vector<int>>> x_edt, vector<vector<vector<int>>> q_edt, vector<int> l_t, vector<int> b_e, vector<int> o_e,vector<int> f_e, vector<vector<vector<int>>> p_edt,vector<vector<int>> s_dt,vector<vector<int>> u_dt, vector<vector<int>> v_dt);

vector<vector<vector<int>>> movimiento(int empleado, int dia, int turno, vector<vector<vector<int>>> x_edt);

bool valid(vector<vector<vector<int>>> x_edt, vector<vector<int>> R_t, vector<vector<int>> m_et, vector<int> l_t, vector<int> a_e, vector<int> c_e, vector<int> g_e, vector<vector<int>> N_e);

bool inTabu(int i,int j, int k,vector<vector<int>> tabu_list);