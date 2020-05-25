#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <ios>

#define SIZE_OF_EMP_NAME 40
#define SIZE_OF_EMP_ADDRESS 80
#define SIZE_OF_EMP_DESIGNATION 20
#define SIZE_OF_EMP_DEPARTMENT 20
#define SIZE_OF_EMP_TYPE 15
#define OVER_TIME_AMOUNT 400

using namespace std;

// forward declaration of the classes
class pay_slip;
class PermanentEmployee;
class ContractualEmployee;

// function prototypes
int getDearnessAllowanceRate();
int getHouseRentAllowanceRate();
int getMedicalAllownce();
int getProfessionalTax();
int getEmpNumber();
int search(int, PermanentEmployee *);
int search(int, ContractualEmployee *);
void generatePaySlip(PermanentEmployee);
void generatePaySlip(ContractualEmployee);
void computeOverTime();

// Base Class
class Employee
{
protected:
    int empNumber;
    char empName[SIZE_OF_EMP_NAME];
    char empAddress[SIZE_OF_EMP_ADDRESS];
    char empDesignation[SIZE_OF_EMP_DESIGNATION];
    char empDepartment[SIZE_OF_EMP_DEPARTMENT];

public:
    void getData();
    void displayData();
};

void Employee ::getData()
{
    empNumber = getEmpNumber();

    cout << "\nEnter the name of employee : ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(empName, SIZE_OF_EMP_NAME);

    cout << "\nEnter the address of the employee : ";
    cin.getline(empAddress, SIZE_OF_EMP_ADDRESS);

    cout << "\nEnter the designation of the employee : ";
    cin.getline(empDesignation, SIZE_OF_EMP_DESIGNATION);

    cout << "\nEnter the department of employee : ";
    cin.getline(empDepartment, SIZE_OF_EMP_DEPARTMENT);
}

void Employee::displayData()
{
    cout << "\nEmp. No. : " << empNumber;

    cout << "\nName : " << empName;

    cout << "\nAddress : " << empAddress;

    cout << "\nDesignation : " << empDesignation;

    cout << "\nDepartment : " << empDepartment;
}

//* Start of PermanetEmployee Class

// Derived from the base class : Employee

class PermanentEmployee : public Employee
{
private:
    char empType[SIZE_OF_EMP_TYPE];
    long empBasic, empDearnessAllowance, empHouseRentAllowance;
    long empProvidentFund, gross, net;
    int empMedicalAllowance, professionalTax, incomeTax;

public:
    void getData();     // over-riden method
    void displayData(); // over-riden method
    void calculate();
    void storeInFile();

    // friend functions
    friend int search(int, PermanentEmployee *);
    friend void generatePaySlip(PermanentEmployee);
    friend void computeOverTime();
};

void PermanentEmployee::getData()
{

    //calling base class getData function
    Employee::getData();

    /**
     * *Exception Handling for negative basic
    */
    try
    {
        cout << "\nEnter the basic Salary : ";
        cin >> empBasic;
        if (empBasic < 0)
        {
            throw empBasic;
        }
    }
    catch (...)
    {
        cout << "\nBasic should be positive";
        return;
    }

    /**
     * *Exception Handling for negative income tax
    */
    try
    {
        cout << "Enter the income Tax to be deducted : ";
        cin >> incomeTax;
        if (incomeTax < 0)
        {
            throw incomeTax;
        }
    }
    catch (...)
    {
        cout << "\nIncome Tax should be positive";
        return;
    }

    calculate();
}

void PermanentEmployee::calculate()
{
    int da, hra;
    strcpy(empType, "Permanent");

    // dearness allowance is calculated as percentage of basic
    da = getDearnessAllowanceRate();
    empDearnessAllowance = empBasic * da / 100;

    // House rent allowance is calculated as percentage of basic

    hra = getHouseRentAllowanceRate();

    empHouseRentAllowance = empBasic * hra / 100;

    empMedicalAllowance = getMedicalAllownce();

    // provident fund is calculated as percentage of (basic + da)
    empProvidentFund = (empBasic + empDearnessAllowance) * 12 / 100;

    professionalTax = getProfessionalTax();

    //calculation of gross
    gross = empBasic + empDearnessAllowance + empHouseRentAllowance + empMedicalAllowance;

    //calculation of net
    net = gross - professionalTax - incomeTax - empProvidentFund;
}

