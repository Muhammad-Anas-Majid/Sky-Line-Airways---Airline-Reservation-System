#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include<conio.h>
#include <cstdlib>
#include <thread>
#include<chrono>
using namespace std;

/* ======================= COLORS ======================= */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"




//============= TYPING EFFECT FUNCTION ======================
void typeme(string sentence, int delay_ms = 50)
{

    for (int i = 0; i < sentence.size(); i++)
    {

        cout << sentence[i];
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
}


// ======================= OUTPUT COLORING FUNCTIONS ======================= 
void clearScreen()
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void pauseScreen()
{

    typeme("\nPress ENTER to continue...");
    cin.ignore();
    cin.get();

}

void printLine()
{
    cout << BLUE << "------------------------------------------------------------" << RESET << endl;
}

void printDoubleLine()
{
    cout << BLUE << "============================================================" << RESET << endl;
}

void printHeader(string title)
{
    clearScreen();
    printDoubleLine();
    cout << BOLD << CYAN << "  " << title << RESET << endl;
    printDoubleLine();
}

//======================= STRUCTS OF THIS PROGRAM ======================= 
struct Seat
{
    bool booked;
    bool justBooked;
    Seat()
    {
        booked = false;
        justBooked = false;
    }
};

struct Flight 
{
    int id;
    string from;
    string to;
    string status;
    double fare;
    Seat seats[20][20];
};

struct Passenger
{
    int id;
    string password;
};

struct Admin
{
    int id;
    string password;
};


struct BookingHistory
{
    int passengerId;
    int flightId;
    int row;
    int col;
};

string hidden_password()
{
    string password;
    char ch;

    while (true)
    {

        ch = _getch();  

        if (ch == '\r') 
        {

            cout << endl;
            break;

        } 
        else if (ch == '\b') 
        {

            if (!password.empty())
            {

                password.pop_back();
                cout << "\b \b";

            }
        }
        else
        {

            password.push_back(ch);
            cout << '*';

        }
    }

    return password;
}



//======================= GLOBAL DATA OF PROGRAM ======================= 
Flight* flights = nullptr;
Passenger* passengers = nullptr;
BookingHistory* historyList = nullptr;

int flight_count = 0;
int passenger_count = 0;
int booking_count = 0;

Admin adminUser;

//======================== FLIGHTS MODULE FOR TALHA ====================


void setupFlights()
{
    flight_count = 20;
    flights = new Flight[flight_count];

    string countries[20] =
    {

        "Pakistan","USA","UK","Canada","Germany",
        "France","Japan","China","Australia","UAE",
        "India","Italy","Spain","Turkey","Egypt",
        "Brazil","Russia","Norway","Sweden","Malaysia"
    };

    for (int i = 0; i < flight_count; i++)
    {

        flights[i].id = 100 + i;
        flights[i].from = countries[i];
        flights[i].to = countries[(i + 5) % 20];
        flights[i].fare = 200 + (i * 25);

        if (i % 7 == 0)
        {

            flights[i].status = "Cancelled";

        }
        else if (i % 5 == 0)
        {

            flights[i].status = "Delayed";

        }

        else
        {

            flights[i].status = "Available";

        }
    }
}
void addFlight()
{
    printHeader("ADD NEW FLIGHT");

    Flight temp;

    typeme("Enter Flight ID: ");
    cin >> temp.id;

   
    for (int i = 0; i < flight_count; i++)
    {
        if (flights[i].id == temp.id)
        {

            cout << RED << "Flight ID already exists!\n" << RESET;
            pauseScreen();
            return;

        }
    }

    typeme("From: ");
    cin >> temp.from;

    typeme("To: ");
    cin >> temp.to;

    typeme("Fare: ");
    cin >> temp.fare;

    typeme("Status (Available/Delayed/Cancelled): ");
    cin >> temp.status;

    
    for (int r = 0; r < 20; r++)
    {
        for (int c = 0; c < 20; c++)
        {

            temp.seats[r][c].booked = false;
            temp.seats[r][c].justBooked = false;

        }
    }

  
    Flight* new_flights = new Flight[flight_count + 1];

    for (int i = 0; i < flight_count; i++)
        new_flights[i] = flights[i];

    new_flights[flight_count] = temp;

    delete[] flights;
    flights = new_flights;
    flight_count++;

    cout << GREEN << "\nFlight added successfully!\n" << RESET;
    pauseScreen();
}



void showFlights()
{
    printLine();
    cout << left << setw(8) << "ID"
        << setw(15) << "FROM"
        << setw(15) << "TO"
        << setw(15) << "STATUS"
        << "FARE\n";
    printLine();

    for (int i = 0; i < flight_count; i++)
    {
        if (flights[i].status == "Available") 
        {
            cout << GREEN;
        }
        else if (flights[i].status == "Delayed") 
        {
            cout << YELLOW;
        }
        else
        {
            cout << RED;
        }

        cout << setw(8) << flights[i].id
            << setw(15) << flights[i].from
            << setw(15) << flights[i].to
            << setw(15) << flights[i].status
            << "$" << flights[i].fare
            << RESET << endl;
    }
}


void showSeatMap(int flightIndex)
{

    cout << "\n        ";

    for (int column = 0; column < 20; column++)
    {

        cout << char('A' + column) << " ";

    }
    cout << endl;


    for (int row = 0; row < 20; row++)
    {

        cout << setw(3) << row << " | ";

        for (int column = 0; column < 20; column++)
        {


            if (flights[flightIndex].seats[row][column].justBooked == true)
            {

                cout << CYAN << "[$]" << RESET << " ";

            }

            else if (flights[flightIndex].seats[row][column].booked == true)
            {

                cout << RED << "[*]" << RESET << " ";

            }

            else
            {

                cout << GREEN << "[O]" << RESET << " ";

            }
        }
        cout << endl;
    }
}


//======================= PASSENGER MODULE FOR AYESHA ======================= 

void passengerSignup()
{
    printHeader("PASSENGER SIGN UP");

    Passenger temp;

    typeme("Create ID: ");
    cin >> temp.id;

    for (int i = 0; i < passenger_count; i++)
    {

        if (passengers[i].id == temp.id)
        {

            cout << RED << "ID already exists!\n" << RESET;
            pauseScreen();
            return;

        }
    }

    typeme("Create Password: ");
    temp.password = hidden_password();

    Passenger* new_passengers = new Passenger[passenger_count + 1];

    for (int i = 0; i < passenger_count; i++)
        new_passengers[i] = passengers[i];

    new_passengers[passenger_count] = temp;

    delete[] passengers;
    passengers = new_passengers;
    passenger_count++;

    cout << GREEN << "Account created successfully!\n" << RESET;
    pauseScreen();
}


bool passengerLogin(Passenger& loggedPassenger)
{
    printHeader("PASSENGER LOGIN");

    int id;
    string pass;

    typeme("ID: ");
    cin >> id;

    typeme("Password: ");
    pass = hidden_password();

    for (int i = 0; i < passenger_count; i++)
    {

        if (passengers[i].id == id && passengers[i].password == pass)
        {

            loggedPassenger = passengers[i];
            return true;

        }
    }

    cout << RED << "Wrong credentials!\n" << RESET;
    pauseScreen();

    return false;
}


void passengerHistory(Passenger& loggedPassenger)
{
    printHeader("YOUR BOOKING HISTORY");

    bool found = false;

    for (int i = 0; i < booking_count; i++)
    {
        if (historyList[i].passengerId == loggedPassenger.id)
        {

            cout << "Flight ID: " << historyList[i].flightId << endl;
            cout << "Seat: (" << historyList[i].row
                << "," << historyList[i].col << ")\n";

            found = true;

        }
    }

    if (!found)
    {

        typeme("No bookings yet.\n");
    }

    pauseScreen();
}


void changePassengerCredentials(Passenger& loggedPassenger)
{
    printHeader("CHANGE PASSENGER ID / PASSWORD");

    int old_id = loggedPassenger.id;

    typeme("  New ID: ");
    cin >> loggedPassenger.id;

    typeme("  New Password: ");
    loggedPassenger.password = hidden_password();

    
    for (int i = 0; i < passenger_count; i++)
    {
        if (passengers[i].id == old_id)
        {

            passengers[i].id = loggedPassenger.id;
            passengers[i].password = loggedPassenger.password;

            break;

        }
    }

    
    for (int i = 0; i < booking_count; i++)
    {

        if (historyList[i].passengerId == old_id)
        {

            historyList[i].passengerId = loggedPassenger.id;

        }
    }

    cout << GREEN << "\n  Credentials updated successfully!" << RESET << endl;

    pauseScreen();
}


void printReceipt(Passenger& loggedPassenger, Flight& flight, int row, int col)
{

    printHeader("BOOKING RECEIPT");

    time_t t = time(0);
    tm* timeNow = localtime(&t);

    cout << "Passenger ID: " << loggedPassenger.id << endl;

    cout << "Flight ID: " << flight.id << endl;

    cout << "From: " << flight.from << endl;
    cout << "To: " << flight.to << endl;

    cout << "Seat Row: " << row << endl;
    cout << "Seat Column: " << char('A' + col) << endl;

    cout << "Fare: " << flight.fare << endl;
    cout << "Payment: Paid" << endl;

    cout << "Date: "
        << timeNow->tm_mday << "/"
        << (timeNow->tm_mon + 1) << "/"
        << (timeNow->tm_year + 1900) << endl;

    cout << "Time: "
        << timeNow->tm_hour << ":"
        << timeNow->tm_min << ":"
        << timeNow->tm_sec << endl;

    printLine();
}

void bookSeat(Passenger& loggedPassenger)
{
    printHeader("BOOK A SEAT");
    showFlights();

    int flightId;

    typeme("Enter Flight ID: ");
    cin >> flightId;

    int flightIndex = -1;

    for (int i = 0; i < flight_count; i++)
    {

        if (flights[i].id == flightId)
        {

            flightIndex = i;
            break;

        }
    }

    if (flightIndex == -1)
    {

        cout << RED << "Flight not found!\n" << RESET;
        pauseScreen();
        return;

    }

    if (flights[flightIndex].status != "Available")
    {

        cout << RED << "Flight is not available!\n" << RESET;
        pauseScreen();
        return;

    }

    showSeatMap(flightIndex);

    int row, col;

    typeme("Enter Row (0-19): ");
    cin >> row;
    typeme("Enter Column (0-19): ");
    cin >> col;

    if (row < 0 || row > 19 || col < 0 || col > 19)
    {

        cout << RED << "Invalid seat selection!\n" << RESET;
        pauseScreen();
        return;

    }

    if (flights[flightIndex].seats[row][col].booked)
    {

        cout << RED << "Seat already booked!\n" << RESET;
        pauseScreen();
        return;

    }

   
    flights[flightIndex].seats[row][col].booked = true;
    flights[flightIndex].seats[row][col].justBooked = true;
    showSeatMap(flightIndex);

    BookingHistory* new_history = new BookingHistory[booking_count + 1];

    for (int i = 0; i < booking_count; i++)
        new_history[i] = historyList[i];

    new_history[booking_count] =
    {
        loggedPassenger.id,
        flightId,
        row,
        col
    };

    delete[] historyList;

    historyList = new_history;
    booking_count++;

    cout << GREEN << "Seat booked successfully!\n" << RESET;

    pauseScreen();
}




void passengerMenu(Passenger& loggedPassenger)
{
    int choice;
    do
    {
        printHeader("PASSENGER MENU");

        typeme("  1. View Flights\n");
        typeme("  2. Book Seat\n");
        typeme("  3. Booking History\n");
        typeme("  4. Cancel flight\n");
        typeme("  5. Change ID / Password\n");
        typeme("  6. Logout\n");
        typeme("\n  Choice: ");

        if (!(cin >> choice))
        {

            cin.clear();
            cin.ignore(1000, '\n');

            cout << RED << "\nINVALID INPUT! Enter number only.\n" << RESET;

            pauseScreen();
            continue;

        }

        if (choice == 1)
        {

            printHeader("FLIGHTS LIST");
            showFlights();
            pauseScreen();

        }
        else if (choice == 2)
           
        {
            bookSeat(loggedPassenger);
        }
        else if (choice == 3)
           
        {
            passengerHistory(loggedPassenger);
        }
        else if (choice == 4)

        {
            cout << RED << "Your Request has been sent to Admin contact at admin@gmail.com." << endl;
            pauseScreen();
        }
        else if (choice == 5)
            
        {
            changePassengerCredentials(loggedPassenger);
        }

    } while (choice != 6);
}

// ======================= ADMIN MODULE ======================= 
bool adminLogin()
{

    printHeader("ADMIN LOGIN");
    int id;
    string pass;
    typeme("  Admin ID: ");
    cin >> id;
    typeme("  Password: ");
    pass = hidden_password();

    if (id == adminUser.id && pass == adminUser.password)
    {

        return true;

    }

    cout << RED << "\n  Wrong admin credentials!" << RESET << endl;
    pauseScreen();

    return false;
}

void adminHistory()
{

    printHeader("ALL BOOKINGS HISTORY");

    if (booking_count == 0)
    {

        typeme("  No bookings yet.\n");
        pauseScreen();
        return;

    }

    for (int i = 0; i < booking_count; i++)
    {

        Flight bookedFlight;
        bool flight_found = false;

        for (int j = 0; j < flight_count; j++)
        {

            if (flights[j].id == historyList[i].flightId)
            {

                bookedFlight = flights[j];
                flight_found = true;
                break;

            }
        }

        if (!flight_found)
        {
            continue;
        }

        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);

        cout << "--------------------------------------------\n";

        typeme("Passenger ID: ");
        cout << historyList[i].passengerId << endl;

        typeme("Flight: ");
        cout << bookedFlight.from << " -> "<< bookedFlight.to<< " (ID: " << bookedFlight.id << ")\n";

        typeme("Seat: Row ");
        cout << historyList[i].row << " Column " << historyList[i].col << endl;

        typeme("Fare: $");
        cout << bookedFlight.fare << endl;

        typeme("Flight Status: ");
        cout << bookedFlight.status << endl;

        typeme("Booking Time: ");
        cout << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday << " " << ltm.tm_hour << ":" << ltm.tm_min << ":" << ltm.tm_sec << endl;

        cout << "--------------------------------------------\n";
    }

    pauseScreen();
}


