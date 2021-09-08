#include <iostream>
#include <math.h>
#include <cstdarg>
#include <queue>
#include <fstream>

using namespace std;

double lambda, mu;
double systemTime;
double finalTime;

/* Function to choose a queue randomly with equal probability */
int selectQueueRandomly() {
    int queueNumber = rand() % 2;
    return queueNumber;
}


/* Function to generate an exponential random variable */
long double generateExponentialRV(long double x) {
    long double uniformRV = rand() / (RAND_MAX + 1.0);
    return -log(1-uniformRV)/x;
}

/* Returns the server which is free or more closer to getting free */
int findFreeServer(double lastDeparted[]) {
    int server;
    if(lastDeparted[0] < lastDeparted[1])
        server = 0;
    else   
        server = 1;
    return server;
}

/* Workqueue class represents an individual queue in system */
class WorkerQueue {
    public:
        long int arrivedWorkers;
        long int departedWorkers;
        long int waitedWorkers;
        long double lastArrivalTime;
        long double lastDepartedTime;
        int workersInSystem;
        int waitingWorkers;
        long double totalWaitingTime;
        long double totalResponseTime;
        queue<pair<double, double>> wqueue;                     // queue stores the arrival and service time of each process/worker

        // Constructor for WorkerQueue
        WorkerQueue() {
            this->arrivedWorkers = 0;
            this->departedWorkers = 0;
            this->waitedWorkers = 0;
            this->waitingWorkers = 0;
            this->lastArrivalTime = 0;
            this->lastDepartedTime = 0;
            this->workersInSystem = 0;
            this->waitingWorkers = 0;
            this->totalWaitingTime = 0;
            this->totalResponseTime = 0;
        }

        // adds a new worker to the system
        void addWorker() {
            this->arrivedWorkers++;
            this->workersInSystem++;
            
            // compute the service time for current worker
            long double serviceTime = generateExponentialRV(mu);
            long double completionTime;
            long double waitingTime;
            long double responseTime;

            // if no worker in system or server is free
            if(wqueue.empty() || systemTime > wqueue.back().second) {
                completionTime = systemTime + serviceTime; 
                waitingTime = 0;
                responseTime = 0;
            }

            // if the system already has some workers
            else {
                completionTime = wqueue.back().second + serviceTime;
                waitingTime = completionTime - systemTime - serviceTime;
                responseTime = waitingTime - serviceTime;
                this->waitedWorkers += wqueue.size();
            }

            wqueue.push({systemTime, completionTime});

            this->totalWaitingTime += waitingTime;
            this->totalResponseTime += responseTime;
            this->lastDepartedTime = (this->wqueue.front()).second;

            // compute the interarrival time using exponential distribution
            long double interArrivalTime = generateExponentialRV(lambda);
            systemTime += interArrivalTime;
        }

        // removes a worker from system
        void removeWorker() {
            this->workersInSystem--;
            this->departedWorkers++;
            this->wqueue.pop();
        }
};

/* Initialise queues with a single worker */
void initialiseQueues(int numberOfArguments, ...) {
    WorkerQueue *currQueue;
    va_list valist;
    va_start(valist, numberOfArguments);

    for(int i=0; i<numberOfArguments; i++) {
        currQueue = va_arg(valist, WorkerQueue *);
        currQueue->addWorker();
    }
}