void PermanentEmployee::storeInFile()
{
    ofstream outfile;

    //per_emp_filr.txt stores details of permanent employees
    outfile.open("per_emp_file.txt", ios::app);

    outfile << empNumber << "\n"
            << empName << "\n";
    outfile << empAddress << "\n"
            << empDesignation << "\n";
    outfile << empDepartment << "\n"
            << empType << "\n";
    outfile << empBasic << "\n"
            << empDearnessAllowance << "\n";
    outfile << empHouseRentAllowance << "\n"
            << empMedicalAllowance << "\n";
    outfile << empProvidentFund << "\n"
            << professionalTax << "\n";
    outfile << incomeTax << "\n"
            << gross << "\n"
            << net << "\n";

    outfile.close();
}

void PermanentEmployee::displayData()
{
    Employee::displayData();

    cout << "\nEmployee Type : " << empType;

    cout << "\nBasic : " << empBasic;

    cout << "\nDearness Allownce : " << empDearnessAllowance;

    cout << "\nHouse Rent Allownce : " << empHouseRentAllowance;

    cout << "\nMedical Allownce : " << empMedicalAllowance;

    cout << "\nPF : " << empProvidentFund;

    cout << "\nProfessional Tax : " << professionalTax;

    cout << "\nGross Salary : " << gross;

    cout << "\nIncome Tax : " << incomeTax;

    cout << "\nNet Salary : " << net;
}
//* End of Permanent Employee

//* Start of Contractual Class
// Derived from the base class : Employee

class ContractualEmployee : public Employee
{
private:
    char empType[SIZE_OF_EMP_TYPE];
    long gross;
    int professionalTax;
    int incomeTax;
    long net;

public:
    void getData();     // over-riden method
    void displayData(); // over-riden method
    void calculate();
    void storeInFile();

    // friend functions
    friend int search(int, ContractualEmployee *);
    friend void generatePaySlip(ContractualEmployee);
    friend void computeOverTime();
};

// function to get the data of Contractual Employee
void ContractualEmployee::getData()
{
    Employee::getData();

    /**
     * Exception Handling for 
     * *Negative gross salary
     * *Negative Income Tax
     */
    /*
    try
    {
        cout << "\nEnter the Gross salary : ";
        cin >> gross;
        if (gross < 0)
            throw gross;
    }
    catch (...)
    {
        cout << "\nGross Salary should be positive";
        return;
    }
    try{
        cout << "\nEnter the Income Tax : ";
        cin >> incomeTax;

        if (incomeTax < 0)
            throw incomeTax;
    }
    catch (...)
    {
        cout << "\nIncome Tax should be positive";
        return;
    }*/
    cout << "\nEnter the Gross salary : ";
    cin >> gross;

    cout << "\nEnter the Income Tax : ";
    cin >> incomeTax;

    calculate();
}

void ContractualEmployee::calculate()
{
    strcpy(empType, "Contractual");

    professionalTax = getProfessionalTax();

    net = gross - professionalTax - incomeTax;
}

void ContractualEmployee::storeInFile()
{
    ofstream outfile;

    // con_emp_file.txt stores details of contractual employees
    outfile.open("con_emp_file.txt", ios::app);

    outfile << empNumber << "\n"
            << empName << "\n";
    outfile << empAddress << "\n"
            << empDesignation << "\n";
    outfile << empDepartment << "\n"
            << empType << "\n"
            << gross << "\n"
            << professionalTax << "\n";
    outfile << incomeTax << "\n"
            << net << "\n";

    outfile.close();
}
void ContractualEmployee::displayData()
{
    Employee::displayData();

    cout << "\nEmployee Type : " << empType;

    cout << "\nGross Salary : " << gross;

    cout << "\nProfessional Tax : " << professionalTax;

    cout << "\nIncome Tax : " << incomeTax;

    cout << "\nNet Salary : " << net;
}
//* End of Contractual Class

// * Start of Friend Functions

