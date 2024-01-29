#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstddef>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;

const int TOTAL_SEATS = 100;

class Movie {
public:
    Movie(const string& title, int duration) :
        title(title), duration(duration) {}

    const string& getTitle() const { return title; }
    int getDuration() const { return duration; }

private:
    string title;
    int duration;
};

class Booking {
public:
    Booking(const Movie& movie, const string& showTime, int seatNumber, int numberOfTickets) :
        movie(movie), showTime(showTime), seatNumber(seatNumber), numberOfTickets(numberOfTickets), canceled(false) {}

    void cancelBooking() { canceled = true; }
    bool isCanceled() const { return canceled; }
    int getSeatNumber() const { return seatNumber; }
    int getNumberOfTickets() const { return numberOfTickets; }
    const Movie& getMovie() const { return movie; }
    const string& getShowTime() const { return showTime; }

private:
    const Movie& movie;
    string showTime;
    int seatNumber;
    int numberOfTickets;
    bool canceled;
};

class Pantry {
public:
    // Function to add an item to the user's choices queue
    void addToChoicesQueue(const string& item) {
        choicesQueue.push(item);
    }

    // Function to display the user's choices in the order they were made
    void displayUserChoices() const {
        if (choicesQueue.empty()) {
            cout << "No choices made yet." << endl;
        } else {
            cout << "User Choices:" << endl;
            queue<string> tempQueue = choicesQueue;
            while (!tempQueue.empty()) {
                cout << tempQueue.front() << endl;
                tempQueue.pop();
            }
        }
    }

private:
    queue<string> choicesQueue;
};

class User {
public:
    User(const string& name, bool isMember) : name(name), isMember(isMember) {}
    ~User() { delete bookedTicket; }

    void bookTicket(const Movie& movie, const string& showTime, int seatNumber, int numberOfTickets);
    void cancelTicket();
    double calculatePayment() const;
    void giveFeedback(const Movie& movie, const string& feedback);
    int selectShowTime(const vector<string>& showTimes);

    const string& getName() const { return name; }
    Booking* getBookedTicket() const { return bookedTicket; }

private:
    string name;
    bool isMember;
    Booking* bookedTicket = nullptr;
};

void User::bookTicket(const Movie& movie, const string& showTime, int seatNumber, int numberOfTickets) {
    if (bookedTicket) {
        delete bookedTicket; // Release previous booking
    }
    bookedTicket = new Booking(movie, showTime, seatNumber, numberOfTickets);
    cout << "\n" << string(50, '*') << endl; // Stars in the middle of the screen
    cout << "Ticket booked for " << movie.getTitle() << " at " << showTime
         << ", Seat: " << seatNumber << ", Number of Tickets: " << numberOfTickets << endl;

    // Check if today is a special day (for example, Christmas)
    bool isSpecialDay = true; // Assume today is a special day
    if (isSpecialDay) {
        double discount = 0.1; // 10% additional discount
        double totalDiscount = discount * calculatePayment();
        cout << "Congratulations! An additional 10% discount has been applied for you today." << endl;
        cout << "Total Payment after discount: $" << fixed << setprecision(2) << calculatePayment() - totalDiscount << endl;
    }
    cout << string(50, '*') << "\n" << endl; // Stars in the middle of the screen
}

int User::selectShowTime(const vector<string>& showTimes) {
    cout << "Select Show Time:" << endl;
    for (int i = 0; i < showTimes.size(); ++i) {
        cout << i + 1 << ". " << showTimes[i] << endl;
    }

    int choice;
    cout << "Enter your choice (1, 2, or 3): ";
    cin >> choice;

    if (choice < 1 || choice > 3) {
        cout << "Invalid choice. Defaulting to Show Time 1." << endl;
        return 1;
    }

    return choice;
}

void User::cancelTicket() {
    if (bookedTicket && !bookedTicket->isCanceled()) {
        bookedTicket->cancelBooking();
        cout << "Booking canceled for " << bookedTicket->getMovie().getTitle()
             << " at " << bookedTicket->getShowTime() << ", Seat: " << bookedTicket->getSeatNumber() << endl;
    }
    else {
        cout << "No active booking to cancel." << endl;
    }
}

double User::calculatePayment() const {
    if (bookedTicket) {
        double moviePrice = 12.0; // Fixed ticket price
        double totalPayment = moviePrice * bookedTicket->getNumberOfTickets();

        // Apply discounts based on conditions
        if (isMember) totalPayment *= 0.9; // 10% member discount

        return totalPayment;
    }
    return 0.0; // No booking, no payment
}

void User::giveFeedback(const Movie& movie, const string& feedback) {
    // Store feedback somewhere (e.g., in a data structure within Movie)
    cout << "Feedback submitted for " << movie.getTitle() << ": " << feedback << endl;
}

void displaySeats() {
    cout << "  -----------------------------  " << endl;
    for (int row = 0; row < 10; ++row) {
        cout << 10 - row << " | ";
        for (int col = 0; col < 10; ++col) {
            cout << setw(2) << right << (row * 10 + col + 1) << " ";
        }
        cout << endl;
    }
    cout << "  -----------------------------" << endl;
    cout << "  A  B  C  D  E  F  G  H  I  J " << endl;
}

