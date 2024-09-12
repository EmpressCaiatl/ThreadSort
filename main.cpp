/*
 * Name: [Isabella]
 * Description: This sort algorithm uses insertion sort and merge sort along with multithreading 
                to sort a given list with a given amount of threads.
 * Input: Takes in two integers from user to determine list size and amount of threads used, must be powers of 2
 * Output: Sorted list of user size.
 */

#include <iostream>     //access to normal function
#include <vector>       //access to use of vectors and its corresponding functions
#include <thread>       //access to threading functions
#include <algorithm>    //access to sort();

using namespace std;

//GLOBAL VECTOR
vector<int> meeSeeks;

//FUNCTIONS
void insertionSort(int left, int right);
void mergeLists(int subOneLeft, int subOneRight, int subTwoLeft, int subTwoRight);
void printList(vector<int> copy);

/*MAIN*/
int main()
{
    int listSize = 0;     //size of list that will be sorted
    int numThreads= 0;    // amount of threads user wants to be used
    bool goodRead = false;

    int maxThreads = thread::hardware_concurrency();   //for computing how many threads the users computer can handle

    /*Read in*/ 
    while (!goodRead) {

        cout << "Please enter an integer for the size: ";
        cin >> listSize;

        if (cin.fail()) {
            cout << "Error: Invalid input. Try again." << endl;
            goodRead = false;
            break;
        }
        else {
            goodRead = true;
        }

        cout << "Please enter an integer for the number of threads: ";
        cin >> numThreads;

        if(numThreads > maxThreads){
            goodRead = false;
            cout << "Error: Max threads reached! Try a smaller amount of threads." << endl;
            break;
        }
        else {
            continue;
        }

        if (cin.fail()) {
            cout << "Error: Invalid input. Try again." << endl;
            goodRead = false;
            break;
        }
        else {
            goodRead = true;
        }
      
    }

    /*OUTPUT INFORMATION*/
    cout << endl;
    cout << "List Size: " << listSize << endl;
    cout << "Num Threads: " << numThreads << endl;
    cout << "Sublist Size: " << listSize / numThreads << endl;
    cout << endl;

    /*POPULATE LIST*/
    for (int x = 0; x < listSize; x++) {

        int insert = rand() % listSize + 1;
        meeSeeks.push_back(insert);
        
    }

    /*PRINT UNSORTED LIST*/
    cout << "Unsorted: ";
    printList(meeSeeks);

    vector<thread> threadPool;
    int subList = listSize / numThreads;    //sublist size
    int left = 0;                           //left index
    int right = 0;                          //right index
    int count = 0;                          //keeps track of location in indices

    /*INSERTION SORT*/
    for (int x = 0; x < numThreads; x++) {
                
        while (count < numThreads) {
            right = left + subList - 1;
            if (count > 0) {
                left = right + 1;
                right = right + subList;
            }

            threadPool.push_back( thread (insertionSort, left, right));

            count++;
        }

    }

    /*JOIN THREADS*/
    for (unsigned int y = 0; y < threadPool.size(); y++) {
       threadPool[y].join();
    }
    
    //OUTPUT RESULTS
    cout << "First Sort: ";
    printList(meeSeeks);
    cout << endl;

    //DO NOT USE UNLESS SORTING IS BUSTED (probs is)
    //sort(meeSeeks.begin(), meeSeeks.end());

    count = 0;      //reset variables for use in mergesort
    left = 0;
    right = 0;
    int xleft = 0;  //extra indices for calling mergelists
    int xright = 0;

    /*MERGESORT*/
    for (int x = numThreads/2; x > 0; x = x/2) {

        threadPool.clear();
        right = -1;
        for (count = 0; count < x; count++) {

            subList = (listSize / 2) / x;

            xleft = right + 1;
            xright = xleft + subList - 1;
            left = xright + 1;
            right = left + subList - 1;
  
            threadPool.push_back(thread(mergeLists, xleft, xright, left, right )); 

        }
        //for testing without threading --->
        //mergeLists(xleft, xright, left, right);

        for (unsigned int y = 0; y < threadPool.size(); y++) {
            threadPool[y].join();
        }

    }

    //OUTPUT RESULTS
    cout << "Final Sort: ";
    printList(meeSeeks);
    cout << endl;
    
    //CLEAR THREADS
    threadPool.clear();
       
}

/*
* function_identifier: outputs list to terminal
* parameters: takes a vector of integers
* return value: no return value
*/
void printList(vector<int> copy) {
    for (int x = 0; x < copy.size(); x++) {
        cout << copy[x] << " ";
    }
    cout << endl;
    cout << endl;

}

/*
* function_identifier: performs an insertion sort on a vector within certain bounds
* parameters: takes the left and right boundaries for the vector
* return value: no return value
*/
void insertionSort(int left, int right) {

    vector<int> copy = meeSeeks;
    int i, key, j = 0;

    for (i = left + 1; i > left && i <= right; i++) {
        key = copy[i];
        j = i - 1;
        while (j >= 0 && j >= left && j <= right && copy[j] > key) {
            copy[j + 1] = copy[j];
            j = j - 1;
        }
        copy[j + 1] = key;
    }

    meeSeeks = copy;
}

/*
* function_identifier: performs a merge sort on a vector between certain boundaries
* parameters: takes four indices that determine the begin and end points for the sort
* return value: no return value
*/
void mergeLists(int subOneLeft, int subOneRight, int subTwoLeft, int subTwoRight) {

    vector<int> temp1;
    vector<int> temp2;

    for (int x = subOneLeft; x <= subOneRight; x++) {
        temp1.push_back(meeSeeks[x]);
    }
    for (int x = subTwoLeft; x <= subTwoRight; x++) {
        temp2.push_back(meeSeeks[x]);
    }

    int i = 0;
    int j = 0;
    int k = subOneLeft;

    int domainOne = subOneRight - subOneLeft + 1;
    int domainTwo = subTwoRight - subTwoLeft + 1;

    while (i < domainOne || j < domainTwo) {

        if (i > temp1.size()) {
            meeSeeks[k] = temp2[j];
            j++;
            k++;
        }

        if (j > temp2.size()) {
            meeSeeks[k] = temp1[i];
            i++;
            k++;
        }

        if (temp1[i] <= temp2[j]) {
            meeSeeks[k] = temp1[i];
            i++;
        }
        else {
            meeSeeks[k] = temp2[j];
            j++;
        }
        k++;
    }
}

