#include <iostream>
#include <fstream>
 #include <string>
#include <iomanip>
  #include <vector>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/* ================= CLASS DEFINITIONS ================= */
class doctor {
public:
       string name;
    string password;
    string designation;
    string day;
};

class patient {
public:
    string name;
     string password;
    int age;
     string diagonise;
    string medhistory;
    string contact;
};

class staff {
public:
     string name;
    string designation;
     string email;
    string contact;
      string day;
};

class appointment {
public:
      string patientName;
    string doctorName;
       string date;
    string time;
    string reason;
    string status;
    int appId;
};


 string  adminname1 (string Name)
{
    string filtered; 
    for (char c : Name)
    {
        if (isalpha(c) || isspace(c)) 
        {
            filtered += c;
            }
         }
            return filtered; 
    
}





/* ================= GLOBAL VECTORS & VARIABLES ================= */
vector<doctor> num1;
vector<patient> num2;
vector<staff> num4;
vector<appointment> appointments;
int appointmentCounter = 1000;

const int MAX_DAYS = 365;
const int MAX_PATIENTS = 50;

/* ================= FORWARD DECLARATIONS ================= */
void saveDoctorsToFile();
void savePatientsToFile();
void saveStaffToFile();

/* ================= WELCOME ================= */
void welcome_message() {
    cout << "\n==========================================" << endl;
    cout << "   Hospital Management System" << endl;
    cout << "==========================================" << endl;
}

/* ================= FILE HANDLING UTILITIES ================= */



// FIXED: Removed infinite recursion
bool open_file(fstream& file) {
    file.open("health.txt", ios::in);
    if (!file.is_open()) {
        cout << "Error: health.txt not found!" << endl;
        return false;
    }
    return true;
}

