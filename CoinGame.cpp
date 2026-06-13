#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib> //used for system("PAUSE") and system("CLS")
#include <ctime>   //used for rand()
#include <conio.h> //used for rand()

using namespace std;
int AUX = 0;

char MainMenu[5][201], PMenu[10][201], IMenu[17][201], menuType = 'm';
//MainMenu = Main Menu matrix
//PMenu = Menu (matrix) for game modes display
//IMenu = info Menu
//menuType = decides what Menu will show up in the console

int m = 1, p = 1, MenuChanged, arrowISright, PMove, P2starts = 0, BLFMode = 0, HIT, MaxCoinVal = 15, NBCoins = 8;
char MCVChar[3], NBCChar[3];
//m,p = arrow's position ('>') for MainMenu and PMenu respectively
//MenuChanged = used for menu changes (either arrow or menu changes)
//arrowISright = arrow's position in game (0=left; 1=right)
//PMove = checks if a human player made a move
//P2starts= checks if P2/bot starts (1=Yes; 0=No)
//BFLDMode= Blindfold mode (1=on 0=off)
//HIT= (used for 2 human player game mode) controls: A/D+E(P1) ; LEFT/RIGHT+ENTER (P2)
//MaxCoinVal = maximum coin value that can show up in a game
//NBCoins= nb. of coins in a game
//MCVChar= MaxCoinVal, char version
//NBCChar= NBCoins, char version

int c[17], P1[10], P2[10], P = 0, n;
//c[] = coin row
//P1[], P2[] = players' money bags
//P = game turn index
//n = auxiliary var; used for coin number 
int x, y, v[101], s[101], change = 2; //used in fuse() function
//v[],s[] = auxiliary vectors
//x, y = v&s lengths
//change = checks if the fusion had been made

void cit() 
{
    ///reading and creating all the menus
    ifstream f("menu.in");
    for (int i = 0;i < 5;i++) f.getline(MainMenu[i], 101); //Main Menu
    for (int i = 0;i < 10;i++) f.getline(PMenu[i], 101); //Player Menu
    for (int i = 0;i < 17;i++) f.getline(IMenu[i], 201);//Info Menu
    f.close();
}

void AF() {
    //prints the menu, depending on menuType
    if (menuType == 'm')      for (int i = 0;i < 5;i++) cout << MainMenu[i] << endl;
    else if (menuType == 'p') for (int i = 0;i < 10;i++) cout << PMenu[i] << endl;
    else if (menuType == 'i') for (int i = 0;i < 17;i++) cout << IMenu[i] << endl;
}

void NBtoCHAR(int a, char b[3]) 
{
    //diy itoa
    int aux = 0, n = a; strcpy(b, "");
    while (n > 0) { aux = aux * 10 + n % 10; n = n / 10; }
    n = aux;
    while (n > 0) 
    {
        switch (n % 10)
        {
        case 1: strcat(b, "1"); break;
        case 2: strcat(b, "2"); break;
        case 3: strcat(b, "3"); break;
        case 4: strcat(b, "4"); break;
        case 5: strcat(b, "5"); break;
        case 6: strcat(b, "6"); break;
        case 7: strcat(b, "7"); break;
        case 8: strcat(b, "8"); break;
        case 9: strcat(b, "9"); break;
        }
        n /= 10;
    }
    if (a % 10 == 0) strcat(b, "0");
}

