#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;
const string ClientsFileName = "Client.txt";
void LogIn();
void ShowATMmainMenueScreen();
void  ShowQuickWithDrawScreen();
void  ShowNormalWithDrawScreen();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;

};
sClient CurrentClient;
enum enATMoption
{
    eQuickwithdraw = 1, eNormalwithdraw = 2, Deposit = 3,
    CheckBalance = 4, LogOut = 5
};
void GoBackToATMmenue()
{
    cout << "\n\nPress any key to go back to Transaction menue...\n";
    system("pause>0");
    ShowATMmainMenueScreen();
}
vector <string> SplitString(string S1, string Delim)
{
    vector<string> vstring;

    short Pos = 0;
    string sWord;

    while ((Pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, Pos);
        if (sWord != " ")
        {
            vstring.push_back(sWord);
        }

        S1.erase(0, Pos + Delim.length());
    }
    if (S1 != " ")
    {
        vstring.push_back(S1);
    }

    return vstring;
}
sClient convertLineToRecord(string line, string sparater = "#//#")
{
    sClient Client;
    vector<string > vClientdata;
    vClientdata = SplitString(line, sparater);

    Client.AccountNumber = vClientdata[0];
    Client.PinCode = vClientdata[1];
    Client.Name = vClientdata[2];
    Client.Phone = vClientdata[3];
    Client.AccountBalance = stod(vClientdata[4]);
    return Client;
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = convertLineToRecord(Line);

            vClients.push_back(Client);
        }
        MyFile.close();
    }

    return vClients;
}
string ConvertRecordToLine(sClient Client, string Separater = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Separater;
    stClientRecord += Client.PinCode + Separater;
    stClientRecord += Client.Name + Separater;
    stClientRecord += Client.Phone + Separater;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}
short ReadQuickWithdrawAmount()
{
    short CHOOSE = 0;
    while (CHOOSE < 1 || CHOOSE>9)
    {
        cout << "\nChoose what to Withdraw From[1] to [9] ? ";
        cin >> CHOOSE;
    }
   
    return CHOOSE;
}
short GetAmountFromQuickW(short Quick)
{
    switch (Quick)
    {

    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    default:
        return 0;
    }
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients) 
{
    char Answer = 'n';   
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber) 
            {
                C.AccountBalance += Amount; 
                SaveCleintsDataToFile(ClientsFileName, vClients); 
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true; 
            }
        }
        return false;
    }

}
void PerFormQWithdrawOption(short QuickAmount)
{
    if (QuickAmount == 9)
    {
        return;
    }

    short WithDrawAmount = GetAmountFromQuickW(QuickAmount);

    if (WithDrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n"; 
        cout << "Press Anykey to continue...";     
        system("pause>0");    
        ShowQuickWithDrawScreen(); 
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawAmount * -1, vClients);  
    CurrentClient.AccountBalance -= WithDrawAmount;

}
void ShowQuickWithDrawScreen()
{
    system("cls");
    cout << "===================================\n";
    cout << "\tQuick WithDraw Screen\n";
    cout << "===================================\n";
    cout << "\t[1] 20\t\t[2] 50\n ";
    cout << "\t[3] 100\t\t[4] 200\n ";
    cout << "\t[5] 400\t\t[6] 600\n ";
    cout << "\t[7] 800\t\t[8] 1000\n ";
    cout << "\t[9] Exit";
    cout << "\n===================================";
    cout << "\nYour Balance is: " << CurrentClient.AccountBalance;

    PerFormQWithdrawOption(ReadQuickWithdrawAmount());

}
short ReadWithDrawAmount()
{
    int Amount;
    do
    {
        cout << "\nPlease Enter an amount Multiple Of 5's.";
        cin >> Amount;
    } while (Amount % 5 != 0);
    return Amount;
}
void PerformWithdrawOption()
{
    int WithDrawAmount = ReadWithDrawAmount();

    if (WithDrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n"; 
        cout << "Press Anykey to continue...";     
        system("pause>0");    
        ShowNormalWithDrawScreen(); 
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawAmount * -1, vClients);  
    CurrentClient.AccountBalance -= WithDrawAmount;


}
void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===================================\n";
    cout << "\tNormal WithDraw Screen\n";
    cout << "===================================\n";
    PerformWithdrawOption();
}
double ReadDepositAmount()
{
    double Amount;
    cout << "\nEnter a positive Deposit Amount? "; 
    cin >> Amount;

    while (Amount <= 0) 
    {
        cout << "\nEnter a positive Deposit Amount? ";  
        cin >> Amount;
    }

    return Amount;
}
void PerformDepositAmount()
{
    int DepositAmount = ReadDepositAmount();

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;


}
void ShowDepositScreen()
{
    system("cls");
    cout << "===================================\n";
    cout << "\tDeposit Screen\n";
    cout << "===================================\n";
    PerformDepositAmount();
}
void ShowCheckBalanceScreen()
{
    
    cout << "===================================\n";
    cout << "\tChech Balance Screen\n";
    cout << "===================================\n";

    cout << "\nYour Balance is: " << CurrentClient.AccountBalance;

}
//-------------
short ReadAtMChoose()
{
    short CHOOSE = 0;
    cout << "\nChoose what do you want to do? [1 to 5]?";
    cin >> CHOOSE;
    return CHOOSE;
}
void StartATM(enATMoption ATM)
{
    switch (ATM)
    {

    case enATMoption::eQuickwithdraw:
        system("cls");
        ShowQuickWithDrawScreen();
        GoBackToATMmenue();
        break;

    case enATMoption::eNormalwithdraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToATMmenue();
        break;

    case enATMoption::Deposit:
        system("cls");
        ShowDepositScreen();
        GoBackToATMmenue();
        break;

    case enATMoption::CheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATMmenue();
        break;

    case enATMoption::LogOut:
        LogIn();
        break;


    }
}
void ShowATMmainMenueScreen()
{
    system("cls");
    cout << "===================================\n";
    cout << "\tATM Main Menue Screen\n";
    cout << "===================================\n";
    cout << "\t[1] Quick withdraw.\n";
    cout << "\t[2] Normal withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] LogOut.\n";
    cout << "===================================\n";
    StartATM((enATMoption)ReadAtMChoose());

}
bool FindClientByAccountNumberAndPin(string AccountNum,string Pin,sClient& Client)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNum && C.PinCode == Pin)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool LoadClientInfo(string AccountNum, string Pin)
{
        if (FindClientByAccountNumberAndPin(AccountNum,Pin,CurrentClient)) 
            return true;
        else
            return false;
}
void LogIn()
{
   
    bool LoginFaild = false;
    string AccountNumber, Pin;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid AccountNumber/Pin!\n";
        }
        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin? ";
        cin >> Pin;
        LoginFaild = !LoadClientInfo(AccountNumber, Pin);

    } while (LoginFaild);

    ShowATMmainMenueScreen();



}


int main()
{
    LogIn();
    return 0;
}