void changeAdminCredentials()
{

    printHeader("CHANGE ADMIN ID / PASSWORD");

    typeme("  New Admin ID: ");
    cin >> adminUser.id;

    typeme("  New Password: ");
     adminUser.password = hidden_password();

    cout << GREEN << "\n  Admin credentials updated!" << RESET << endl;
    pauseScreen();

}
void changeFlightStatus()
{

    printHeader("CHANGE FLIGHT STATUS");
    showFlights();

    int fid;
    typeme("  Enter Flight ID to change status: ");
    cin >> fid;

    int index = -1;

    for (int i = 0; i < flight_count; i++)
    {

        if (flights[i].id == fid)
        {

            index = i;
            break;

        }
    }

    if (index == -1)
    {

        cout << RED << "  Flight not found!" << RESET << endl;
        pauseScreen();
        return;

    }

    typeme("  Enter new status (Available / Delayed / Cancelled): ");

    string newStatus;
    cin >> newStatus;

    flights[index].status = newStatus;

    cout << GREEN << "\n  Flight status updated successfully!" << RESET << endl;

    pauseScreen();
}

void adminCancelBooking()
{

    printHeader("CANCEL A BOOKING");

    if (booking_count == 0)
    {

        typeme("  No bookings available to cancel.\n");
        pauseScreen();
        return;

    }

    int passengerId, flightId, row, col;

    typeme("Enter Passenger ID to cancel booking: ");
    cin >> passengerId;

    typeme("Enter Flight ID: ");
    cin >> flightId;

    typeme("Enter Seat Row: ");
    cin >> row;

    typeme("Enter Seat Column: ");
    cin >> col;

    bool found = false;

    for (int i = 0; i < booking_count; i++)
    {

        if (historyList[i].passengerId == passengerId &&
            historyList[i].flightId == flightId &&
            historyList[i].row == row &&
            historyList[i].col == col)
        {
            
            for (int j = 0; j < flight_count; j++)
            {

                if (flights[j].id == flightId)
                {

                    flights[j].seats[row][col].booked = false;
                    flights[j].seats[row][col].justBooked = false;
                    break;

                }
            }

            for (int k = i; k < booking_count - 1; k++)
            {

                historyList[k] = historyList[k + 1];

            }

            booking_count--;
            found = true;

            break;
        }
    }

    if (found)
    {
        typeme("\nBooking cancelled successfully!\n");
    }
    else
    {
        typeme("\nBooking not found!\n");
    }

    pauseScreen();
}