void Logic() 
{// Menu control logic
    if (_kbhit()) //if a key is pressed
        switch (_getch()) //specific key is pressed cases:
        {
            case 'w':
            case  72: //72 = 'UP'
                // if pressed, '>' ascends
                MenuChanged = 1;
                if (menuType == 'm') { if (m != 1) { MainMenu[m][0] = ' ';MainMenu[--m][0] = '>'; } }
                else if (menuType == 'p') if (p != 1) { PMenu[p][0] = ' ';PMenu[--p][0] = '>'; }
                break;

            case 's':
            case  80: //80 = 'DOWN'
                // if pressed, '>' descendes
                MenuChanged = 1;
                if (menuType == 'm')  if (m != 3) { MainMenu[m][0] = ' ';m++;MainMenu[m][0] = '>'; }
                if (menuType == 'p')  if (p != 9) { PMenu[p][0] = ' ';p++;PMenu[p][0] = '>'; }
                break;

            case 'e':
            case  13: MenuChanged = 1;// 13 = 'ENTER'
                //if current menu is MainMenu:
                if (menuType == 'm')      
                    switch (m) 
                    {
                        // goes to PMenu
                        case 1: menuType = 'p'; break;
                        //goes to IMenu
                        case 2: menuType = 'i';break;
                        // quit the game/ game closes
                        case 3: menuType = 0; break;
                    }

                else if (menuType == 'p') 
                    switch (p) 
                    {
                        //start Player vs Player game mode
                        case 1: menuType = '2'; break;
                        //start Player vs Normal Bot game mode
                        case 2: menuType = 'b';break;
                        //start Player vs Max Bot game mode
                        case 3: menuType = 'M';break;
                        //start Player vs Smart Bot game mode
                        case 4: menuType = 's';break;
                        //switching on/off Player2 starts option
                        case 5: 
                                if (P2starts == 0) { P2starts = 1; strcpy(PMenu[5] + 23, "ON"); } 
                                else { P2starts = 0; strcpy(PMenu[5] + 23, "OFF"); }
                                break;
                        //switching on/off Blindfold Mode
                        case 6: 
                                if (BLFMode == 0) { BLFMode = 1;strcpy(PMenu[6] + 18, "ON"); }
                                else { BLFMode = 0; strcpy(PMenu[6] + 18, "OFF"); }
                                break;
                        //go back to MainMenu
                        case 9: menuType = 'm'; break;
                    }
                    //if in Info Menu, the go back to Main Menu
                    else if (menuType == 'i') menuType = 'm';
                    break;

            case 'a':
            case  75: //75 = 'LEFT'
                MenuChanged = 1;
                // decrements Max Coin Value
                if (p == 7) {
                    if (MaxCoinVal > 2) MaxCoinVal--;
                    NBtoCHAR(MaxCoinVal, MCVChar);
                    strcpy(PMenu[7] + 18, MCVChar);
                }
                // decrements Nb. of coins
                else if (p == 8) {
                    if (NBCoins > 3) NBCoins--;
                    NBtoCHAR(NBCoins, NBCChar);
                    strcpy(PMenu[8] + 19, NBCChar);
                }
                break;
            case 'd':
            case  77: //77= 'RIGHT'
                MenuChanged = 1;
                // increments Max Coin Value
                if (p == 7) {
                    if (MaxCoinVal < 30) MaxCoinVal++;
                    NBtoCHAR(MaxCoinVal, MCVChar);
                    strcpy(PMenu[7] + 18, MCVChar);
                }
                // increments Nb. of coins
                else if (p == 8) {
                    if (NBCoins < 16) NBCoins++;
                    NBtoCHAR(NBCoins, NBCChar);
                    strcpy(PMenu[8] + 19, NBCChar);
                }
                break;
        }
}

void GLogic() {
    // Game control logic
    if (_kbhit())
        switch (_getch())
        {
        //switches arrow position to left
        case 'a': HIT = 1; 
        case  75: //75 = 'LEFT'
            if (menuType != '2' || (menuType == '2' && ((HIT && P % 2 == 0) || (!HIT && P % 2 == 1))))
            {
                if (arrowISright == 1) arrowISright = 0; PMove = 1;
            }
            break;
        //switches arrow position to right
        case 'd': HIT = 1;
        case  77: 
            if (menuType != '2' || (menuType == '2' && ((HIT && P % 2 == 0) || (!HIT && P % 2 == 1))))
                {
                    if (arrowISright == 0) arrowISright = 1; 
                    PMove = 1;
                }
            break;
        case 'e': HIT = 1;
        //13 = 'ENTER'
        case  13: if (menuType != '2' || (menuType == '2' && ((HIT && P % 2 == 0) || (!HIT && P % 2 == 1))))
        {
            if (arrowISright == 0) {
                if (P % 2 == 0) P1[P / 2 + 1] = c[1];   //adding the coin to P1
                else       P2[(P + 1) / 2] = c[1];      //adding coin to P2
                //deleting coin from the coin row
                for (int i = 1;i < n;i++) c[i] = c[i + 1];
                n--;
            } 
            else {
                if (P % 2 == 0) P1[P / 2 + 1] = c[n];
                else       P2[(P + 1) / 2] = c[n];
                n--;
            }
            P++; PMove = 1;
        }
            break;
        }
}