// Function Definition for searching permanent employees from files
int search(int no, PermanentEmployee *per)
{
    ifstream infile;
    PermanentEmployee p;
    char ch;

    //opening file in read mode
    infile.open("per_emp_file.txt");

    if (!infile.fail())
    {
        while (infile >> p.empNumber)
        {
            infile.ignore(1000, '\n');

            infile.getline(p.empName, SIZE_OF_EMP_NAME);
            infile.getline(p.empAddress, SIZE_OF_EMP_ADDRESS);
            infile.getline(p.empDesignation, SIZE_OF_EMP_DESIGNATION);
            infile.getline(p.empDepartment, SIZE_OF_EMP_DEPARTMENT);

            infile >> p.empType >> p.empBasic >> p.empDearnessAllowance >> p.empHouseRentAllowance;
            infile >> p.empMedicalAllowance >> p.empProvidentFund >> p.professionalTax >> p.incomeTax;
            infile >> p.gross >> p.net;
            if (no == p.empNumber)
            {
                //set pointer to object if found
                *per = p;
                return 1; // return true
            }
        }
        infile.close();
    }
    return 0; // return falsse if not found
}

// function to generate pay slips of permanent employees
void generatePaySlip(PermanentEmployee per)
{
    int no;
    char file_name[8], num[8], emp[8], c;

    cout << "\n\nEnter the month number (1...12) : ";
    cin >> no;

    if (no > 12 || no < 1)
    {
        cout << "\n\nInvalid Month Number ";
        return;
    }

    system("clear");

    cout << "::::::::::::::::::"
         << "Pay slip for month number " << no << "::::::::::::::::::";

    //Perform Calculation
    per.calculate();
    per.displayData();

    // create file name to store pay slip of employee by
    // concatenating month number with employee number with an under score charachter in between
    sprintf(num, "%d_", no);
    sprintf(emp, "%d", per.empNumber);

    strcpy(file_name, num);
    strcat(file_name, emp);

    file_name[strlen(num) + strlen(emp)] = '\0';

    ofstream outfile(file_name);

    outfile << "::::::::::::::::::"
            << "Pay slip for month number " << no << "::::::::::::::::::\n\n";

    outfile << "Emp Number : " << per.empNumber << "\n";

    outfile << "Emp Name : " << per.empName << "\n";

    outfile << "Emp Address : " << per.empAddress << "\n";

    outfile << "Emp Designation : " << per.empDesignation << "\n";

    outfile << "Emp Department : " << per.empDepartment << "\n";

    outfile << "Emp Type : " << per.empType << "\n";

    outfile << "Emp Basic : " << per.empBasic << "\n";

    outfile << "Dearness Allowance : " << per.empDearnessAllowance << "\n";

    outfile << "Medical Allowance : " << per.empMedicalAllowance << "\n";

    outfile << "Provident fund : " << per.empProvidentFund << "\n";

    outfile << "Professional Tax : " << per.professionalTax << "\n";

    outfile << "Income Tax : " << per.incomeTax << "\n";

    outfile << "Gross Salary : " << per.gross << "\n";

    outfile << "Net Salary : " << per.net << "\n";

    outfile.close();
}

// function for searching contractual employee from file
int search(int no, ContractualEmployee *con)
{
    ifstream infile;
    ContractualEmployee p;
    infile.open("con_emp_file.txt");
    if (!infile.fail())
    {
        while (infile >> p.empNumber)
        {
            infile.ignore(1000, '\n');

            infile.getline(p.empName, SIZE_OF_EMP_NAME);
            infile.getline(p.empAddress, SIZE_OF_EMP_ADDRESS);
            infile.getline(p.empDesignation, SIZE_OF_EMP_DESIGNATION);
            infile.getline(p.empDepartment, SIZE_OF_EMP_DEPARTMENT);

            infile >> p.empType >> p.gross >> p.professionalTax >> p.incomeTax >> p.net;

            if (no == p.empNumber)
            {
                //set pointer to object if found
                *con = p;
                infile.close();
                return 1; // return true
            }
        }
        infile.close();
    }
    return 0; //return false if not found
}