void computeSimulationResultsCaseA(int numberOfArguments, ...) {
    WorkerQueue *currQueue;
    va_list valist;
    va_start(valist, numberOfArguments);

    long double finalWaitingTime=0, finalResponseTime=0;
    long int finalWorkerCount = 0;
    long int finalDepartedWorkers = 0;
    long int finalWaitedWorkers = 0;

    for(int i=0; i<numberOfArguments; i++) {
        currQueue = va_arg(valist, WorkerQueue *);
        finalWaitingTime += currQueue->totalWaitingTime;
        finalResponseTime += currQueue->totalResponseTime;
        finalWorkerCount += currQueue->arrivedWorkers;
        finalDepartedWorkers += currQueue->departedWorkers;
        finalWaitedWorkers += currQueue->waitedWorkers;
    }

    cout << "\n\n------------------------------------------------------------------------------------------------------\n";
    cout << "Average number of workers getting checked = " << (long double)finalDepartedWorkers/finalTime << endl;
    cout << "Average response time for workers getting checked = " << (long double)finalResponseTime/finalWorkerCount << endl;
    cout << "Average waiting time a worker has to wait until getting checked = " << ((long double)finalWaitingTime)/finalWorkerCount << endl;
    cout << "Average number of workers in queue " << (long double)finalWaitedWorkers/(2*finalTime) << endl;
    cout << "------------------------------------------------------------------------------------------------------\n\n";

    ofstream fout;
    fout.open("CaseA_output.txt", ios::out);

    fout << "\n\n------------------------------------------------------------------------------------------------------\n";
    fout << "Average number of workers getting checked = " << (long double)finalDepartedWorkers/finalTime << endl;
    fout << "Average response time for workers getting checked = " << (long double)finalResponseTime/finalWorkerCount << endl;
    fout << "Average waiting time a worker has to wait until getting checked = " << ((long double)finalWaitingTime)/finalWorkerCount << endl;
    fout << "Average number of workers in queue " << (long double)finalWaitedWorkers/(2*finalTime) << endl;
    fout << "------------------------------------------------------------------------------------------------------\n\n";
   
    fout.close();
}


void computeSimulationResultsCaseC(int numberOfArguments, ...) {
    WorkerQueue *currQueue;
    va_list valist;
    va_start(valist, numberOfArguments);

    long double finalWaitingTime=0, finalResponseTime=0;
    long int finalWorkerCount = 0;
    long int finalDepartedWorkers = 0;
    long int finalWaitedWorkers = 0;

    for(int i=0; i<numberOfArguments; i++) {
        currQueue = va_arg(valist, WorkerQueue *);
        finalWaitingTime += currQueue->totalWaitingTime;
        finalResponseTime += currQueue->totalResponseTime;
        finalWorkerCount += currQueue->arrivedWorkers;
        finalDepartedWorkers += currQueue->departedWorkers;
        finalWaitedWorkers += currQueue->waitedWorkers;
    }

    cout << "\n\n------------------------------------------------------------------------------------------------------\n";
    cout << "Average number of workers getting checked = " << (long double)finalDepartedWorkers/finalTime << endl;
    cout << "Average response time for workers getting checked = " << (long double)finalResponseTime/finalWorkerCount << endl;
    cout << "Average waiting time a worker has to wait until getting checked = " << ((long double)finalWaitingTime)/finalWorkerCount << endl;
    cout << "Average number of workers in queue " << (long double)finalWaitedWorkers/(2*finalTime) << endl;
    cout << "------------------------------------------------------------------------------------------------------\n\n";

    ofstream fout;
    fout.open("CaseC_output.txt", ios::out);

    fout << "\n\n------------------------------------------------------------------------------------------------------\n";
    fout << "Average number of workers getting checked = " << (long double)finalDepartedWorkers/finalTime << endl;
    fout << "Average response time for workers getting checked = " << (long double)finalResponseTime/finalWorkerCount << endl;
    fout << "Average waiting time a worker has to wait until getting checked = " << ((long double)finalWaitingTime)/finalWorkerCount << endl;
    fout << "Average number of workers in queue " << (long double)finalWaitedWorkers/(2*finalTime) << endl;
    fout << "------------------------------------------------------------------------------------------------------\n\n";
   
    fout.close();
}


void simulateCaseA(long double lambda, long double mu) {
    srand(time(0));
    systemTime = 0;
    WorkerQueue firstQueue, secondQueue;                        // create two queues for each of the two officers
    WorkerQueue *currentQueue;                                  

    initialiseQueues(2, &firstQueue, &secondQueue);             // initialise both queues with a worker
    
    // iterate till end of time
    while(systemTime < finalTime) {

        if(firstQueue.workersInSystem == 0) {                   // add a worker to first queue if it becomes empty in middle of simulation
            firstQueue.addWorker();
        }
        if(secondQueue.workersInSystem == 0) {                  // add a worker to second queue if it becomes empty in middle of simulation
            secondQueue.addWorker();
        }

        if(selectQueueRandomly() == 0) {                        // select any one queue randomly with equal probabilty
            currentQueue = &firstQueue;
        }
        else {
            currentQueue = &secondQueue;
        }

        if(systemTime < currentQueue->lastDepartedTime) {       // if current worker arrives and server is not free add worker to queue
            currentQueue->addWorker();
        }
        else {
            currentQueue->removeWorker();                       // if current worker arrives and server is free immidiately process and remove worker from system
        }
    }

    computeSimulationResultsCaseA(2, &firstQueue, &secondQueue);
}