bool compareMovies(const Movie& movie1, const Movie& movie2) {
    return movie1.getTitle() < movie2.getTitle();
}

int main() {
    // Creating movie instances
    Movie movie1("Barbie", 120);
    Movie movie2("Oppenheimer", 110);
    Movie movie3("Animal", 120);
    Movie movie4("Spider-Man: Across the Spider-Verse", 120);
    Movie movie5("Home For Rent", 120);

    //sorted movie order
    vector<Movie> movies = {movie1, movie2, movie3, movie4, movie5};
    sort(movies.begin(), movies.end(), compareMovies);
    cout << "Available movies in sorted order" << endl;
    for (int i = 0; i < movies.size(); ++i) {
        cout << i + 1 << ". " << movies[i].getTitle() << " (" << movies[i].getDuration() << " min)" << endl;
    }  

    Pantry userPantry;

    cout << "Welcome to the Pantry! Please choose from the following items before booking the tickets" << endl;
    cout << "1. Chips" << endl;
    cout << "2. Juice" << endl;
    cout << "3. Popcorn" << endl;
    int userChoicefood;
    do {
        cout << "Enter your choice (1, 2, or 3), or enter 0 to finish: ";
        cin >> userChoicefood;

        switch (userChoicefood) {
            case 1:
                userPantry.addToChoicesQueue("Chips");
                break;
            case 2:
                userPantry.addToChoicesQueue("Juice");
                break;
            case 3:
                userPantry.addToChoicesQueue("Popcorn");
                break;
            case 0:
                cout << "Thank you for choosing! Here are your selections:" << endl;
                userPantry.displayUserChoices();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (userChoicefood != 0);

    // Creating a map to store show times for each movie
    map<string, vector<string>> showTimes;
    showTimes[movie1.getTitle()] = { "10:00 AM", "02:00 PM", "06:00 PM" };
    showTimes[movie2.getTitle()] = { "11:30 AM", "03:30 PM", "07:30 PM" };
    showTimes[movie3.getTitle()] = { "10:00 AM", "02:00 PM", "06:00 PM" };
    showTimes[movie4.getTitle()] = { "11:30 AM", "03:30 PM", "07:30 PM" };
    showTimes[movie5.getTitle()] = { "10:00 AM", "02:00 PM", "06:00 PM" };

    string userName;
    cout << "Welcome to the Movie Booking System!" << endl;
    cout << "Please enter your name: ";
    cin.ignore(); // Ignore the newline character in the buffer
    getline(cin, userName);

    User user(userName, true); // Assume the user is a member

    // Display all available movies
    cout << "Available movies:" << endl;
    for (int i = 0; i < movies.size(); ++i) {
        cout << i + 1 << ". " << movies[i].getTitle() << " (" << movies[i].getDuration() << " min)" << endl;
    }

    int movieChoice;
    cout << "Enter the number of the movie you want to watch: ";
    cin >> movieChoice;

    // Check if the movie choice is valid
    if (movieChoice < 1 || movieChoice > movies.size()) {
        cout << "Invalid movie choice." << endl;
        return 1;
    }

    string selectedMovieTitle = movies[movieChoice - 1].getTitle();

    // Display available show times
    cout << "Available show times for " << selectedMovieTitle << ":" << endl;
    int chosenShowTimeIndex = user.selectShowTime(showTimes[selectedMovieTitle]);

    // Display available seats
    cout << "Available Seats for " << selectedMovieTitle << ":" << endl;
    displaySeats();

    int seatNumber;
    cout << "Enter the seat number you want to book: ";
    cin >> seatNumber;

    int numberOfTickets;
    cout << "Enter the number of tickets you want to buy: ";
    cin >> numberOfTickets;

    // Book the ticket
    user.bookTicket(movies[movieChoice - 1], showTimes[selectedMovieTitle][chosenShowTimeIndex - 1], seatNumber, numberOfTickets);

    // Display booking details with decorative elements
    cout << "\n" << string(80, '*') << endl; // Stars in the middle of the screen
    cout << setw(50) << " Booking Details: " << endl;
    cout << setw(50) << " User: " << user.getName() << endl;
    cout << setw(50) << " Movie: " << user.getBookedTicket()->getMovie().getTitle() << endl;
    cout << setw(50) << " Show Time: " << user.getBookedTicket()->getShowTime() << endl;
    cout << setw(50) << " Seat Number: " << user.getBookedTicket()->getSeatNumber() << endl;
    cout << setw(50) << " Number of Tickets: " << user.getBookedTicket()->getNumberOfTickets() << endl;
    cout << setw(50) << " Total Payment: $ " << fixed << setprecision(2) << user.calculatePayment() << endl;
    cout << string(80, '*') << "\n" << endl; // Stars in the middle of the screen

    // Provide options for further interaction
    int userChoice;
    do {
        cout << "\nOptions:" << endl;
        cout << "1. Cancel Ticket" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1:
                user.cancelTicket();
                break;
            case 2:
                cout << " Dear Customer! " << endl;
                cout << " Your Ticket Has Been Booked " << endl;
                cout << " Thank you for using the Movie Booking System! " << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (userChoice != 2);

    // Clean up dynamically allocated memory
    delete user.getBookedTicket();

    return 0;
}