// Function to generate pay slip for contactual employee
void generatePaySlip(ContractualEmployee per)
{
    int no;
    char file_name[8], num[8], emp[8], c;

    cout << "\n\nEnter the month number (1...12) : ";
    cin >> no;

    if (no > 12 || no < 1)
    {
        cout << "\n\nInvalid Month Number ";
        return;
    }

    system("clear");

    cout << "::::::::::::::::::"
         << "Pay slip for month number " << no << "::::::::::::::::::";

    //Perform Calculation
    per.calculate();
    per.displayData();

    // create file name to store pay slip of employee by
    // concatenating month number with employee number with an under score charachter in between
    sprintf(num, "%d_", no);
    sprintf(emp, "%d", per.empNumber);

    strcpy(file_name, num);
    strcat(file_name, emp);

    file_name[strlen(num) + strlen(emp)] = '\0';

    ofstream outfile(file_name);

    outfile << "::::::::::::::::::"
            << "Pay slip for month number " << no << "::::::::::::::::::\n\n";

    outfile << "Emp Number : " << per.empNumber << "\n";

    outfile << "Emp Name : " << per.empName << "\n";

    outfile << "Emp Address : " << per.empAddress << "\n";

    outfile << "Emp Designation : " << per.empDesignation << "\n";

    outfile << "Emp Department : " << per.empDepartment << "\n";

    outfile << "Emp Type : " << per.empType << "\n";

    outfile << "Gross Salary : " << per.gross << "\n";

    outfile << "Professional Tax : " << per.professionalTax << "\n";

    outfile << "Income Tax : " << per.incomeTax << "\n";

    outfile << "Net Salary : " << per.net << "\n";

    outfile.close();
}
void computeOverTime()
{
    ifstream infile;

    PermanentEmployee p;
    ContractualEmployee c;
    int hours;

    ofstream outfile;

    infile.open("per_emp_file.txt");
    outfile.open("per_payroll.txt");

    system("clear");

    cout << "\n\nComputation of overtime dues for permanent employee in progress........";

    while (infile >> p.empNumber)
    {
        if (p.empNumber < 1)
            break;
        infile.ignore(1000, '\n');

        infile.getline(p.empName, SIZE_OF_EMP_NAME);
        infile.getline(p.empAddress, SIZE_OF_EMP_ADDRESS);
        infile.getline(p.empDesignation, SIZE_OF_EMP_DESIGNATION);
        infile.getline(p.empDepartment, SIZE_OF_EMP_DEPARTMENT);

        infile >> p.empType >> p.empBasic >> p.empDearnessAllowance >> p.empHouseRentAllowance;
        infile >> p.empMedicalAllowance >> p.empProvidentFund >> p.professionalTax >> p.incomeTax;
        infile >> p.gross >> p.net;

        cout << "\nEnter the over time hours for employee number " << p.empNumber << " : ";
        cin >> hours;

        p.net += (hours * OVER_TIME_AMOUNT);

        outfile << p.empNumber << "\n"
                << p.empName << "\n";
        outfile << p.empAddress << "\n"
                << p.empDesignation << "\n";
        outfile << p.empDepartment << "\n"
                << p.empType << "\n";
        outfile << p.empBasic << "\n"
                << p.empDearnessAllowance << "\n";
        outfile << p.empHouseRentAllowance << "\n"
                << p.empMedicalAllowance << "\n";
        outfile << p.empProvidentFund << "\n"
                << p.professionalTax << "\n";
        outfile << p.incomeTax << "\n"
                << p.gross << "\n"
                << hours * OVER_TIME_AMOUNT << "\n"
                << p.net << "\n";
    }

    infile.close();
    outfile.close();

    cout << "\nOvertime dues computation for permanent employees complete";
    cin.get();

    infile.open("con_emp_file.txt");
    outfile.open("con_payroll.txt");

    system("clear");

    cout << "\n\nComputation of Overtime dues for contractual employee in progress......";

    while (infile >> c.empNumber)
    {
        if (c.empNumber < 1)
            break;
        infile.ignore(1000, '\n');

        infile.getline(c.empName, SIZE_OF_EMP_NAME);
        infile.getline(c.empAddress, SIZE_OF_EMP_ADDRESS);
        infile.getline(c.empDesignation, SIZE_OF_EMP_DESIGNATION);
        infile.getline(c.empDepartment, SIZE_OF_EMP_DEPARTMENT);

        infile >> c.empType >> c.gross >> c.professionalTax >> c.incomeTax >> c.net;

        cout << "\nEnter the over time hours for employee number " << c.empNumber << " : ";
        cin >> hours;

        c.net += (hours * OVER_TIME_AMOUNT);

        outfile << c.empNumber << "\n"
                << c.empName << "\n";
        outfile << c.empAddress << "\n"
                << c.empDesignation << "\n";
        outfile << c.empDepartment << "\n"
                << c.empType << "\n"
                << c.gross << "\n"
                << c.professionalTax << "\n";
        outfile << c.incomeTax << "\n"
                << hours * OVER_TIME_AMOUNT << "\n"
                << c.net << "\n";
    }
    infile.close();
    outfile.close();

    cout << "\nOvertime dues computation for contractual employees complete";
    cin.get();

    system("clear");

    cout << "\nOvertime dues for permanent employees are as follows : \n\n";

    infile.open("per_payroll.txt");

    cout.setf(ios::left, ios::adjustfield);
    cout.width(10);
    cout << "Emp No. ";

    cout.setf(ios::left, ios::adjustfield);
    cout.width(40);
    cout << "Emp Name";

    cout.setf(ios::left, ios::adjustfield);
    cout.width(10);
    cout << "Overtime Dues"
         << "\n\n";

    while (infile >> p.empNumber)
    {
        if (p.empNumber < 1)
            break;
        infile.ignore(1000, '\n');

        infile.getline(p.empName, SIZE_OF_EMP_NAME);
        infile.getline(p.empAddress, SIZE_OF_EMP_ADDRESS);
        infile.getline(p.empDesignation, SIZE_OF_EMP_DESIGNATION);
        infile.getline(p.empDepartment, SIZE_OF_EMP_DEPARTMENT);

        infile >> p.empType >> p.empBasic >> p.empDearnessAllowance >> p.empHouseRentAllowance;
        infile >> p.empMedicalAllowance >> p.empProvidentFund >> p.professionalTax >> p.incomeTax;
        infile >> p.gross >> hours >> p.net;

        cout.setf(ios::left, ios::adjustfield);
        cout.width(10);
        cout << p.empNumber;

        cout.setf(ios::left, ios::adjustfield);
        cout.width(SIZE_OF_EMP_NAME);
        cout << p.empName;

        cout.width(10);
        cout << hours << "\n";
    }
    infile.close();

    infile.open("con_payroll.txt");

    while (infile >> c.empNumber)
    {
        if (c.empNumber < 1)
            break;
        infile.ignore(1000, '\n');

        infile.getline(c.empName, SIZE_OF_EMP_NAME);
        infile.getline(c.empAddress, SIZE_OF_EMP_ADDRESS);
        infile.getline(c.empDesignation, SIZE_OF_EMP_DESIGNATION);
        infile.getline(c.empDepartment, SIZE_OF_EMP_DEPARTMENT);

        infile >> c.empType >> c.gross >> c.professionalTax >> c.incomeTax >> hours >> c.net;

        cout.setf(ios::left, ios::adjustfield);
        cout.width(10);
        cout << c.empNumber;

        cout.setf(ios::left, ios::adjustfield);
        cout.width(SIZE_OF_EMP_NAME);
        cout << c.empName;

        cout.width(10);
        cout << hours << "\n";
    }
    infile.close();
}
// * End of Friend functons
/**
 * * Start of Functions
 */

