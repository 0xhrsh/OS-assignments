#include <bits/stdc++.h>
using namespace std;

sem_t semSTDOUT;

class Customer{
    public:
        int id;
        Customer(int c_id){
            id = c_id;
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" arrives at the barbershop\n";
            sem_post(&semSTDOUT);
        }
        void enterShop(){
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" enters the barbershop\n";
            sem_post(&semSTDOUT);
        }

        void sitOnSofa(){
            // couchQ.push(id);
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" sits in the waiting room\n";
            sem_post(&semSTDOUT);
        }

        void sitInBarberChair(int barberID){
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" sits on barber chair "<<barberID<<" to get a haircut\n";
            sem_post(&semSTDOUT);
        }
        void waitforPayment(){
            // wait here
        }
        void pay(int barberID){
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" leaves barber chair "<<barberID<<" to pay\n";
            sem_post(&semSTDOUT);
        }
        void leaveShop(){
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" leaves the barbershop after paying for the service\n";
            sem_post(&semSTDOUT);
        }
        void exitShop(){
            sem_wait(&semSTDOUT);
            cout<<"Customer "<<id<<" exits the barbershop\n";
            sem_post(&semSTDOUT);
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
            sem_wait(&semSTDOUT);
            cout<<"Barber "<<id<<" is sleeping, waiting for customer\n";
            sem_post(&semSTDOUT);
        }

        void cutHair(int customerID){
            sem_wait(&semSTDOUT);
            cout<<"Barber "<<id<<" starts haircut of customer "<<customerID<<"\n";
            sem_post(&semSTDOUT);
            std::this_thread::sleep_for (std::chrono::seconds(1));
            sem_wait(&semSTDOUT);
            cout<<"Barber "<<id<<" finishes haircut of customer "<<customerID<<"\n";
            sem_post(&semSTDOUT);
        }

        void cleanChair(){
            std::this_thread::sleep_for (std::chrono::seconds(1));
            sem_wait(&semSTDOUT);
            cout<<"Barber "<<id<<" is cleaning.\n";
            sem_post(&semSTDOUT);
        }
        void acceptPayment(int customerID){
            std::this_thread::sleep_for (std::chrono::seconds(1));
            sem_wait(&semSTDOUT);
            cout<<"Barber "<<id<<" recieve payment from customer "<<customerID<<"\n";
            sem_post(&semSTDOUT);
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
            sem_wait(&semSTDOUT);
            cout<<"customer "<<customerID<<" takes token\n";
            sem_post(&semSTDOUT);
        }

        void verifyPayment(int customerID){
            sem_wait(&semSTDOUT);
            cout<<"The cashier has received payment from customer: "<<customerID<<endl;
            sem_post(&semSTDOUT);
        }

        void takeToken(int customerID){
            tokens_issued--;
            sem_wait(&semSTDOUT);
            cout<<"The customer "<<customerID<<" submits the token to the gatekeeper.\n";
            sem_post(&semSTDOUT);
        }
};