void work() {
    // displaying a menu until a game starts
    while (menuType != 0 && !strchr("2bMs", menuType))
    {
        system("CLS");
        MenuChanged = 0;
        AF();
        while (!MenuChanged) Logic();
    }
}

void rand_coins() 
{ 
    // generating coins...
    for (int i = 1;i <= NBCoins;i++) 
        c[i] = rand() % MaxCoinVal + 1; 
}

int sum(int v[5]) 
{ 
    // diy vector sum
    int s = 0; 
    for (int i = 1;i <= 8;i++) 
        s += v[i]; 
    return s; 
}

void afG(int n) {
    //game display: players' turns, coin row, players' money bags, winner
    if (n != 0)
    {
        // player turn display
        if (P % 2 == 0) cout << "P1's turn" << endl;
        else if (menuType == '2') cout << "P2's turn" << endl;
        // coin row (and choice arrow)
        if (arrowISright == 0) {
            cout << "\t>\t";
            for (int i = 1;i <= n;i++) 
                if (BLFMode == 1) 
                    cout << "0\t"; 
                else 
                    cout << c[i] << '\t';
            cout << ' ' << endl;
        }
        else {
            cout << "\t \t";
            for (int i = 1;i <= n;i++) 
                if (BLFMode == 1) 
                    cout << "0\t"; 
                else 
                    cout << c[i] << '\t';
            cout << '<' << endl;
        }
    }
    cout << "P1:";
    // display P1 coin bag
    for (int i = 1;i <= 8;i++) 
        if (P1[i] != 0) 
        {
            if (BLFMode == 1 && n != 0) 
                cout << "0 "; 
            else cout << P1[i] << ' '; 
        }
    // display P1 total score, if game is over
    if (n == 0) cout << "| TOTAL: " << sum(P1);
    cout << endl; 

    // display P2/ Bot coin bag
    if (menuType != '2') cout << "Bot:"; 
    else cout << "P2:";
    for (int i = 1;i <= 8;i++) 
        if (P2[i] != 0) 
        { 
            if (BLFMode == 1 && n != 0) 
                cout << "0 "; 
            else cout << P2[i] << ' '; 
        }
    //if game is over, display P2 total score and winner
    if (n == 0) 
    {
        cout << "| TOTAL: " << sum(P2) << endl;

        if (sum(P1) != sum(P2)) {
            cout << "Winner is: ";
            if (sum(P1) > sum(P2)) cout << "P1!";
            else if (menuType != '2') cout << "Bot!";else cout << "P2!";
        }
        else cout << "TIE!";cout << endl;
    }
}

void Clear() {
    //game reset
    system("CLS");
    rand_coins();
    n = x = NBCoins; 
    if (!P2starts) x--;  
    arrowISright = 0; 
    change = 2;
    for (int i = 1;i <= 10;i++) 
        P1[i] = P2[i] = 0;
}

void gameP2() {  
    // simulating Player vs Player game mode
    Clear();
    while (n != 0)
    {
        if (P2starts == 1 && P == 0) 
            P++; 
        afG(n); 
        PMove = 0;
        while (PMove == 0) 
        { 
            HIT = 0; 
            GLogic(); 
        }
        system("CLS");
    }
    //when game ends, display game results and go to MainMenu 
    afG(n);
    system("PAUSE");
    menuType = 'm';
}