// function for auto generating employee number
int getEmpNumber()
{
    ifstream infile;
    int no;
    infile.open("empno_file.txt");
    if (!infile.fail())
    {
        infile >> no;
        no += 1;
        infile.close();
    }
    else
        no = 1;
    ofstream outfile("empno_file.txt");
    outfile << no;
    outfile.close();
    return no;
}
// function to retrieve dearness alllowance rate from file
int getDearnessAllowanceRate()
{
    int da;
    ifstream infile("da_file.txt");
    infile >> da;
    infile.close();
    return da;
}

// function to retrieve house rate allowance rate from file
int getHouseRentAllowanceRate()
{
    int hra;
    ifstream infile("hra_file.txt");
    infile >> hra;
    infile.close();
    return hra;
}

// function to retrieve medical allowance from the file
int getMedicalAllownce()
{
    int ma;
    ifstream infile("ma_file.txt");
    infile >> ma;
    infile.close();
    return ma;
}

// function to retrieve professional tax from the file
int getProfessionalTax()
{
    int pTax;
    ifstream infile("ptax_file.txt");
    infile >> pTax;
    infile.close();
    return pTax;
}

// function to set da rate
void setDearnessAllowanceRate(int da)
{
    ofstream outfile("da_file.txt");
    outfile << da;
    outfile.close();
}

// function to set hra rate
void setHouseRentAllowanceRate(int hra)
{
    ofstream outfile("hra_file.txt");
    outfile << hra;
    outfile.close();
}

// function to set medical allowance
void setMedicalAllownce(int ma)
{
    ofstream outfile("ma_file.txt");
    outfile << ma;
    outfile.close();
}