int countPatients(fstream& fin) {
    fin.clear();
    fin.seekg(0);

    string name;
    int count = 0;

    while (fin >> name) {
        count++;
        fin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    fin.clear();
    fin.seekg(0);
    return count;
}

int countDays(fstream& fin) {
    fin.clear();
    fin.seekg(0);

    string line;
    getline(fin, line);

    int count = 0;
    bool isNumber = false;

    for (char c : line) {
        if (isdigit(c) || c == '.') {
            if (!isNumber) {
                count++;
                isNumber = true;
            }
        } else {
            isNumber = false;
        }
    }

    fin.clear();
    fin.seekg(0);

    return count - 1;
}

void readPatientNames(fstream& fin, string names[], int num) {
    fin.clear();
    fin.seekg(0);

    for (int i = 0; i < num; i++) {
        fin >> names[i];
        fin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    fin.clear();
    fin.seekg(0);
}

void readHealthData(fstream& fin, double data[MAX_DAYS][MAX_PATIENTS], int days, int patients) {
    fin.clear();
    fin.seekg(0);

    string tempName;

    for (int p = 0; p < patients; p++) {
        if (!(fin >> tempName)) break;

        for (int d = 0; d < days; d++) {
            if (!(fin >> data[d][p])) {
                data[d][p] = 0;
            }
        }
    }

    fin.clear();
    fin.seekg(0);
}

void displayAll(string names[], double data[MAX_DAYS][MAX_PATIENTS], int days, int patients) {
    cout << "\nHealth Data Table:\n";
    cout << left << setw(15) << "Day/Patient";
    for (int i = 0; i < patients; i++)
        cout << setw(10) << names[i];
    cout << endl;

    for (int d = 0; d < days; d++) {
        cout << "Day " << left << setw(11) << d + 1;
        for (int p = 0; p < patients; p++)
            cout << setw(10) << data[d][p];
        cout << endl;
    }
}

/* ================= ANALYSIS LOGIC ================= */
double healthChange(double values[], int days) {
    if (days < 2) return 0.0;
    if (values[0] == 0) return 0.0;
    return ((values[days - 1] - values[0]) / values[0]) * 100;
}

string healthTrend(double values[], int days) {
    if (days < 2) return "Insufficient Data";
    int improve = 0, worsen = 0;
    for (int i = 1; i < days; i++) {
        if (values[i] > values[i - 1]) improve++;
        else if (values[i] < values[i - 1]) worsen++;
    }
    if (improve > worsen) return "Improving";
    if (worsen > improve) return "Deteriorating";
    return "Stable";
}

string recommendation(double values[], int days) {
    double change = healthChange(values, days);
    string trend = healthTrend(values, days);
    if (change > 5 && trend == "Improving") return "DISCHARGE";
    if (change < -5 && trend == "Deteriorating") return "ADMIT/ICU";
    return "OBSERVE";
}

/* ================= DOCTOR SEARCH FUNCTIONS ================= */
void DoctorDesignation() {
    string designation;
    cout << "\nEnter Designation to Search: ";
    cin >> designation;
     if(cin.fail())
     {
        clearInputBuffer();
        cout<<"Invald input Dear"<<endl;
        return ;
     }
    
    cout << "\n----- Doctors with Designation: " << designation << " -----" << endl;
    cout << left << setw(20) << "Doctor Name"
         << setw(20) << "Designation"
         << setw(20) << "Working Days" << endl;
    cout << string(60, '-') << endl;
    
    bool found = false;
    for (auto &doc : num1) {
        if (doc.designation == designation) {
            found = true;
            cout << left << setw(20) << doc.name
                 << setw(20) << doc.designation
                 << setw(20) << doc.day << endl;
        }
    }
    
    if (!found) {
        cout << "No doctors found with  this:" << designation <<"designation"<< endl;
    }
}

void findDoctorByName() {
    string name;
    cout << "\nEnter Doctor Name to Search: ";
    cin >> name;
    if(cin.fail())
    {
        clearInputBuffer();
        cout<<"Enter valid data Please"<<endl;
        return ;
    }
    
    bool found = false;
    for (auto &doc : num1) {
        if (doc.name == name) {
            found = true;
            cout << "\n----- Doctor Details -----" << endl;
            cout << "Name: " << doc.name << endl;
            cout << "Designation: " << doc.designation << endl;
            cout << "Working Days: " << doc.day << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Doctor not found: " << name << endl;
    }
}

/* ================= STAFF SEARCH FUNCTIONS ================= */
void findStaffByEmail() {
    string email;
    cout << "\nEnter Email to Search: ";
    cin >> email;
if(cin.fail())
{
    clearInputBuffer();
    cout<<"Invalid Email format:"<<endl;
    return ;
}
    
    bool found = false;
    for (auto &stf : num4) {
        if (stf.email == email) {
            found = true;
            cout << "\n----- Staff Details -----" << endl;
            cout << "Name: " << stf.name << endl;
            cout << "Designation: " << stf.designation << endl;
            cout << "Email: " << stf.email << endl;
            cout << "Contact: " << stf.contact << endl;
            cout << "Working Days: " << stf.day << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Staff not found with email: " << email << endl;
    }
}

void findStaffByDesignation() {
    string designation;
    cout << "\nEnter Designation to Search: ";
    cin >> designation;
    if(cin.fail()){
        clearInputBuffer();
        cout<<"Your Designation formate is wrong:"<<endl;
        return ;
    }
    
    cout << "\n----- Staff with Designation: " << designation << " -----" << endl;
    cout << left << setw(20) << "Staff Name"
         << setw(20) << "Designation"
         << setw(25) << "Email"
         << setw(15) << "Contact" << endl;
    cout << string(80, '-') << endl;
    
    bool found = false;
    for (auto &stf : num4) {
        if (stf.designation == designation) {
            found = true;
            cout << left << setw(20) << stf.name
                 << setw(20) << stf.designation
                 << setw(25) << stf.email
                 << setw(15) << stf.contact << endl;
        }
    }
    
    if (!found) {
        cout << "No staff found with designation: " << designation << endl;
    }
}

void findStaffByName() {
    string name;
    cout <<endl<< "Enter Staff Name to Search: ";
    cin >> name;
    if(cin.fail()){
        clearInputBuffer();
        cout<<"Enter valid Staff Formate:"<<endl;
        return ;
    }
    
    bool found = false;
    for (auto &stf : num4) {
        if (stf.name == name) {
            found = true;
            cout << "\n----- Staff Details -----" << endl;
            cout << "Name: " << stf.name << endl;
            cout << "Designation: " << stf.designation << endl;
            cout << "Email: " << stf.email << endl;
            cout << "Contact: " << stf.contact << endl;
            cout << "Working Days: " << stf.day << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Staff not found: " << name << endl;
    }
}

string mostImproved(string names[], double data[MAX_DAYS][MAX_PATIENTS], int patients, int days) {
    if (patients == 0) return "None";
    string best = names[0];
    double temp[MAX_DAYS];
    for (int d = 0; d < days; d++) temp[d] = data[d][0];

    double bestVal = healthChange(temp, days);

    for (int i = 1; i < patients; i++) {
        for (int d = 0; d < days; d++) temp[d] = data[d][i];
        double currentVal = healthChange(temp, days);

        if (currentVal > bestVal) {
            bestVal = currentVal;
            best = names[i];
        }
    }
    return best;
}

string mostCritical(string names[], double data[MAX_DAYS][MAX_PATIENTS], int patients, int days) {
    if (patients == 0)
     return "None";
                           string worst = names[0];
    
    
        double temp[MAX_DAYS];
    for (int d = 0; d < days; d++) temp[d] = data[d][0];

    double worstVal = healthChange(temp, days);

                         for (int i = 1; i < patients; i++) {
        for (int d = 0; d < days; d++) temp[d] = data[d][i];
        double currentVal = healthChange(temp, days);

        if (currentVal < worstVal) {
            worstVal = currentVal;
            worst = names[i];
        }
    }
    return worst;
}

/* ================= APPOINTMENT FUNCTIONS ================= */
void requestAppointment(string patientName) {
    appointment apt;
    apt.patientName = patientName;
    apt.appId = appointmentCounter++;
    apt.status = "Pending";

    cout << "\n----- Request Appointment -----" << endl;

    if (num1.empty()) {
        cout << "No doctors available at the moment." << endl;
        return;
    }

    cout << "\nAvailable Doctors:" << endl;
    cout << left << setw(20) << "Doctor Name" << setw(20) << "Designation" << endl;
    cout << string(40, '-') << endl;
    for (auto &doc : num1) {
        cout << left << setw(20) << doc.name << setw(20) << doc.designation << endl;
    }

    cout << "\nEnter Doctor Name: ";
    cin >> apt.doctorName;
    if(cin.fail()){
        clearInputBuffer();
        cout<<"Enter a valid Data:"<<endl;
        return ;
    }

    bool found = false;
    for (auto &doc : num1) {
        if (doc.name == apt.doctorName) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Doctor not found!" << endl;
        return;
    }

    cout << "Enter Appointment Date (e.g., 2025-01-15): ";
    cin >> apt.date;
  
        cout << "Enter Appointment Time (e.g., 10:00AM): ";
    cin >> apt.time;
    cout << "Enter Reason for Visit: ";
    clearInputBuffer();
    getline(cin, apt.reason);

    appointments.push_back(apt);

    cout << "\nAppointment requested successfully!" << endl;
   
         cout << "Appointment ID: " << apt.appId << endl;
    cout << "Status: " << apt.status << endl;
}

void viewMyAppointments(string patientName) {
    cout << "\n----- My Appointments -----" << endl;
    bool found = false;

    for (auto &apt : appointments) {
        if (apt.patientName == patientName) {
            found = true;
            cout << "\n--- Appointment ID: " << apt.appId << " ---" << endl;
            cout << "Doctor: " << apt.doctorName << endl;
           
                cout << "Date: " << apt.date << endl;
            cout << "Time: " << apt.time << endl;
              
            cout << "Reason: " << apt.reason << endl;
            
               cout << "Status: " << apt.status << endl;
        }
    }

    if (!found) {
        cout << "No appointments found." << endl;
    }
}

void viewDocApp(string doctorName) {
    cout << "\n----- Appointment Requests -----" << endl;
    bool found = false;

    for (auto &apt : appointments) {
        if (apt.doctorName == doctorName) {
            found = true;
            cout << "\n--- Appointment ID: " << apt.appId << " ---" << endl;
            cout << "Patient: " << apt.patientName << endl;
            cout << "Date: " << apt.date << endl;
            cout << "Time: " << apt.time << endl;
            cout << "Reason: " << apt.reason << endl;
            cout << "Status: " << apt.status << endl;
        }
    }

    if (!found) {
        cout << "No appointment requests found." << endl;
    }
}

void manageAppointments(string doctorName) {
    bool managing = true;

    while (managing) {
        viewDocApp(doctorName);

        cout << "\n1. Approve Appointment" << endl;
        cout << "2. Decline Appointment" << endl;
        cout << "3. Back to Doctor Menu" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        if (choice == 3) {
            managing = false;
            continue;
        }

        if (choice == 1 || choice == 2) {
            int aptId;
            cout << "Enter Appointment ID: ";
            cin >> aptId;

            if (cin.fail()) {
                clearInputBuffer();
                cout << "Invalid ID!" << endl;
                continue;
            }

            bool found = false;
            for (auto &apt : appointments) {
                if (apt.appId == aptId && apt.doctorName == doctorName) {
                    found = true;
                    if (choice == 1) {
                        apt.status = "Approved";
                        cout << "Appointment approved successfully!" << endl;
                    } else {
                        apt.status = "Declined";
                        cout << "Appointment declined." << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "Appointment not found." << endl;
            }
        }
    }
}

/* ================= DISPLAY FUNCTIONS ================= */
void displayDoctorList() {
    cout << "\n--------------Doctor List------------------" << endl;
    cout << left << setw(20) << "Doctor Name"
         << setw(20) << "Designation"
         << setw(20) << "Day" << endl;
    cout << string(60, '-') << endl;

    if (num1.empty()) {
        cout << "No doctors registered yet." << endl;
    } else {
        for (auto &doctor : num1) {
            cout << left << setw(20) << doctor.name
                 << setw(20) << doctor.designation
                 << setw(20) << doctor.day << endl;
        }
    }
}

void displayStaffList() {
    cout << "\n--------------Staff List------------------" << endl;
    cout << left << setw(20) << "Staff Name"
         << setw(20) << "Designation"
         << setw(20) << "Contact" << endl;
    cout << string(60, '-') << endl;

    if (num4.empty()) {
        cout << "No staff registered yet." << endl;
    } else {
        for (auto &staff : num4) {
            cout << left << setw(20) << staff.name
                 << setw(20) << staff.designation
                 << setw(20) << staff.contact << endl;
        }
    }
}

/* ================= FILE SAVING FUNCTIONS ================= */

void saveDoctorsToFile() {
    ofstream fout("doctors.txt");
    if (!fout) {
        cout << "Error: Unable to create doctors file!" << endl;
        return;
    }
    for (auto &doc : num1) {
        fout << doc.name << " "
             << doc.password << " "
             << doc.designation << " "
             << doc.day << "\n";
    }
    fout.close();
}

void savePatientsToFile() {
    ofstream fout("patients.txt");
    if (!fout) {
        cout << "Error: Unable to create patients file!" << endl;
        return;
    }
    for (auto &pat : num2) {
        fout << pat.name << " "
             << pat.password << " "
             << pat.age << " "
             << pat.diagonise << " "
             << pat.medhistory << " "
             << pat.contact << "\n";
    }
    fout.close();
}

void saveStaffToFile() {
    ofstream fout("staff.txt");
    if (!fout) {
        cout << "Error: Unable to create staff file!" << endl;
        return;
    }
    for (auto &stf : num4) {
        fout << stf.name << " "
             << stf.designation << " "
             << stf.email << " "
             << stf.contact << " "
             << stf.day << "\n";
    }
    fout.close();
}

void loadDoctorsFromFile() {
    ifstream fin("doctors.txt");
    if (!fin) return;

    num1.clear();
    doctor d;

    while (fin >> d.name >> d.password >> d.designation >> d.day) {
        num1.push_back(d);
    }
    fin.close();
}

void loadPatientsFromFile() {
    ifstream fin("patients.txt");
    if (!fin) return;

    num2.clear();
    patient p;

    while (fin >> p.name >> p.password >> p.age >> p.diagonise >> p.medhistory >> p.contact) {
        num2.push_back(p);
    }
    fin.close();
}

void loadStaffFromFile() {
    ifstream fin("staff.txt");
    if (!fin) return;

    num4.clear();
    staff s;

    while (fin >> s.name >> s.designation >> s.email >> s.contact >> s.day) {
        num4.push_back(s);
    }
    fin.close();
}

void saveAllData() {
    saveDoctorsToFile();
    savePatientsToFile();
    saveStaffToFile();
    cout << "\nAll data saved successfully!" << endl;
}

/* ================= SIGN IN FUNCTIONS ================= */
void doctorSignIn() {
    doctor a;
    cout << "\n----- Doctor Registration -----" << endl;
    cout << "Enter Your Name: ";
    cin >> a.name;
    cout << "Create a Password: ";
    cin >> a.password;
    cout << "Enter Your Designation: ";
    cin >> a.designation;
    cout << "Enter Your Days of Working: ";
    cin >> a.day;

    num1.push_back(a);
    saveDoctorsToFile();
    cout << "\nDoctor registered successfully!" << endl;
}

void staffSignIn() {
    staff d;
    int password;

    for (int i = 1; i <= 3; i++) {
        cout << "\nEnter Master Staff Password (123): ";
        cin >> password;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input!" << endl;
            continue;
        }

        if (password == 123) {
            cout << "Enter Your Name: ";
            cin >> d.name;
            cout << "Enter Your Days of Working: ";
            cin >> d.day;
            cout << "Enter Your Contact Number: ";
            cin >> d.contact;
            cout << "Enter Your Email ID: ";
            cin >> d.email;
            cout << "Enter Your Designation: ";
            cin >> d.designation;

            num4.push_back(d);
            saveStaffToFile();

            cout << "Staff registered successfully!" << endl;
            return;
        } else {
            cout << "Invalid password. " << (3 - i) << " attempts remaining." << endl;
        }
    }
    cout << "Maximum attempts exceeded." << endl;
}

void patientSignIn() {
    patient b;
    cout << endl<<"----- Patient Registration -----" << endl;
        cout << "Enter Your Name: ";
    cin >> b.name;
    cout << "Create a Password: ";
    cin >> b.password;
    cout << "Enter Your Age: ";
    cin >> b.age;

    if (cin.fail()) {
        clearInputBuffer();
        cout << "Invalid age! Registration failed." << endl;
        return;
    }

    cout << "Enter Your Diagnosis: ";
     cin >> b.diagonise;
   
     cout << "Enter Your Medicine History: ";
     cin >> b.medhistory;
    
     cout << "Enter Your Contact: ";
       cin >> b.contact;

    num2.push_back(b);
    savePatientsToFile();

    cout << endl<<"Patient registered successfully!" << endl;
    cout << "Name: " << b.name << endl;
    cout << "You can now login." << endl;
}

/* ================= LOGIN & MENU FUNCTIONS ================= */
void doctorMenu(string doctorName) {
    bool inMenu = true;
    while (inMenu) {
        cout << "\n========== Doctor Menu ==========" << endl;
        cout << "1. View My Appointments" << endl;
        cout << "2. Manage Appointments (Approve/Decline)" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choice) {
            case 1: viewDocApp(doctorName); break;
            case 2: manageAppointments(doctorName); break;
            case 3: inMenu = false; cout << "Logging out..." << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    }
}

bool doctorLogin() {
    string name, password;
    for (int attempt = 1; attempt <= 3; attempt++) {
        cout << "\n----- Doctor Login -----" << endl;
        cout << "Enter Doctor Name: ";
        cin >> name;
        cout << "Enter Password: ";
        cin >> password;

        for (auto &doctor : num1) {
            if (doctor.name == name && doctor.password == password) {
                cout << "\nLogin successful! Welcome Dr. " << doctor.name << endl;
                doctorMenu(name);
                return true;
            }
        }
        cout << "Invalid credentials! " << (3 - attempt) << " attempts remaining." << endl;
    }
    return false;
}

void patientMenu(string patientName) {
    bool inMenu = true;
    while (inMenu) {
        cout << endl<<"========== Patient Menu ==========" << endl;
        cout << "1. Request Appointment" << endl;
            cout << "2. View My Appointments" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choice) {
            case 1: requestAppointment(patientName); break;
           
           
           
            case 2: viewMyAppointments(patientName); break;
            
          
          
              case 3: inMenu = false; cout << "Logging out..." << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    }
}

bool patientLogin() {
    string name, password;
    for (int attempt = 1; attempt <= 3; attempt++) {
        cout <<endl<< "---- Patient Login -----" << endl;
        cout << "Enter Patient Name: ";
         cin >> name;
        
        cout << "Enter Password: ";
        cin >> password;

        for (auto &patient : num2) {
            if (patient.name == name && patient.password == password) {
                cout << "\nLogin successful! Welcome " << patient.name << endl;
                patientMenu(name);
                return true;
            }
        }
        cout << "Invalid credentials! " << (3 - attempt) << " attempts remaining." << endl;
    }
    return false;
}

// FIXED: Added staff menu
void staffMenu(string staffName) {
    bool inMenu = true;
    while (inMenu) {
        cout << "\n========== Staff Menu ==========" << endl;
        cout << "1. View Doctor List" << endl;
           
        cout << "2. View Staff List" << endl;
            cout << "3. Search Doctor by Name" << endl;
        cout << "4. Search Doctor by Designation" << endl;
           cout << "5. Search Staff by Name" << endl;
        cout << "6. Search Staff by Email" << endl;
        cout << "7. Search Staff by Designation" << endl;
           cout << "8. Logout" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choice) {
            case 1: displayDoctorList(); break;
           
           
           
            case 2:
             displayStaffList();
              break;
            case 3:
             findDoctorByName();
              break;
            
             case 4:
              DoctorDesignation();
               break;
            case 5:
             findStaffByName()
             ; break;
               case 6: 
               findStaffByEmail(); 
               break;
            case 7: 
            findStaffByDesignation();
             break;
                case 8:
                 inMenu = false; 
                 cout << "Logging out..." << endl;
                  break;
            default: 
            cout << "Invalid choice!" << endl;
        }
    }
}

bool staffLogin() {
    string name;
    cout << "\nEnter Staff Name: ";
    cin >> name;
    for (auto &staff : num4) {
        if (staff.name == name) {
            cout << endl << "Login successful! Welcome " << staff.name << endl;
            staffMenu(name);
            return true;
        }
    }
    cout << "Staff not found!" << endl;
    return false;
}

/* ================= ADMIN MENU ================= */
void adminMenu() {
    string fileName,adminname,    adminPassword;

    for (int attempt = 1; attempt <= 3; attempt++) {
        cout << "\n----- Admin Login -----" << endl;
        cout << "Enter Admin Name: ";
        cin >>fileName ;
        adminname = adminname1(fileName);
       
            cout << "Enter Admin Password: ";
        cin >> adminPassword;

        if ((adminname == "Faisal" || adminname == "faisal") &&
            (adminPassword == "faisal123" || adminPassword == "Faisal123")) {

            cout <<endl<< "Admin Login Successful!" << endl;

            bool adminRunning = true;
            while (adminRunning) {
                cout << endl<<"==========================================" << endl;
                cout << "          Admin Panel" << endl;
                cout << "==========================================" << endl;

                  
                     cout << "1. Register New Doctor" << endl;
                cout << "2. Register New Staff" << endl;
        
        
        
                cout << "3. Display Doctor List" << endl;
        cout << "4. Display Staff List" << endl;
                    cout << "5. Patient Health Analysis (File Based)" << endl;
        
        
                    cout << "6. View All Appointments" << endl;
                cout << "7. Search Doctor by Designation" << endl;
                    cout << "8. Search Staff by Email" << endl;
   
                    cout << "9. Search Staff by Designation" << endl;
                  cout << "10. Save All Data" << endl;
            
                  cout << "11. Exit Admin Panel" << endl;
                cout << "Enter your choice: ";

                int choice;
                cin >> choice;

                if (cin.fail()) {
                    clearInputBuffer();
                    cout << "Invalid input!" << endl;
                    continue;
                }

                switch (choice) {
                    case 1:
                     doctorSignIn(); 
                     break;
                    case 2: 
                    staffSignIn();
                     break;
                    case 3: 
                    displayDoctorList();
                     break;
                    case 4: 
                    displayStaffList();
                     break;
                    case 5: {
                        fstream file;
                        if (!open_file(file)) 
                           break;

                        int patients = countPatients(file);
                        int days = countDays(file);

                        if (patients > MAX_PATIENTS) patients = MAX_PATIENTS;
                        if (days > MAX_DAYS) days = MAX_DAYS;

                        string patientNames[MAX_PATIENTS];
                        double healthData[MAX_DAYS][MAX_PATIENTS];

                        readPatientNames(file, patientNames, patients);
                        readHealthData(file, healthData, days, patients);
                        file.close();

                        displayAll(patientNames, healthData, days, patients);

                        cout << "\n--- Analysis Report ---\n";
                        for (int p = 0; p < patients; p++) {
                            double temp[MAX_DAYS];
                            for (int d = 0; d < days; d++) temp[d] = healthData[d][p];

                            cout << "Patient: " << patientNames[p] << endl;
                            cout << "  Change: " << fixed << setprecision(2) << healthChange(temp, days) << "%\n";
                            cout << "  Trend: " << healthTrend(temp, days) << endl;
                            cout << "  Rec: " << recommendation(temp, days) << endl;
                        }
                        break;
                    }
                    case 6: {
                        cout << "\n----- All Appointments -----" << endl;
                        if (appointments.empty()) {
                            cout << "No appointments found." << endl;
                        } else {
                            for (auto &apt : appointments) {
                                cout << "ID: " << apt.appId
                                     << " | Pat: " << apt.patientName
                                     << " | Doc: " << apt.doctorName
                                     << " | Status: " << apt.status << endl;
                            }
                        }
                        break;
                    }
                        case 7: 
                            DoctorDesignation();
                         break;
                    case 8:  
                      findStaffByEmail();
                       break;
                    case 9:
                     findStaffByDesignation(); 
                     break;
                    case 10:
                     saveAllData(); 
                     break;
                    case 11: 
                    adminRunning = false; cout << "Exiting admin panel..." << endl; 
                    break;
                    default:
                     cout << "Invalid choice!" << endl;
                      break;

                }
            }
            return;
        }
        cout << "Invalid credentials! " << (3 - attempt) << " attempts remaining." << endl;
    }
}

/* ================= PATIENT ANALYSIS (STOCK STYLE) ================= */
void stockmenu() {
    cout << "\n--- Patient Health Analysis Module ---\n";

    fstream file;
    if (!open_file(file)) return;

    int patients = countPatients(file);
    int days = countDays(file);

    if (patients > MAX_PATIENTS) patients = MAX_PATIENTS;
    if (days > MAX_DAYS) days = MAX_DAYS;

    string patientNames[MAX_PATIENTS];
    double healthData[MAX_DAYS][MAX_PATIENTS];

    readPatientNames(file, patientNames, patients);
    readHealthData(file, healthData, days, patients);
    file.close();

    cout << "\nAnalyzing " << patients << " patients over " << days << " days...\n";

    cout << "Most Improved: " << mostImproved(patientNames, healthData, patients, days) << endl;
    cout << "Most Critical: " << mostCritical(patientNames, healthData, patients, days) << endl;
}

/* ================= MAIN ================= */
int main() {
    welcome_message();

    loadDoctorsFromFile();
    loadPatientsFromFile();
    loadStaffFromFile();

    bool running = true;
    while (running) {
        cout << "\n==========================================" << endl;
        cout << "        Hospital Management System" << endl;
        cout << "==========================================" << endl;
           cout << "1. Admin Panel" << endl;
        cout << "2. Doctor Login" << endl;
          
         cout << "3. Patient Register" << endl;
        cout << "4. Patient Login" << endl;
           cout << "5. Staff Login" << endl;
        cout << "6. Analysis (File)" << endl;
                   cout << "7. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1:
             adminMenu();
              break;
            case 2:
             doctorLogin();
              break;
            case 3: 
            patientSignIn(); 
            break;
            case 4: 
               patientLogin(); 
                break;
               case 5: 
                  staffLogin();
                     break;
            case 6:  
            stockmenu();
             break;
            case 7:
             running = false; cout << "Thank you for using the Hospital Management System!" << endl; break;
            default: 
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}