void simulateCaseB(long double lambda, long double mu) {
    srand(time(0));
    systemTime = 0;

    WorkerQueue q;                          // create only one queue for both officers
    
    initialiseQueues(1, &q);                // initialise queue with one worker

    long int workersArrived = 0;
    long int workersDeparted = 0;
    double lastDepartedTime[2] = {0, 0};
    double totalServiceTime=0, totalResponseTime=0;

    // iterate till end of simulation time
    while(systemTime < finalTime) {
        if(q.workersInSystem == 0) {                                          
            int chosenServer = findFreeServer(lastDepartedTime);                // find a server who is free or is closer to getting free
            if(systemTime > lastDepartedTime[chosenServer]) {                   // if current worker arrives after chosen server is free
                workersArrived++;
                long double serviceTime = generateExponentialRV(mu);
                lastDepartedTime[chosenServer] += serviceTime;
                totalServiceTime += serviceTime;
                totalResponseTime += serviceTime;
                long double interArrivalTime = generateExponentialRV(lambda);
                systemTime += interArrivalTime;
            }
            else {                                                              // if current worker arrives before chosen server is free, add it to queue
                q.addWorker();
                long double serviceTime = generateExponentialRV(mu);
                totalServiceTime += serviceTime;
            }
        }  
        else {
            int chosenServer = findFreeServer(lastDepartedTime);
            if(systemTime > lastDepartedTime[chosenServer]) {
                workersArrived++;
                lastDepartedTime[chosenServer] += q.wqueue.front().second;
                totalResponseTime += (lastDepartedTime[chosenServer] - q.wqueue.front().first);
                q.removeWorker();
                workersDeparted++;
            }
            else {
                q.addWorker();
                long double serviceTime = generateExponentialRV(mu);
                totalServiceTime += serviceTime;
            }
        } 
    }

    cout << "\n\n------------------------------------------------------------------------------------------------------\n";
    cout << "Average number of workers getting checked = " << (long double)workersArrived/finalTime << endl;
    cout << "Average response time for workers getting checked = " << (long double)totalResponseTime/workersArrived << endl;
    cout << "Average waiting time a worker has to wait until getting checked = " << ((long double)(totalResponseTime - totalServiceTime))/workersDeparted << endl;
    cout << "Average number of workers in queue " << (long double)totalResponseTime/finalTime << endl;
    cout << "------------------------------------------------------------------------------------------------------\n\n";

    ofstream fout;
    fout.open("CaseB_output.txt", ios::out);

    fout << "\n\n------------------------------------------------------------------------------------------------------\n";
    fout << "Average number of workers getting checked = " << (long double)workersArrived/finalTime << endl;
    fout << "Average response time for workers getting checked = " << (long double)totalResponseTime/workersArrived << endl;
    fout << "Average waiting time a worker has to wait until getting checked = " << ((long double)(totalResponseTime - totalServiceTime))/workersDeparted << endl;
    fout << "Average number of workers in queue " << (long double)totalResponseTime/finalTime << endl;
    fout << "------------------------------------------------------------------------------------------------------\n\n";
   
    fout.close();
    
}