// function to set professional tax
void setProfessionalTax(int pTax)
{
    ofstream outfile("ptax_file.txt");
    outfile << pTax;
    outfile.close();
}

/**
 * * End of Functions
 */

int main()
{
    int choice, empType, no, da, hra, ma, pTax;

    PermanentEmployee permanentPerson;
    ContractualEmployee contractPerson;

    char ch = 'y';
    while (ch == 'y' || ch == 'Y')
    {
        system("clear");

        cout << "\n::::::::::MENU::::::::::";

        cout << "\n\n1. New Employee";

        cout << "\n2. Generate Pay Slip";

        cout << "\n3. Set Dearness Allowance Rate";

        cout << "\n4. Set HRA";

        cout << "\n5. Set Professional Tax";

        cout << "\n6. Set Medical Allowance Rate";

        cout << "\n7. Display Detail of Employee";

        cout << "\n8. Compute Overtime Dues for Employee";

        cout << "\n9. Exit";

        cout << "\n\nEnter your choice : ";
        cin >> choice;

        fflush(stdin);

        system("clear");

        switch (choice)
        {
        //Entering new employee details
        case 1:
            cout << "\nEnter the Employee Type";

            cout << "\n1. Permanent Employee";
            cout << "\n2. Contractual Employee\n";
            cin >> empType;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (empType)
            {

            // Permanent Employee
            case 1:
                permanentPerson.getData();
                permanentPerson.storeInFile();
                break;

            // Contractual Employee
            case 2:
                contractPerson.getData();
                contractPerson.storeInFile();
                break;
            default:
                cout << "\nInvalid Choice";
                break;
            }
            break;
            // Generate a pay slip
        case 2:
            cout << "\n\nEnter Employee Number : ";
            cin >> no;

            cout << "\n\nEnter Employee Type : ";

            cout << "\n1. Permanent Employee : ";
            cout << "\n2. Contractual EMployee : ";
            cin >> empType;

            switch (empType)
            {
            // Permanent Employe Pay Slip generation
            case 1:
                if (search(no, &permanentPerson) == 0)
                {
                    cout << "\n\nInvalid Employee Number ";
                }
                else
                {
                    generatePaySlip(permanentPerson);
                }
                break;
                // Contractual Employee Pay Slip Generation
            case 2:
                if (search(no, &contractPerson) == 0)
                {
                    cout << "\n\nInvalid Employee Number ";
                }
                else
                {
                    generatePaySlip(contractPerson);
                }
                break;
            default:
                cout << "\n\nWrong input";
                break;
            }
            break;
        // set Dearness Allowance
        case 3:
            cout << "\n\nEnter new Dearness allowance rate : ";
            cin >> da;
            setDearnessAllowanceRate(da);
            break;
        // set HRA rate
        case 4:
            cout << "\n\nEnter the new HRA rate : ";
            cin >> hra;
            setHouseRentAllowanceRate(hra);
            break;
        // set professiona tax
        case 5:
            cout << "\n\nEnter the new professional Tax : ";
            cin >> pTax;
            setProfessionalTax(pTax);
            break;
        // set medical allowance
        case 6:
            cout << "\n\nEnter the new medical allowance : ";
            cin >> ma;
            setMedicalAllownce(ma);
            break;
        // search of employee
        case 7:
            cout << "\n\nEnter the employee number : ";
            cin >> no;
            cout << "\n\nEnter Employee Type : ";

            cout << "\n1. Permanent Employee : ";
            cout << "\n2. Contractual EMployee : ";
            cin >> empType;
            switch (empType)
            {
            // permanent employee
            case 1:
                if (search(no, &permanentPerson) == 0)
                {
                    cout << "\n\nInvalid Employee Number ";
                }
                else
                {
                    permanentPerson.displayData();
                }
                break;
            // Contractual Employee
            case 2:
                if (search(no, &contractPerson) == 0)
                {
                    cout << "\n\nInvalid Employee Number ";
                }
                else
                {
                    contractPerson.displayData();
                }

                break;
            default:
                cout << "\n\nWrong input";
                break;
            }
            break;
        // compute overtime dues for employee
        case 8:
            computeOverTime();
            break;
        case 9:
            exit(0);
        default:
            cout << "\n\nInvalid Input";
        }

        cout << "\n\nDo you want to perform more operations (y/n)";
        cin >> ch;
    }
    return 0;
}