void gameB() {   
    //P1 VS Bot game mode
    //Bot is choosing randomly
    Clear();
    while (n != 0)
    {
        afG(n); if (P2starts == 1 && P == 0) { PMove = 1;P++; }
        else PMove = 0;
        while (PMove == 0 && n != 0) 
            GLogic();
        // if P1 finished its turn, Bot's turn starts
        if (P % 2 == 1 && n != 0) 
        {
            //Bot chose the left coin
            if (rand() % 2 == 0) {
                P2[(P + 1) / 2] = c[1];
                for (int i = 1;i < n;i++) 
                    c[i] = c[i + 1];
                n--;
            }
            //Bot chose the right coin
            else 
            { 
                P2[(P + 1) / 2] = c[n]; 
                n--; 
            }
            P++;
        }
        system("CLS");
    }
    afG(n);system("PAUSE");menuType = 'm';
}

void gameMB() {  
    //P1 VS Max Bot game mode
    //Bot chooses the max value between the two coins
    Clear();
    while (n != 0)
    {
        afG(n); if (P2starts == 1 && P == 0) { PMove = 1;P++; }
        else PMove = 0;
        while (PMove == 0 && n != 0) GLogic();
        if (P % 2 == 1 && n != 0)
        {
            if (c[1] > c[n]) {
                P2[(P + 1) / 2] = c[1];
                for (int i = 1;i < n;i++)
                    c[i] = c[i + 1]; 
                n--;
            }
            else {
                P2[(P + 1) / 2] = c[n];
                n--;
            }
            P++;
        }
        system("CLS");
    }
    afG(n);system("PAUSE");menuType = 'm';
}

void cpy(int a[101], int b[101]) 
{
    //diy vector copy
    for (int i = 1;i <= x;i++) 
        a[i] = b[i]; 
}

int fuse() {
    //function used for Smart Bot's choice

    //if the last time the fusion didn't occur or <= 3 values remain, then choose the max value
    if (x <= 3 || change == 0) 
    { 
        if (change == 0)
            change = 2; 
        if (v[1] >= v[x]) 
            return 1; 
        else 
            return 0; 
    } 
    else
    {
        y = 0;change = 0;
        for (int i = 2;i <= x;i++)
        {
            //if fusion occured, copy the rest of the row (can't do two fusions at the same time)
            if (change == 1)
                s[++y] = v[i];
            //adding the last two values
            else if (i == x)
            {
                s[++y] = v[i - 1];
                s[++y] = v[i];
            }
            //if element is greater than the neighbouring values, initiate fusion (sum of neighbours minus element)
            else if (v[i] >= v[i - 1] && v[i] >= v[i + 1])
            {
                s[++y] = v[i - 1] - v[i] + v[i + 1];
                i++; 
                change = 1;
            }
            //else skip element
            else
                s[++y] = v[i - 1];
        }
        cpy(v, s); 
        x = y; 
        return fuse();
    }
}

void gameSB() {  
    //P1 VS Smart Bot simulation
    //using fuse() function to decide
    Clear();
    while (n != 0)
    {
        afG(n);
        if (P2starts == 1 && P == 0) 
        { 
            PMove = 1;
            P++; 
        }
        else PMove = 0;
        while (PMove == 0) 
            GLogic();
        if (P % 2 == 1 && n != 0)
        {
            cpy(v, c);
            x = NBCoins - P + P2starts;
            if (fuse()) 
            { 
                P2[(P + 1) / 2] = c[1]; 
                for (int i = 1;i < n;i++)
                    c[i] = c[i + 1]; 
                n--; 
            }
            else 
            { 
                P2[(P + 1) / 2] = c[n]; 
                n--; 
            }
            P++;
        }
        system("CLS");
    }
    afG(n);system("PAUSE");menuType = 'm';
}

int main() {
    cit(); 
    srand(time(0));
    while (menuType != 0)
    {
        if      (menuType == '2') gameP2();
        else if (menuType == 'b') gameB();
        else if (menuType == 'M') gameMB();
        else if (menuType == 's') gameSB();
        work();
    }
    return 0;
}