void adminMenu()
{
    int choice;
    do
    {
        printHeader("ADMIN MENU");

        typeme("  1. View Flights\n");
        typeme("  2. View All Booking History\n");
        typeme("  3. Change Admin ID / Password\n");
        typeme("  4. Change Flight Status\n");
        typeme("  5. Add a flight\n");
        typeme("  6. Cancel a Booking\n");
        typeme("  7. Logout\n");
        typeme("\n  Choice: ");

        if (!(cin >> choice))
        {

            cin.clear();
            cin.ignore(1000, '\n');

            cout << RED << "\nINVALID INPUT! Enter number only.\n" << RESET;
            pauseScreen();
            continue;

        }

        if (choice == 1)
        {
            printHeader("FLIGHTS LIST");
            showFlights();
            pauseScreen();
        }
        else if (choice == 2)
           
        {
            adminHistory();
        }
        else if (choice == 3)
           
        {
            changeAdminCredentials();
        }
        else if (choice == 4)
           
        {
            changeFlightStatus();
        }
        else if (choice == 5)
          
        {
            addFlight();
        }
        else if (choice == 6)
           
        {
            adminCancelBooking();
        }

    } while (choice != 7);
}

// ====================== FILE HANDLING FUNCTIONS ======================

void saveData()
{
    ofstream file("arline.txt");

    
    for (int i = 0; i < passenger_count; i++)
    {
        file << "PASSENGER\n";
        file << "ID: " << passengers[i].id << "\n";
        file << "PASSWORD: " << passengers[i].password << "\n";
        file << "END_PASSENGER\n";
    }

   
    for (int i = 0; i < booking_count; i++)
    {
        file << "BOOKING\n";
        file << "PASSENGER_ID: " << historyList[i].passengerId << "\n";
        file << "FLIGHT_ID: " << historyList[i].flightId << "\n";
        file << "ROW: " << historyList[i].row << "\n";
        file << "COL: " << historyList[i].col << "\n";
        file << "END_BOOKING\n";
    }

 
    file << "ADMIN\n";
    file << "ID: " << adminUser.id << "\n";
    file << "PASSWORD: " << adminUser.password << "\n";
    file << "END_ADMIN\n";

    file.close();
}

