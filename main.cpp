 // referene taken from Proferssors code available on net
#include <iostream>
#include "rv.h"
#include "event.h"

int main()
{
  using namespace std;
  EventList Elist;                // Create event list
  enum {ARR,DEP_Q1,DEP_Q2};         // Define the event types

  double lambda,mu1,mu2,pH,pL; //defining variables
  double r2d,r21;					// defining departures from queue 2.                
  double clock = 0.0;             // System clock
  int N1 = 0;
  int N2=0;                      // Number of customers in system
  double a1,a2;
  int Ndep = 0;                   // Number of departures from system
  
  double EN1_H = 0.0, EN1_L=0.0;	// Expected number q1 high and q1 low
  double EN2_H =0.0, EN2_L=0.0;		// Expected number q2 high and q2 low
  int done = 0;                   // End condition satisfied?
  int Arr_1_H,Arr_1_L,Arr_2_H,Arr_2_L;	//	Q1 and Q2 arrivals for both clases
  int Dep_1_H,Dep_1_L,Dep_2_H,Dep_2_L;	//	Q1 and Q2 Departures for both clases
  double prev;
  int pri;
/*  
		clock = 0;
                prev = 0.0;
                clock = 0.0;done =0;N1=0;N2=0,Ndep = 0;
                EN1_H = 0.0;EN2_H = 0.0;
                EN1_L = 0.0; EN2_L = 0.0;
                Arr_1_H =0;     Arr_2_H=0;
                Arr_1_L=0; Arr_2_L=0;
                Dep_1_H=0; Dep_2_H=0;
                Dep_1_L=0; Dep_2_L=0;
*/ 
  
//start taking user inputs

  cout <<"Prob of High priority customer(pH):"; cin >> pH;
  pL= 1-pH;
  cout <<"Prob of Low priority customer(pL):"; cout <<pL<<endl;
  cout <<"Arrival of pH to Q1(a1):"; cin >> a1;
  a2=1-a1;
  cout <<"Arrival of pH to Q2(a2):"; cout << a2<<endl;
  cout <<"r2d:"; cin >> r2d;
  r21 = 1- r2d;
  cout <<"r21:"; cout << r21 << endl;
  cout <<"Service time at Q1 (mu1):"; cin>> mu1;
  cout <<"Service time at Q2 (mu2):"; cin>> mu2;

//End taking user inputs
  
  
  //pH=0.4; pL=0.6; a1=0.5; a2=0.5; r2d=0.25; r21=0.75; mu1=40; mu2=50;

  
  for(lambda=1.0; lambda<=10.0; lambda++){
  
		cout<<"lambda: " << lambda << endl;
		Event* CurrentEvent;
		clock = 0;
		prev = 0.0;	
		clock = 0.0;done =0;N1=0;N2=0,Ndep = 0;
		EN1_H = 0.0;EN2_H = 0.0;
		EN1_L = 0.0; EN2_L = 0.0;
		Arr_1_H =0;	Arr_2_H=0;
		Arr_1_L=0; Arr_2_L=0;
		Dep_1_H=0; Dep_2_H=0;
		Dep_1_L=0; Dep_2_L=0;

		Elist.insert(exp_rv((lambda*pH)),ARR,1);					// Generate first arrival event for highpriority 
		Elist.insert(exp_rv((lambda*pL)),ARR,2);					// Generate first arrival event for lowpriority
		
		while (!done){
			CurrentEvent = Elist.get();               // Get next Event from list
			prev = clock;                             // Store old clock value
			clock=CurrentEvent->time;                 // Update system clock 

			switch (CurrentEvent->type) {
			
			case ARR:                                 		  // If arrival event occurs
			
				EN1_H+= Arr_1_H*(clock-prev);                   //  expected number high P q1
				EN2_H+= Arr_2_H*(clock-prev);					  //  expected number high P q2
				EN1_L+= Arr_1_L*(clock-prev);                   //  expected number low P q1
				EN2_L+= Arr_2_L*(clock-prev);					  //  expected number low P q2
							  
				if(CurrentEvent->priority==1){				// implies high priority customer arrives
					  
					if(uni_rv()<=a1){  					  //implies HP customer arrives Q1						
						N1++;
						Arr_1_H++;
						if(N1!=0)
							Elist.insert(clock+exp_rv(mu1),DEP_Q1,1);
					} 
					else {  							  //implies HP customer arrives Q2						
						N2++;
						Arr_2_H++;
						if(N2!=0)
							Elist.insert(clock+exp_rv(mu2),DEP_Q2,1);
					}
					 Elist.insert(clock+exp_rv((lambda*pH)),ARR,1); //  generate next arrival
				}
				else{   					 			  // implies Low priority customer arrives
					N1++;
					Arr_1_L++;
					if(N1!=0)
						Elist.insert(clock+exp_rv(mu1),DEP_Q1,2);		
					 Elist.insert(clock+exp_rv((lambda*pL)),ARR,2); //  generate next arrival	
					}
				break;
				
/*------------------------------------------------------------------------------------------------------------------------*/
			case DEP_Q1:                                 // If dep from Q1 occurs 
			 
				pri  = CurrentEvent->priority;         //current events priority will be stored in variable pri
				EN1_H+= Arr_1_H*(clock-prev);                   
				EN2_H+= Arr_2_H*(clock-prev);
				EN1_L+= Arr_1_L*(clock-prev);                  
				EN2_L+= Arr_2_L*(clock-prev);
				  
				N1--;                   
				  
				if(pri==1){             //Priority = 1 implies High priority cust departs
					Dep_1_H++;
					Arr_1_H--;
				}
				else if(pri==2){			//Priority = 2 implies low priority cust departs
					Dep_1_L++;
					Arr_1_L--;				
				}

				if (Arr_1_H > 0)                             // If High prio customers remain in Q1 generate Dep for them
					Elist.insert(clock+exp_rv(mu1),DEP_Q1,1);   
				else if (Arr_1_L > 0)                        // If Low prio customers remain in Q1 generate Dep for them
					Elist.insert(clock+exp_rv(mu1),DEP_Q1,2);  
				  
				N2++;			//increment no of jobs in queue2
				
				if(pri==1)
					Arr_2_H++;                    
				else if(pri==2)
					Arr_2_L++;
				
				if (N2!=0)                            // If customers exist in queue2 generate depearture for them
				  Elist.insert(clock+exp_rv(mu2),DEP_Q2,pri);  
				 
				break;
				
/*------------------------------------------------------------------------------------------------------------------------*/

			case DEP_Q2:                                 // If departure from Q2 occurs
			
				 pri  = CurrentEvent->priority;         		//current events priority will be stored in variable pri			  
				EN1_H+= Arr_1_H*(clock-prev);                   //  high priority q1
				EN2_H+= Arr_2_H*(clock-prev);					// high priority q2
				EN1_L+= Arr_1_L*(clock-prev);                   //  low priority q1
				EN2_L+= Arr_2_L*(clock-prev);					// low priority q2
				
				N2--;    

				if(pri==1){										//Priority = 1 implies High priority cust departs
					Dep_2_H++;				
					Arr_2_H--;
				}
				if(pri==2){										//Priority = 2 implies High priority cust departs
					Dep_2_L++;
					Arr_2_L--;
				}

				if (Arr_2_H > 0)                             	 // If High prio customers remain in Q2 generate Dep for them
					Elist.insert(clock+exp_rv(mu2),DEP_Q2,1);  
				else if (Arr_2_L > 0)                              // If Low prio customers remain in Q1 generate Dep for them
					Elist.insert(clock+exp_rv(mu2),DEP_Q2,2);   
				 
				 
				//now need to decide whether customer leaves system or goes back to Q1 
				if (uni_rv()<r2d)     //customer departs from system                            
					Ndep++;
				else{				  //send back to Q1
				  	N1++;
					if(pri==1)					
						Arr_1_H++;
					else if (pri==2)
						Arr_1_L++;
			
					if(N1!=0)
						Elist.insert(clock+exp_rv(mu1),DEP_Q1,CurrentEvent->priority);
				}
				break;
				
				
			}   //end for switch
			
			delete CurrentEvent;
			
			if (Ndep > 500000)
				done=1;        // End condition
		} //end for while

	//theoritical values

	double Th_2H  = (lambda * pH)/(r2d) ; 
	double Th_2L  = (lambda * pL)/(r2d) ; 
	double Th_1L  = (lambda * pL)/(r2d) ; 
	double Th_1H  = (lambda * pH * a1) + (Th_2H *r21) ;

    double rho1= ((Th_1H+Th_1L)/mu1);
    double rho2= ((Th_2H+Th_2L)/mu2);
		
		
		
  // output simulation results for N, E[N] 
	cout << "SIMULATION RESULTS---------------------------" << endl;
	
  cout << "Throughput of Queue1 (High Priority): " << Dep_1_H/clock << endl ;
  cout << "(Theo )Throughput of Queue1 (High Priority): " << Th_1H << endl; 
  cout << " Throughput of Queue1 (Low Priority): " << Dep_1_L/clock << endl;
  cout << "(Theo )Throughput of Queue1 (low Priority): " << Th_1L << endl;
  cout << "Throughput of Queue2 (High Priority): " << Dep_2_H/clock << endl;
  cout << "(Theo )Throughput of Queue2 (High Priority): " << Th_2H << endl;
  cout << "  Throughput of Queue2 (Low Priority): " << Dep_2_L/clock << endl;
  cout << "(Theo )Throughput of Queue2 (low Priority): " << Th_2L << endl; 
	
  cout <<"E[n] in Queue 1: " << (EN1_H + EN1_L)/clock << endl;
  cout << "(Theo )E[n] in Queue 1:  " << (rho1/(1-rho1)) << endl;
  
  cout <<"E[n] in Queue 2: " <<(EN2_H + EN2_L)/clock << endl;
  cout << "(Theo )E[n] in Queue 2: " << (rho2/(1-rho2)) << endl;
  
  cout << "E[n] of High Priority in Queue1: " << EN1_H/clock << endl;  
  cout <<"E[n] of Low Priority in Queue1: " << EN1_L/clock << endl;    
  cout << "E[n] of High Priority in Queue2: " << EN2_H/clock << endl;
  cout << "E[n] of Low Priority in Queue2: " << EN2_L/clock << endl;

  cout << "Delay of Queue2(High Priority)"<< EN2_H/Dep_2_H << endl;
  cout << "Delay of Queue2(Low Priority)"<< EN2_L/Dep_2_L << endl;
  cout <<"---------------------------"<< endl ;
  Elist.clear();

 } // end of for

}// end of main
