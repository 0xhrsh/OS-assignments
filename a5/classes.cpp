#include <bits/stdc++.h>
using namespace std;

sem_t semSTDOUT;

class Customer{
    public:
        int id;
        Customer(int c_id){
            id = c_id;
            cout<<"Customer "<<id<<" arrives at the barbershop\n";
        }
        void enterShop(){
            cout<<"Customer "<<id<<" enters the barbershop\n";
        }

        void sitOnSofa(){
            // couchQ.push(id);
            cout<<"Customer "<<id<<" sits in the waiting room\n";
        }

        void sitInBarberChair(int barberID){
            cout<<"Customer "<<id<<" sits on barber chair "<<barberID<<" to get a haircut\n";
        }
        void waitforPayment(){
            // wait here
        }
        void pay(int barberID){
            cout<<"Customer "<<id<<" leaves barber chair "<<barberID<<" to pay\n";
        }
        void leaveShop(){
            cout<<"Customer "<<id<<" leaves the barbershop after paying for the service\n";
        }
        void exitShop(){
            cout<<"Customer "<<id<<" exits the barbershop\n";
        }
};

class Barber{
    public:
        int id;
        Barber(int b_id){
            id = b_id;
            cerr<<"Barber "<<id<<" created\n";
        }
        void sleep(){
            cout<<"Barber "<<id<<" is sleeping, waiting for customer\n";
        }

        void cutHair(int customerID){
            cout<<"Barber "<<id<<" starts haircut of customer "<<customerID<<"\n";
            std::this_thread::sleep_for (std::chrono::seconds(1));
            cout<<"Barber "<<id<<" finishes haircut of customer "<<customerID<<"\n";
        }

        void cleanChair(){
            std::this_thread::sleep_for (std::chrono::seconds(1));
            cout<<"Barber "<<id<<" is cleaning.\n";
        }
        void acceptPayment(int customerID){
            std::this_thread::sleep_for (std::chrono::seconds(1));
            cout<<"Barber "<<id<<" recieve payment from customer "<<customerID<<"\n";
        }
};

class Gatekeeper{
    public:
        int tokens_issued;
        Gatekeeper(){
            tokens_issued = 0;
            cerr<<"Gatekeeper created\n";
        }

        void giveToken(int customerID){
            tokens_issued++;
            cout<<"customer "<<customerID<<" takes token\n";
        }

        void verifyPayment(int customerID){
            cout<<"The cashier receives payment from customer: "<<customerID<<endl;
        }

        void takeToken(int customerID){
            tokens_issued--;
            cout<<"The customer "<<customerID<<" submits the token to the gatekeeper.\n";
        }
};
