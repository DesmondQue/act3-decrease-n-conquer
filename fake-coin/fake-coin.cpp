#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

bool checkEquality(int coins[], int left, int right);
int weighOdd(int coins[], int left, int right, int itr);
int weighEven(int coins[], int left, int right, int itr);

bool checkEquality(int coins[], int left, int right){
    for(int i = left; i <= right; i++){
        if(coins[i] != coins[left]) // Found fake weight
            return false;
    }
    return true; // If all weights are real
}

int weighOdd(int coins[], int left, int right, int itr){
    int numCoins = right - left + 1; // No. of coins in current iteration

    if(numCoins == 1) // Base case if there's only 1 coin left to weigh
        return left;
    
    int partition = numCoins / 3; // Splits the odd amount of coins into 3 groups (left, right, unused)
    int remainder = numCoins % 3; // To handle coins of different odd values

    // To ensure left and right are split equally, while unused group is whatever the remaining coins are
    int leftEnd = left + partition - 1 + (remainder != 0 ? 1 : 0); // Sets 1/3 of the coins as left side
    int rightEnd = leftEnd + partition + (remainder == 0 ? 0 : 1); // Sets middle group as right side, unused group is set aside

    int grpSize = (numCoins / 3) + 1;
    int unusedSize = right - rightEnd;

    int leftWeight = 0, rightWeight = 0;

    // To demonstrate process
    cout << "\n========================================\n";
    cout << "Attempt " << itr << " || Balancing Odd Numbers" << endl;
    cout << "Coin Weights: ";
    for(int i = left; i <= right; i++){
        cout << coins[i] << " ";
    }
    cout << "\n----------------------------------------\n";

    cout << "Left Group: "; // Displays and adds all weights of the left side
    for(int i = left; i <= leftEnd; i++){
        leftWeight += coins[i];
        cout << coins[i] << " ";
    }
    
    cout << "\nRight Group: "; // Displays and adds all weights of the right side
    for(int i = leftEnd + 1; i <= rightEnd; i++){
        rightWeight += coins[i];
        cout << coins[i] << " ";
    }

    cout << "\nUnused Group: "; // Displays weights of the remaining unused side
    for(int i = rightEnd + 1; i <= right; i++){
        cout << coins[i] << " ";
    }
    
    // Compares which side is heavier, if left and right are equal then the fake weight is at the unused group
    if (leftWeight < rightWeight){
        cout << "\nThe fake coin is in the left group." << endl;
        return (grpSize % 2 == 0) ? weighEven(coins, left, leftEnd, itr+1) : weighOdd(coins, left, leftEnd, itr+1);
    } else if (leftWeight > rightWeight){
        cout << "\nThe fake coin is in the right group." << endl;
        return (grpSize % 2 == 0) ? weighEven(coins, leftEnd + 1, rightEnd, itr+1) : weighOdd(coins, leftEnd + 1, rightEnd, itr+1);
    } else {
        cout << "\nThe fake coin is in the unused group." << endl;
        return (unusedSize % 2 == 0) ? weighEven(coins, rightEnd + 1, right, itr+1) : weighOdd(coins, rightEnd + 1, right, itr+1);
    }
}

int weighEven(int coins[], int left, int right, int itr){
    int numCoins = right - left + 1; // No. of coins in current iteration

    if (numCoins == 1) // Base case if there's only 1 coin left to weigh
        return left;
    
    int mid = left + (numCoins-1) / 2; // Splits the even amount of coins into 2
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    int leftWeight = 0, rightWeight = 0;
    
    // To demonstrate process
    cout << "\n========================================\n";
    cout << "Attempt " << itr << " || Balancing Even Numbers" << endl;
    cout << "Coin Weights: ";
    for(int i = left; i <= right; i++){
        cout << coins[i] << " ";
    }
    cout << "\n----------------------------------------\n";

    cout << "Left Group: "; // Displays and adds all weights of the left side
    for(int i = left; i <= mid; i++){
        leftWeight += coins[i];
        cout << coins[i] << " ";
    }
    cout << "\nRight Group: "; // Displays and adds all weights of the right side
    for(int i = mid + 1; i <= right; i++){
        rightWeight += coins[i];
        cout << coins[i] << " ";
    }

    // Compares which side is heavier
    if(leftWeight < rightWeight){
        cout << "\nThe fake coin is in the left group." << endl;
        return (leftSize % 2 == 0) ? weighEven(coins, left, mid, itr+1) : weighOdd(coins, left, mid, itr+1);
    } else {
        cout << "\nThe fake coin is in the right group." << endl;
        return (rightSize % 2 == 0) ? weighEven(coins, mid + 1, right, itr+1) : weighOdd(coins, mid + 1, right, itr+1);
    }
}

int main(){
    srand(time(0));
    char choice;
    bool reset = true;

    do{ // To reset the program if user wants to
        int N;
        cout << "Enter number of coins to generate: ";
        while(!(cin >> N) || N <= 0){ // Handle invalid inputs
            cout << "Invalid input, try again." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Enter number of coins to generate: ";
        }

        if(N == 1){ // If user enters 1 coin to be generated
            cout << "\nThere is only one coin in the group, cannot weigh.\n\n";
            continue;
        }

        int coins[N];
        int realWeight = 10 + rand() % 11; // Generate a fixed number that ranges 10 - 20
        int insertFake = rand() % 3; // 2/3 chance of a counterfeit coin existing
        int fakeWeight = realWeight - 1; // Fake weight is -1 of the real weight
        int fakeIndex;
        
        for(int i = 0; i < N; i++){ // Insert real weights to all coins
            coins[i] = realWeight;
        }

        if(insertFake != 2) // Insert fake weight to a random index within the array of coins
            coins[rand() % N] = fakeWeight;

        cout << "\nCoin Weights: "; // Display all coins generated
        for(int i = 0; i < N; i++){
            cout << coins[i] << " ";
        }
        cout << endl;
        
        bool isEqual = checkEquality(coins, 0, N-1); // To verify if all weights are equal or not
        if(isEqual){ // If there is no fake coin generated
            cout << "\nThere are no fake coins in this group, all are of equal weight.\n\n";
            continue;
        }

        // If user generates an odd or even amount of coins
        if(N % 2 == 0)
            fakeIndex = weighEven(coins, 0, N-1, 1);
        else
            fakeIndex = weighOdd(coins, 0, N-1, 1);

        cout << "\nFake coin found at index: " << fakeIndex << endl;
        
        do{ // User may choose whether or not to do another
            cout << "\nWould you like to do another? [Y/N]: ";
            cin >> choice;
            switch(choice){
                case 'y':
                case 'Y':
                    system("cls");
                    break;
                case 'n':
                case 'N':
                    reset = false;
                    break;
                default:
                    cout << "Invalid input, try again." << endl;
            }
        }while(choice != 'Y' && choice != 'N' && choice != 'y' && choice != 'n');
    }while(reset);
    return 0;
}