void loadData()
{
    ifstream file("arline.txt");
    if (!file.is_open())
       
    {
        return;
    }

    string line;

    while (getline(file, line))
    {
       
        if (line == "PASSENGER")
        {

            Passenger temp;

            getline(file, line);
            temp.id = stoi(line.substr(4));

            getline(file, line);
            temp.password = line.substr(10);

            Passenger* new_passengers = new Passenger[passenger_count + 1];

            for (int i = 0; i < passenger_count; i++)
                new_passengers[i] = passengers[i];

            new_passengers[passenger_count] = temp;

            delete[] passengers;
            passengers = new_passengers;
            passenger_count++;

            getline(file, line); 
        }

       
        else if (line == "BOOKING")
        {
            BookingHistory temp;

            getline(file, line);
            temp.passengerId = stoi(line.substr(13));

            getline(file, line);
            temp.flightId = stoi(line.substr(10));

            getline(file, line);
            temp.row = stoi(line.substr(5));

            getline(file, line);
            temp.col = stoi(line.substr(5));

            BookingHistory* new_history = new BookingHistory[booking_count + 1];

            for (int i = 0; i < booking_count; i++)
                new_history[i] = historyList[i];

            new_history[booking_count] = temp;

            delete[] historyList;
            historyList = new_history;
            booking_count++;

            getline(file, line); 
        }

       
        else if (line == "ADMIN")
        {
            getline(file, line);
            adminUser.id = stoi(line.substr(4));

            getline(file, line);
            adminUser.password = line.substr(10);

            getline(file, line); 
        }
    }

    file.close();
}