void simulateCaseC(long double lambda, long double mu) {
    srand(time(0));
    systemTime = 0;

    WorkerQueue firstQueue, secondQueue;                    // Two special worker queues with only 5 chairs each
    WorkerQueue *currentQueue;                               

    initialiseQueues(2, &firstQueue, &secondQueue);        // initialise the queues with some worker

    long int droppedPackets = 0;                           // keeps a count of the number of workers dropped from the system
    
    // iterate till the simulation time ends
    while(systemTime < finalTime) {

        if(firstQueue.workersInSystem == 0) {               // add a worker to first queue if it becomes empty in middle of simulation
            firstQueue.addWorker();
        }
        if(secondQueue.workersInSystem == 0) {              // add a worker to second queue if it becomes empty in middle of simulation
            secondQueue.addWorker();
        }

        if(selectQueueRandomly() == 0) {                    // select any one queue randomly with equal probabilty
            currentQueue = &firstQueue;
        }
        else {
            currentQueue = &secondQueue;
        }

    
        if(systemTime < currentQueue->lastDepartedTime) {           // if current worker arrives and server is not free add worker to queue
            currentQueue->addWorker();
            if(currentQueue->wqueue.size() >= 5) {                  // check if queue's limit of 5 has been reached
                int droppingChance = rand() % 100;                  
                if(droppingChance != 0 && droppingChance != 1) {    // worker is sent to main queue with 0.02 probabilty else it leaves the system
                    droppedPackets++;
                }
            }
        }
        else {
            currentQueue->removeWorker();                           // if current worker arrives and server is free immidiately process and remove worker from system
        }
    }

    //int totalWorkersArrived = firstQueue.arrivedWorkers + secondQueue.arrivedWorkers;
    //cout << "\nTotal number of workers arrived: " << totalWorkersArrived << endl;
    //cout << "Total workers dropped: " << droppedPackets << endl;

    computeSimulationResultsCaseC(2, firstQueue, secondQueue);
}   

/* This function calls the appropriate method to start simulation based on the case */
void startSimulation(long double lambda, long double mu, char caseType) {
    if(caseType == 'A') {
        simulateCaseA(lambda, mu);
    }    
    else if(caseType == 'B') {
        simulateCaseB(lambda, mu);
    }
    else {
        simulateCaseC(lambda, mu);
    }
}

// Function to check the stability condition in the queueing system
inline bool checkStability(long double lambda, long double mu) {
    return (lambda > mu);
}


void displayInputMessage() {
    cout << "Please choose from following cases:\n";
    cout << "Case A: There are 2 separate queues for 2 officers respectively.\n";
    cout << "Case B: There is only one queue for 2 officers.\n";
    cout << "Case C: There is a special queue with 5 chairs for aged/unwell/pregnant workers.\n";
    cout << "Press A/B/C for simulating any one of the above cases. Press E to exit the simulation: ";
}


int main(void) {
    char userChoice;

    do {
        displayInputMessage();
        
        cin >> userChoice;

        cout << endl; 

        switch (userChoice)
        {
        case 'A':
        case 'a':
            cout << "Enter mean arrival rate, lambda: ";
            cin >> lambda;
            cout << "Enter mean service rate, mu: ";
            cin >> mu;
            cout << "Enter simulation time = ";
            cin >> finalTime;
            if(checkStability(lambda, mu) == true) {
                cerr << "Values entered are incorrect. Stability condition violated.\n\n ";
                break;
            }
            else {
                startSimulation(lambda, mu, 'A');
            }
            break;

        case 'B':
        case 'b':
            cout << "Enter mean arrival rate, lambda: ";
            cin >> lambda;
            cout << "Enter mean service rate, mu: ";
            cin >> mu;
            cout << "Enter simulation time = ";
            cin >> finalTime;
            if(checkStability(lambda, mu) == true) {
                cerr << "Values entered are incorrect\n";
                break;
            }
            else {
                startSimulation(lambda, 2*mu, 'B');
            }
            break;

        case 'C':
        case 'c':
            cout << "Enter mean arrival rate, lambda: ";
            cin >> lambda;
            cout << "Enter mean service rate, mu: ";
            cin >> mu;
            cout << "Enter simulation time = ";
            cin >> finalTime;
            if(checkStability(lambda, mu) == true) {
                cerr << "Values entered are incorrect\n";
                break;
            }
            else {
                startSimulation(lambda, mu, 'C');
            }
            break;

        case 'E':
        case 'e':
            exit(0);
        
        default:
            cerr << "Please enter a valid choice.\n";
            break;
        }
    } while(true);

    return 0;
}