int main()
{

    srand(time(0));

   
    passenger_count = 2;
    passengers = new Passenger[passenger_count];

    passengers[0].id = 1;
    passengers[0].password = "123";

    passengers[1].id = 2;
    passengers[1].password = "abc";

    // ===== ADMIN DEFAULT =====
    adminUser.id = 99;
    adminUser.password = "admin";

    // ===== SETUP FLIGHTS =====
    setupFlights();

    // ===== LOAD FILE DATA =====
    loadData();
    int choice;
    do
    {
        printHeader("SKYLINE AIRWAYS");

        typeme("  1. Passenger\n");
        typeme("  2. Admin\n");
        typeme("  3. Exit\n");
        typeme("\n  Choice: ");

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << RED << "\nINVALID INPUT! Enter number only.\n" << RESET;
            pauseScreen();
            continue;
        }

        if (choice == 1)
        {
            char haveAccount;
            typeme("\n  Do you have an account? (Y/N): ");
            cin >> haveAccount;

            if (haveAccount == 'N' || haveAccount == 'n')
            {
                passengerSignup();
            }

            Passenger loggedPassenger;
            if (passengerLogin(loggedPassenger))
            {
                passengerMenu(loggedPassenger);
            }
        }

        else if (choice == 2)
        {

            if (adminLogin())
            {

                adminMenu();
            }
        }
        else if (choice == 3)
        {
            saveData();
            cout << GREEN << "\nThank you for using SKYLINE AIRWAYS!\n" << RESET;
        }
        else
        {
            cout << RED << "\nINVALID CHOICE!\n" << RESET;
            pauseScreen();
        }

    } while (choice != 3);

    delete[] flights;
    delete[] passengers;
    delete[] historyList;

    return 0;
}


