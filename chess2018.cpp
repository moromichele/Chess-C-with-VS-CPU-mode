#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <time.h>
#define SIZE 8

using namespace std;

class Chess {

private:

    const char playerW = 'W';
    const char playerB = 'B';
    char player;
    char board[SIZE][SIZE];
    const string moves[SIZE][SIZE] = {   {"8A","8B","8C","8D", "8E","8F", "8G", "8H"},
                                         {"7A","7B","7C","7D", "7E","7F", "7G", "7H"},
                                         {"6A","6B","6C","6D", "6E","6F", "6G", "6H"},
                                         {"5A","5B","5C","5D", "5E","5F", "5G", "5H"},
                                         {"4A","4B","4C","4D", "4E","4F", "4G", "4H"},
                                         {"3A","3B","3C","3D", "3E","3F", "3G", "3H"},
                                         {"2A","2B","2C","2D", "2E","2F", "2G", "2H"},
                                         {"1A","1B","1C","1D", "1E","1F", "1G", "1H"}};
    const char cleanBoard[SIZE][SIZE] = {{'#',' ','#',' ','#',' ','#',' '},
                                         {' ','#',' ','#',' ','#',' ','#'},
                                         {'#',' ','#',' ','#',' ','#',' '},
                                         {' ','#',' ','#',' ','#',' ','#'},
                                         {'#',' ','#',' ','#',' ','#',' '},
                                         {' ','#',' ','#',' ','#',' ','#'},
                                         {'#',' ','#',' ','#',' ','#',' '},
                                         {' ','#',' ','#',' ','#',' ','#'}};

    vector<string> validMoves;
    vector<string> allEnemyMoves;
    vector<string> allMyMoves;
    bool gOver;
    int xA, xB, yA, yB;
    int xWKing, yWKing, xBKing, yBKing;
    int WKingMoves,BKingMoves;
    int BLeftRookMoves, BRightRookMoves;
    int WLeftRookMoves, WRightRookMoves;
    string enPVictim;
    bool CPU;
    const char whites[6]={'P','R','K','B','Q','X'};
    const char blacks[6]={'p','r','k','b','q','x'};
    bool NEXT = false;

public:

    Chess(bool cpu){
        gOver = false;
        CPU = cpu;
        player = 'W';
        xA=0;
        xB=0;
        yA=0;
        yB=0;
        WKingMoves = 0;
        BKingMoves = 0;
        BLeftRookMoves=0;
        BRightRookMoves=0;
        WLeftRookMoves=0;
        WRightRookMoves=0;
        enPVictim="0";
        Init();
        NewGame();
        Set_KingsCoords();
    }

    ~Chess(){

    }

    void Init(){//creazione scacchiera
        for (int i = 0; i<SIZE ; i++){
            for (int j =0; j<SIZE; j++){
                board[i][j]=cleanBoard[i][j];
            }
        }
    }

    void NewGame(){//posizionamento pedine

        //black
        for (int j = 0; j<SIZE ; j++){
            board[1][j]=blacks[0];
        }
        board[0][0]= board[0][7]= blacks[1];
        board[0][1]=board[0][6]=blacks[2];
        board[0][2]=board[0][5]=blacks[3];
        board[0][3]=blacks[4];
        board[0][4]=blacks[5];

        //whites
        for (int j = 0; j<SIZE ; j++){
            board[6][j]=whites[0];
        }
        board[7][0]=board[7][7]=whites[1];
        board[7][1]=board[7][6]=whites[2];
        board[7][2]=board[7][5]=whites[3];
        board[7][3]=whites[4];
        board[7][4]=whites[5];

    }

    void Print(){//stampa scacchiera e giocatore (W o B)

        cout<<"  | A B C D E F G H |"<<endl;
        cout<<"  ____________________"<<endl;
        for (int i = 0; i<SIZE ; i++){
            cout<<SIZE-i<<" | ";
            for (int j =0; j<SIZE; j++){
                cout<<board[i][j]<<" ";
            }
            cout<<"|"<<SIZE-i<<endl;
        }
        cout<<"__|_________________|_"<<endl;
        cout<<player<<" | A B C D E F G H"<<endl<<endl;
    }

    void Start(){
        Print();
        Play();
    }

    inline void ChangePlayer(){
        if(player == playerW){
            player = playerB;
        }else player = playerW;
    }

    inline void Set_A(string a){
        xA = '8'-a[0];
        yA = a[1]-65;
    }
    inline void Set_B(string b){
        xB = '8'-b[0];
        yB = b[1] - 65;
    }

    void Set_KingsCoords(){//salva coordinate dei re per condizioni di fine partita
        for (int i = 0; i<SIZE ; i++){
            for (int j =0; j<SIZE; j++){
                if(board[i][j]==whites[5]){
                    xWKing = i;
                    yWKing = j;
                }else if(board[i][j]==blacks[5]){
                    xBKing = i;
                    yBKing = j;
                }
            }
        }
    }

    void PrintMoves(){//stampa mosse disponibili per la pedina selezionata
        for(int j=0;j<validMoves.size();j++){
             cout<<validMoves[j];
             if(j!=validMoves.size()-1){
                cout<<", ";
             }else cout<<".";
         }
         cout<<endl;
    }

    void Play(){

        while(!gOver){//rimane in questo while fino alla fine della partita
            allMyMoves.clear();
            allEnemyMoves.clear();
            validMoves.clear();

            cout<<"Turno del giocatore "<<player<<endl;
            //valori di controllo per loop/condizioni necessarie mosse speciali
            int c=0;
            int k=0;
            int y=0;
            int j=0;
            int en=0;
            string from = "0";
            string to = "0";

            while(k==0){//scelta pedina da muovere

                cout<<"Quale pedina muovere? [2. Arrenditi |3. Mostra scacchiera";
                if(CanCastle()>0){//se disponibile l'arrocco
                    cout<<" |4.Arrocco";
                    c++;
                }
                cout<<"]"<<endl;
                cin>>from;
                if (from == "2"){
                    k++;
                    y++;
                    j++;
                    Lose();
                }else if(from=="3"){
                    Print();

                }else if(from=="4" && c==1) {
                    DoCastle();
                    k++;
                    y++;
                }else if(Validate_from(from)){   //controlla sintassi e che abbia mosse disponibili
                    k++;
                }else cout<<"Posizione non valida (Esempio input valido: 3B, 3b, b3...)"<<endl;
            }

            while(y==0){
                cout<<"Dove? [1. Mostre mosse | 2. Arrenditi |3. Mostra scacchiera";
                if(board[xA][yA]=='P'||board[xA][yA]=='p'){
                    if(CanEnpassant()>0){//se ho scelto un pedone che puo fare en passant
                        cout<<" |4. En passant";
                    }
                }
                cout<<"]"<<endl;
                cin>>to;
                if(to== "1"){
                    PrintMoves();
                }else if (to == "2"){
                        y++;
                        j++;
                        Lose();
                }else if(to== "3"){
                    Print();
                }else if (to=="4" && CanEnpassant()>0){
                    DoEnPassant();
                    y++;
                }else if(Validate_to(to)){       //controlla sintassi e regole
                        y++;
                        PawnPromotion(); //se è un pedone sull ultima riga promuovo
                        Move();

                }else cout<<"Destinazione non valida"<<endl;

            }

            if(enPVictim != moves[xB][yB]) enPVictim = "0"; //se non ho mosso un pedone di due resetto la variabile

            if(j==0){
                ChangePlayer();
                Print();
                IsGameOver();//SE L'avversario non ha mosse finisce la partita
                if(CPU&&(!gOver)){//se è una partita contro il pc gioca il pc
                    CPU_Turn();
                }


            }
        }
    }

    inline void Move(){//ho posizione di partenza e arrivo, muovo
        board[xB][yB] = board[xA][yA];
        board[xA][yA] = cleanBoard[xA][yA];
    }

    void CPU_Turn(){

        //1.CONTROLLO UNA PER UNA SE POSSO MUOVERMI METTENDO IN SCACCO IL NEMICO
        //2.SE NON POSSO 1 CONTROLLO UNA A UNA SE POSSO MANGIARE
        //3.SE NON POSSO CONTROLLO SE POSSO METTERE "IN SCACCO" UNA PEDINA NEMICA (NON RE)
        //4.SE NON POSSO NE 1 NE 2 NE 3 MUOVO A CASO
        allMyMoves.clear();
        allEnemyMoves.clear();
        validMoves.clear();
        int moved =0;

        if(ThreatenKing()){//1
           moved++;
        }

        if(moved==0) { //2
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; ++j) {
                    if (IsMine(i, j) && moved == 0) {
                        Set_A(moves[i][j]);
                        if (Check_Mosse(i, j, validMoves)) {//ovvero ha mosse disponibili
                            //CERCO SE TRA LE MOSSE C'è UNA MANGIATA
                            for (int l = 0; l < validMoves.size(); l++) {
                                if (IsEnemy(validMoves[l])) {
                                    Set_B(validMoves[l]);
                                    moved++;
                                }
                            }
                        }
                        validMoves.clear();
                    }
                }
            }
        }

        if(moved==0){
            if(ThreatenEnemy()){
                moved++;
            }
        }

        if(moved==0){ //3
            //creo vettore di pedine che possono muoversi
            //ne scelgo una a caso
            //la muovo in un posto a caso
            vector<string> canMove;
            for(int i=0; i<SIZE ; i++){
                for (int j = 0; j < SIZE ; ++j) {
                    if(IsMine(i,j)&& moved==0){
                        Set_A(moves[i][j]);
                        if(Check_Mosse(i,j,validMoves)){//ovvero ha mosse disponibili
                            canMove.push_back(moves[i][j]);//creo vettore con tutte quelle che possono muoversi
                        }
                    }
                    validMoves.clear();
                }
            }
            int r = rand() % canMove.size();
            Set_A(canMove[r]);
            Check_Mosse(xA,yA,validMoves);//popolo validmoves
            int k = rand() % validMoves.size();
            Set_B(validMoves[k]);
        }

        cout<<"MUOVO "<<moves[xA][yA]<<" IN POSIZIONE "<<moves[xB][yB]<<endl;
        Move();
        Print();
        ChangePlayer();
        IsGameOver(); //se il giocatore non ha mosse finisce la partita
    }

    bool ThreatenKing() {
        validMoves.clear();
        vector<string> hisValidMoves;
        int xV,yV;
        xV=0;
        yV=0;
        string temp = "00";
        for (int i = 0; i < SIZE; ++i) {//SCORRO LE PEDINE
            for (int j = 0; j < SIZE; ++j) {
                if(IsMine(i,j)){//SE è MIA
                    Set_A(moves[i][j]);
                    Check_Mosse(i,j,validMoves);//POPOLO LE SUE MOSSE VALIDE
                    //PER OGNI MOSSA VALIDA POPOLO LE SUE MOSSE VALIDE
                    //SE TRA LE SUE MOSSE VALIDE CI SONO LE COORDINATE DEL RE BIANCO
                    //FINITO
                    for (int k = 0; k < validMoves.size(); ++k) {
                        //SE NON C'è IL RE SVUOTO IL VETTORE E PROVO CON LA PROSSIMA MOSSA
                        temp = validMoves[k];
                        xV = '8'- temp[0];
                        yV = temp[1]-65;
                        char lol ='z';
                        lol = board[xV][yV];
                        board[xV][yV]=board[xA][yA];
                        board[xA][yA]=cleanBoard[xA][yA];
                        Check_Mosse(xV,yV,hisValidMoves);
                        board[xA][yA]=board[xV][yV];
                        board[xV][yV]=lol;
                        for (int l = 0; l < hisValidMoves.size(); ++l) {
                            if(hisValidMoves[l]==moves[xWKing][yWKing]){
                                Set_B(temp);
                                return true;
                            }
                        }

                        hisValidMoves.clear();
                    }
                    validMoves.clear();
                }
            }
        }
            return false;
    }

    bool ThreatenEnemy(){
        validMoves.clear();
        vector<string> hisValidMoves;
        int xV,yV;
        xV=0;
        yV=0;
        string temp = "00";
        for (int i = 0; i < SIZE; ++i) {//SCORRO LE PEDINE
            for (int j = 0; j < SIZE; ++j) {
                if(IsMine(i,j)){//SE è MIA
                    Set_A(moves[i][j]);
                    Check_Mosse(i,j,validMoves);//POPOLO LE SUE MOSSE VALIDE
                    //PER OGNI MOSSA VALIDA POPOLO LE SUE MOSSE VALIDE
                    //SE TRA LE SUE MOSSE VALIDE CI SONO LE COORDINATE DEL RE BIANCO
                    //FINITO
                    for (int k = 0; k < validMoves.size(); ++k) {
                        //SE NON C'è IL RE SVUOTO IL VETTORE E PROVO CON LA PROSSIMA MOSSA
                        temp = validMoves[k];
                        xV = '8'- temp[0];
                        yV = temp[1]-65;
                        char lol ='z';
                        lol = board[xV][yV];
                        board[xV][yV]=board[xA][yA];
                        board[xA][yA]=cleanBoard[xA][yA];
                        Check_Mosse(xV,yV,hisValidMoves);
                        board[xA][yA]=board[xV][yV];
                        board[xV][yV]=lol;
                        for (int l = 0; l < hisValidMoves.size(); ++l) {
                            if(IsEnemy(hisValidMoves[l])){
                                Set_B(temp);
                                return true;
                            }
                        }

                        hisValidMoves.clear();
                    }
                    validMoves.clear();
                }
            }
        }
        return false;
    }

    void Lose(){//fine partita
        gOver = true;
        ChangePlayer();
        cout<<"VINCITORE: giocatore "<<player<<endl;
    }

    int CanCastle(){//posso fare arrocco?

        int can = 0;

        if(player=='W'){//ARROCCO BIANCO
            if(WKingMoves==0){
                //CON LA TORRE SINISTRA
                xA=xWKing;
                yA=yWKing;
                if(WLeftRookMoves==0){
                    if(board[7][1]==cleanBoard[7][1] && board[7][2]==cleanBoard[7][2] && board[7][3]==cleanBoard[7][3]){
                        if(!IsSuicide(7,2) && !IsSuicide(7,3) && !IsSuicide(xA,yA)){
                            can+=1;
                        }
                    }
                }
                //CON LA TORRE DESTRA
                if(WRightRookMoves==0){
                    if(board[7][5]==cleanBoard[7][5] && board[7][6]==cleanBoard[7][6]){
                        if(!IsSuicide(7,5) && !IsSuicide(7,6) &&!IsSuicide(xA,yA)){
                            can+=10;
                        }
                    }
                }
                board[xA][yA]=whites[5];
            }
        }else{//ARROCCO NERO
            if(BKingMoves==0){
                //CON LA TORRE SINISTRA
                xA=xBKing;
                yA=yBKing;
                if(BLeftRookMoves==0){
                    if(board[0][1]==cleanBoard[0][1] && board[0][2]==cleanBoard[0][2] && board[0][3]==cleanBoard[0][3]){
                        if(!IsSuicide(0,2) && !IsSuicide(0,3) &&!IsSuicide(xA,yA)){
                            can+=1;
                        }
                    }
                }
                //CON LA TORRE DESTRA
                if(BRightRookMoves==0){
                    if(board[0][5]==cleanBoard[0][5] && board[0][6]==cleanBoard[0][6]){
                        if(!IsSuicide(0,5) && !IsSuicide(0,6) &&!IsSuicide(xA,yA)){
                            can+=10;
                        }
                    }
                }
                board[xA][yA]=blacks[5];
            }
        }
        return can;
    }

    void DoCastle(){//esecuzione arrocco
        string input ="0";
        int k=0;
        if(player =='W'){
            //SE POSSO SIA A DESTRA CHE A SINISTRA CHIEDO
            if(WRightRookMoves==0 && WLeftRookMoves==0 && CanCastle()==11){
                while(k==0) {
                    cout << "Dove? [1. A destra 2. A sinistra]" << endl;
                    cin >> input;
                    if (input == "1") {
                        CastleRight();
                        k++;
                    } else if (input == "2") {
                        CastleLeft();
                        k++;
                    } else cout << "Input non valido" << endl;
                }

            }else if(WRightRookMoves==0 && CanCastle()==10){
                CastleRight();
            }else if(WLeftRookMoves==0 && CanCastle()==1){
               CastleLeft();
            }
        }else{
            if(BRightRookMoves==0 && BLeftRookMoves==0 && CanCastle()==11){
                //ask
                cout<<"Dove? [1. A destra 2. A sinistra]"<<endl;
                cin>>input;
            }else if(BRightRookMoves==0 && CanCastle()==10){
                CastleRight();
            }else if(BLeftRookMoves==0 && CanCastle()==1){
                CastleLeft();
            }
        }
    }

    void CastleRight(){//non posso usare Move() perche muovo 2 pedine
        if(player=='W'){
            board[7][6]=whites[5];
            board[7][5]=whites[1];
            board[7][4]=cleanBoard[7][4];
            board[7][7]=cleanBoard[7][7];
            WKingMoves=1;
            WRightRookMoves=1;
        }else{
            board[0][6]=blacks[5];
            board[0][5]=blacks[1];
            board[0][4]=cleanBoard[0][4];
            board[0][7]=cleanBoard[0][7];
            BKingMoves=1;
            BRightRookMoves=1;
        }
    }

    void CastleLeft(){
        if(player=='W'){
            board[7][2]=whites[5];
            board[7][3]=whites[1];
            board[7][4]=cleanBoard[7][4];
            board[7][0]=cleanBoard[7][0];
            WKingMoves=1;
            WRightRookMoves=1;
        }else{
            board[0][2]=blacks[5];
            board[0][3]=blacks[1];
            board[0][4]=cleanBoard[0][4];
            board[0][0]=cleanBoard[0][0];
            BKingMoves=1;
            BLeftRookMoves=1;
        }
    }

    int CanEnpassant(){//RITORNA 1 eN PASSANT DESTRA 2 EN PASSANT SINISTRA
        int can = 0;

        if(player=='W'){
            //DESTRA
            if(yA<SIZE-1){
                if(board[xA-1][yA+1]==cleanBoard[xA-1][yA+1] && moves[xA][yA+1]==enPVictim){
                    can=1;
                }//AND SI é MOSSO DI DUE AL TURNO PRIMA QUELLO A DESTRA
            }
            //sinistra
            if(yA>0){
                if(board[xA-1][yA-1]==cleanBoard[xA-1][yA-1] && moves[xA][yA-1]==enPVictim){
                    can=2;
                }
            }
        }else{
            //DESTRA
            if(yA<SIZE-1){
                if(board[xA+1][yA+1]==cleanBoard[xA+1][yA+1] && moves[xA][yA+1]==enPVictim){
                    can=1;
                }//AND SI é MOSSO DI DUE AL TURNO PRIMA QUELLO A DESTRA
            }
            //sinistra
            if(yA>0){
                if(board[xA+1][yA-1]==cleanBoard[xA+1][yA-1] && moves[xA][yA-1]==enPVictim){
                    can=2;
                }
            }
        }

        return can;
    }

    void DoEnPassant(){//1 destra 2 sinistra
        //anche qui non posso usare Move()
        if(player == 'W'){
            if(CanEnpassant()==1){
                board[xA-1][yA+1]=whites[0];
                board[xA][yA]=cleanBoard[xA][yA];
                board[xA][yA+1]=cleanBoard[xA][yA+1];
            }else if(CanEnpassant()==2){
                board[xA-1][yA-1]=whites[0];
                board[xA][yA]=cleanBoard[xA][yA];
                board[xA][yA-1]=cleanBoard[xA][yA-1];
            }
        }else{
            if(CanEnpassant()==1){
                board[xA+1][yA+1]=blacks[0];
                board[xA][yA]=cleanBoard[xA][yA];
                board[xA][yA+1]=cleanBoard[xA][yA+1];
            }else if(CanEnpassant()==2){
                board[xA+1][yA-1]=blacks[0];
                board[xA][yA]=cleanBoard[xA][yA];
                board[xA][yA-1]=cleanBoard[xA][yA-1];
            }
        }
    }

    bool IsMine(int x, int y){//true se la pedina xy è mia
        int cont =0;
        for(int i =0; i<6; i++){
            if(player=='W'){
                if(board[x][y]==whites[i])cont++;
            } else if(player=='B'){
                if(board[x][y]==blacks[i])cont++;
            }
        }
        return cont>0;
    }

    bool IsEnemy(int x, int y){//true se la pedina xy è nemica
        int cont =0;
        for(int i = 0; i<6; i++){
            if(player=='W'){
                if(board[x][y]==blacks[i])cont++;
            }else if(player =='B'){
                if(board[x][y]==whites[i])cont++;
            }
        }
       return cont>0;
    }

    bool IsEnemy(string to){//argomento string
        int cont =0;
        int x = '8'-to[0];
        int y = to[1]-65;
        for(int i = 0; i<6; i++){
            if(player=='W'){
                if(board[x][y]==blacks[i])cont++;
            }else if(player =='B'){
                if(board[x][y]==whites[i])cont++;
            }
        }
        return cont>0;
    }

    bool Validate_from(string from){

        if(Check_Lex(from)){ //check lessico da qui ho formato 3A, 4B
            //dev'essere mia
            //deve avere mosse
            Set_A(from);
            if(IsMine(xA,yA)){//mia?
                return Check_Mosse(xA,yA,validMoves);//Ha mosse?

            }
        }

        return false;
    }

    bool Validate_to(string to){
        if(Check_Lex(to)){//check lessico

            Set_B(to);
            if(IsValid()){
                //CONDIZIONI ARROCCO NON PIU VALIDE SE MUOVO RE O TORRE
                if(xA==0 && yA ==0 && board[xA][yA]==blacks[1]){
                    BLeftRookMoves=1;
                }else if(xA==0 && yA ==7 && board[xA][yA]==blacks[1]){
                    BRightRookMoves=1;
                }else if(xA==7 && yA ==0 && board[xA][yA]==whites[1]){
                    WLeftRookMoves=1;
                }else if(xA==7 && yA ==7 && board[xA][yA]==whites[1]){
                    WRightRookMoves=1;
                }else if(xA==0 && yA ==4 && board[xA][yA]==blacks[5]){
                    BKingMoves=1;
                }else if(xA==7 && yA ==4 && board[xA][yA]==blacks[5]){
                    WKingMoves=1;
                }
                //è UNA POSSIBILE VITTIMA DI EN PASSANT AL PROSSIMO TURNNO?
                if(player == 'W'){
                    if((xA-xB)==2)
                        enPVictim = moves[xB][yB];
                }else {
                    if((xB-xA)==2)
                        enPVictim = moves[xB][yB];
                }
                return true;
            }
        }
        return false;

    }

    bool Check_Lex(string& in){//se input valido lo formatto a NUMERO LETTERAMAIUSC
        if(in.length()==2){
            if(isdigit(in[0]) && isupper(in[1])){
                return true;
            }else if(isdigit(in[0]) && islower(in[1])){
                in[1] = in[1]-32;
                return true;
            }else if(isdigit(in[1]) && isupper(in[0])){
                char temp= ' ';
                temp = in[1];
                in[1]=in[0];
                in[0]=temp;
                return true;
            }else if(isdigit(in[1]) && islower(in[0])){
                char temp= ' ';
                temp = in[1];
                in[1]=in[0];
                in[0]=temp;
                in[1] = in[1]-32;
                return true;
            }
        }

        return false;

    }

    void IsGameOver(){//Regole fine partita
        //inizio del mio turno: listo tutte le mie mosse
        //SE NON HO MOSSE
        //SONO IN SCACCO? il nemico può mangiarmi il re? CHECKMATE WIN AVVERSARIO
        //NON SONO IN SCACCO MA NON POSSO MUOVERMI? STALEMATE PAREGGIO
        ListMyMoves();//popolo my moves

        if(allMyMoves.empty()){//se non ne ho
           // ListEnemyMoves();//popolo mosse nemiche
            vector<string>::iterator it;
            //DIFFERENZIO TRA WKIN E BKING
            if(player=='W'){
                it = find(allEnemyMoves.begin(), allEnemyMoves.end(), moves[xWKing][yWKing]);
            }else {
                it = find(allEnemyMoves.begin(), allEnemyMoves.end(), moves[xBKing][yBKing]);
            }
            if(it!=allEnemyMoves.end()){
                cout<<"CHECKMATE"<<endl;
                Lose();//VINCE L'ALTRO
            }else{
                cout<<"STALEMATE"<<endl;
                gOver=true;//PAREGGIO
            }

        }
    }

    bool IsSuicide(int x, int y){//controlla se la mossa è un suicidio (illegale)
        if(player == 'W'){
            char temp = board[x][y];
            board[x][y]=board[xA][yA];
            board[xA][yA]=cleanBoard[xA][yA];
            Set_KingsCoords();
            ListEnemyMoves(true);
            vector<string>::iterator it;
            it = find(allEnemyMoves.begin(), allEnemyMoves.end(), moves[xWKing][yWKing]);
            board[xA][yA]=board[x][y];
            board[x][y]=temp;
            Set_KingsCoords();
            return it!=allEnemyMoves.end();

        }else{
            char temp = board[x][y];
            board[x][y]=board[xA][yA];
            board[xA][yA]=cleanBoard[xA][yA];
            Set_KingsCoords();
            ListEnemyMoves(true);
            vector<string>::iterator it;
            it = find(allEnemyMoves.begin(), allEnemyMoves.end(), moves[xBKing][yBKing]);
            board[xA][yA]=board[x][y];
            board[x][y]=temp;
            Set_KingsCoords();
            return (it!=allEnemyMoves.end());
        }
    }

    bool Check_Mosse(int x,int y, vector<string> &v){//popola vettore con le mosse pedina e ritorna bool
        switch(board[x][y]){
            case 'P':
                return WPawn_Moves(x,y, v);

            case 'p':
                return BPawn_Moves(x,y, v);

            case 'R':
            case 'r':
                return Rook_Moves(x,y, v);

            case 'K':
            case 'k':
                return Knight_Moves(x,y, v);

            case 'B':
            case 'b':
                return Bishop_Moves(x,y, v);

            case 'Q':
            case 'q':
                return Queen_Moves(x,y, v);

            case 'X':
            case 'x':
                return King_Moves(x,y, v);

            default:
                cout<<"Pedina non valida";

        }

        return false;
    }

    bool IsValid(){//usato nel ValidateTo, true se to è una mossa della pedina
            vector<string>::iterator it;
            it = find(validMoves.begin(), validMoves.end(), moves[xB][yB]);
            return (it != validMoves.end());
    }

    void ListEnemyMoves(bool future){//future true= mosse in un ipotetico turno successivo

        allEnemyMoves.clear();
        NEXT = future;

        for (int i = 0; i<SIZE ; i++){
            for (int j = 0; j<SIZE; j++){
                if(IsEnemy(i,j)){//LA PEDINA è w?
                    ChangePlayer(); //IL GIOCATORE è W ora
                    Check_Mosse(i,j,allEnemyMoves);
                    ChangePlayer();
                }

            }
        }
        NEXT = false;
    }

    void ListMyMoves(){//popolo vettore allMyMoves
        allMyMoves.clear();

        for (int i = 0; i<SIZE ; i++){
            for (int j = 0; j<SIZE; j++){
                if(IsMine(i,j)){//LA PEDINA è B?
                    //B è il giocatore
                    xA=i;
                    yA=j;
                    Check_Mosse(i,j,allMyMoves);

                }
            }
        }
    }

    bool WPawn_Moves(int x, int y, vector<string> &v){//logica pedone bianco

        if(board[x-1][y]=='#'||board[x-1][y]==' '){
            if(NEXT){
                v.push_back(moves[x-1][y]);
            }else if(!IsSuicide(x-1,y))v.push_back(moves[x-1][y]);
        }
        if(y<SIZE-1) {
            if (IsEnemy(x - 1, y + 1)) {
                if (NEXT) {
                    v.push_back(moves[x - 1][y + 1]);
                } else if (!IsSuicide(x - 1, y + 1))v.push_back(moves[x - 1][y + 1]);
            }
        }
        if(y>0) {
            if (IsEnemy(x - 1, y - 1)) {
                if (NEXT) {
                    v.push_back(moves[x - 1][y - 1]);
                } else if (!IsSuicide(x - 1, y - 1))v.push_back(moves[x - 1][y - 1]);
            }
        }
        if(x==6){
            if(board[x-2][y]=='#'||board[x-2][y]==' '){
                if(NEXT){
                    v.push_back(moves[x-2][y]);
                }else if(!IsSuicide(x-2,y))v.push_back(moves[x-2][y]);
            }
        }
        return (!v.empty());

    }

    bool BPawn_Moves(int x, int y, vector<string> &v){ //logica pedone nero

        if(board[x+1][y]=='#'||board[x+1][y]==' '){
            if(NEXT){
                v.push_back(moves[x+1][y]);
            }else if(!IsSuicide(x+1,y))v.push_back(moves[x+1][y]);
        }
        if(y<SIZE-1) {
            if (IsEnemy(x + 1, y + 1)) {
                if (NEXT) {
                    v.push_back(moves[x + 1][y + 1]);
                } else if (!IsSuicide(x + 1, y + 1))v.push_back(moves[x + 1][y + 1]);
            }
        }
        if(y>0) {
            if (IsEnemy(x + 1, y - 1)) {
                if (NEXT) {
                    v.push_back(moves[x + 1][y - 1]);
                } else if (!IsSuicide(x + 1, y - 1))v.push_back(moves[x + 1][y - 1]);
            }
        }
        if(x==1){
            if(board[x+2][y]=='#'||board[x+2][y]==' '){
                if(NEXT){
                    v.push_back(moves[x+2][y]);
                }else if(!IsSuicide(x+2,y))v.push_back(moves[x+2][y]);
            }
        }
        return (!v.empty());
    }

    bool Rook_Moves(int x, int y, vector<string> &v){//logica torri
        //UP
        int mine =0;
        int enemy =0;
        int i=1;
        if(x>0){//OVVERO SE PUO MUOVERSI VERSO L'ALTo
            while(mine==0 && enemy == 0 && x-i>=0 && x-i<7){

                if(IsMine(x-i,y))mine++;
                if(IsEnemy(x-i,y)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x-i][y]);
                    }else if(!IsSuicide(x-i,y))v.push_back(moves[x-i][y]);
                }
                if(board[x-i][y]=='#'||board[x-i][y]==' '){
                    if(NEXT){
                        v.push_back(moves[x-i][y]);
                    }else if(!IsSuicide(x-i,y))v.push_back(moves[x-i][y]);
                }

                i++;

            }
        }
        //DOWN
        i=1;
        mine =0;
        enemy =0;
        if(x<SIZE-1){//SE PUO MUOVERSI VERSO IL BASSO
            while(mine==0 && enemy == 0 && x+i>0 && x+i<=7){

                if(IsMine(x+i,y))mine++;
                if(IsEnemy(x+i,y)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x+i][y]);
                    }else if(!IsSuicide(x+i,y))v.push_back(moves[x+i][y]);
                }
                if(board[x+i][y]=='#'||board[x+i][y]==' '){
                    if(NEXT){
                        v.push_back(moves[x+i][y]);
                    }else if(!IsSuicide(x+i,y))v.push_back(moves[x+i][y]);
                }

                i++;
            }
        }
        //RIGHT
        i=1;
        mine =0;
        enemy =0;
        if(y<SIZE-1){//SE PUO MUOVERSI VERSO DESTRA
            while(mine==0 && enemy == 0 && y+i>0 && y+i<=7){

                if(IsMine(x,y+i))mine++;
                if(IsEnemy(x,y+i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x][y+i]);
                    }else if(!IsSuicide(x,y+i))v.push_back(moves[x][y+i]);
                }
                if(board[x][y+i]=='#'||board[x][y+i]==' '){
                    if(NEXT){
                        v.push_back(moves[x][y+i]);
                    }else if(!IsSuicide(x,y+i))v.push_back(moves[x][y+i]);
                }

                i++;
            }
        }
        //LEFT
        i=1;
        mine =0;
        enemy =0;
        if(y>0){//SE PUO MUOVERSI A SINISTRA
            while(mine==0 && enemy == 0 && y-i>=0 && y-i<7){

                if(IsMine(x,y-i))mine++;
                if(IsEnemy(x,y-i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x][y-i]);
                    }else if(!IsSuicide(x,y-i))v.push_back(moves[x][y-i]);
                }
                if(board[x][y-i]=='#'||board[x][y-i]==' '){
                    if(NEXT){
                        v.push_back(moves[x][y-i]);
                    }else if(!IsSuicide(x,y-i))v.push_back(moves[x][y-i]);
                }

                i++;

            }
        }
        return (!v.empty());
    }

    bool Knight_Moves(int x, int y, vector<string> &v){//logica cavalli
        if(x-2>=0){//ALTOALTO
            if(y-1>=0){//SX
                    if(board[x-2][y-1]=='#'||board[x-2][y-1]==' '||IsEnemy(x-2,y-1)){
                        if(NEXT){
                            v.push_back(moves[x-2][y-1]);
                        }else if(!IsSuicide(x-2,y-1))v.push_back(moves[x-2][y-1]);
                    }
                }
                if(y+1<8){//DX
                    if(board[x-2][y+1]=='#'||board[x-2][y+1]==' '||IsEnemy(x-2,y+1)){
                        if(NEXT){
                            v.push_back(moves[x-2][y+1]);
                        }else if(!IsSuicide(x-2,y+1))v.push_back(moves[x-2][y+1]);
                    }
            }

        }
        if(x-1>=0){//ALTO
            if(y-2>=0){//SXSX
                if(board[x-1][y-2]=='#'||board[x-1][y-2]==' '||IsEnemy(x-1,y-2)){
                    if(NEXT){
                        v.push_back(moves[x-1][y-2]);
                    }else if(!IsSuicide(x-1,y-2))v.push_back(moves[x-1][y-2]);
                }
            }
            if(y+2<8){//DXDX
                if(board[x-1][y+2]=='#'||board[x-1][y+2]==' '||IsEnemy(x-1,y+2)){
                    if(NEXT){
                        v.push_back(moves[x-1][y+2]);
                    }else if(!IsSuicide(x-1,y+2))v.push_back(moves[x-1][y+2]);
                }
            }

        }
        if(x+1<8){//BASSO
            if(y-2>=0){//SXSX
                if(board[x+1][y-2]=='#'||board[x+1][y-2]==' '||IsEnemy(x+1,y-2)){
                    if(NEXT){
                        v.push_back(moves[x+1][y-2]);
                    }else if(!IsSuicide(x+1,y-2))v.push_back(moves[x+1][y-2]);
                }
            }
            if(y+2<8){//DXDX
                if(board[x+1][y+2]=='#'||board[x+1][y+2]==' '||IsEnemy(x+1,y+2)){
                    if(NEXT){
                        v.push_back(moves[x+1][y+2]);
                    }else if(!IsSuicide(x+1,y+2))v.push_back(moves[x+1][y+2]);
                }
            }
        }
        if(x+2<8){//BASSOBASSO
            if(y-1>=0){//SX
                if(board[x+2][y-1]=='#'||board[x+2][y-1]==' '||IsEnemy(x+2,y-1)){
                    if(NEXT){
                        v.push_back(moves[x+2][y-1]);
                    }else if(!IsSuicide(x+2,y-1))v.push_back(moves[x+2][y-1]);
                }
            }
            if(y+1<8){//DX
                if(board[x+2][y+1]=='#'||board[x+2][y+1]==' '||IsEnemy(x+2,y+1)){
                    if(NEXT){
                        v.push_back(moves[x+2][y+1]);
                    }else if(!IsSuicide(x+2,y+1))v.push_back(moves[x+2][y+1]);
                }
            }

        }


        return (!v.empty());
    }

    bool Bishop_Moves(int x, int y, vector<string> &v){//logica alfieri
        //UP LEFT
        int mine =0;
        int enemy =0;
        int i=1;
        if(x>0 && y >0){//OVVERO SE PUO MUOVERSI VERSO L'ALTO A SINISTRA
            while(mine==0 && enemy == 0 && x-i>=0 && y-i>=0){

                if(IsMine(x-i,y-i))mine++;
                if(IsEnemy(x-i,y-i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x-i][y-i]);
                    }else if(!IsSuicide(x-i,y-i))v.push_back(moves[x-i][y-i]);
                }
                if(board[x-i][y-i]=='#'||board[x-i][y-i]==' '){
                    if(NEXT){
                        v.push_back(moves[x-i][y-i]);
                    }else if(!IsSuicide(x-i,y-i))v.push_back(moves[x-i][y-i]);
                }

                i++;

            }
        }
        //DOWN LEFT
        i=1;
        mine =0;
        enemy =0;
        if(x<7 && y>0){//SE PUO MUOVERSI VERSO IL BASSO A SINISTRA
            while(mine==0 && enemy == 0 && x+i<=7 && y-i>=0){

                if(IsMine(x+i,y-i))mine++;
                if(IsEnemy(x+i,y-i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x+i][y-i]);
                    }else if(!IsSuicide(x+i,y-i))v.push_back(moves[x+i][y-i]);
                }
                if(board[x+i][y-i]=='#'||board[x+i][y-i]==' '){
                    if(NEXT){
                        v.push_back(moves[x+i][y-i]);
                    }else if(!IsSuicide(x+i,y-i))v.push_back(moves[x+i][y-i]);
                }

                i++;
            }
        }
        //UP RIGHT
        i=1;
        mine =0;
        enemy =0;
        if(y<7 && x>0){//SE PUO MUOVERSI VERSO SU A DESTRA
            while(mine==0 && enemy == 0 && x-i>=0 && y+i<=7){

                if(IsMine(x-i,y+i))mine++;
                if(IsEnemy(x-i,y+i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x-i][y+i]);
                    }else if(!IsSuicide(x-i,y+i))v.push_back(moves[x-i][y+i]);
                }
                if(board[x-i][y+i]=='#'||board[x-i][y+i]==' '){
                    if(NEXT){
                        v.push_back(moves[x-i][y+i]);
                    }else if(!IsSuicide(x-i,y+i))v.push_back(moves[x-i][y+i]);
                }

                i++;
            }
        }

        //DOWN RIGHT
        i=1;
        mine =0;
        enemy =0;
        if(y<7 && x<7){//OVVERO SE PUO MUOVERSI VERSO IL BASSO A DESTRA
            while(mine==0 && enemy == 0 && x+i<=7 && y+i<=7){

                if(IsMine(x+i,y+i))mine++;
                if(IsEnemy(x+i,y+i)){
                    enemy++;
                    if(NEXT){
                        v.push_back(moves[x+i][y+i]);
                    }else if(!IsSuicide(x+i,y+i))v.push_back(moves[x+i][y+i]);
                }
                if(board[x+i][y+i]=='#'||board[x+i][y+i]==' '){
                    if(NEXT){
                        v.push_back(moves[x+i][y+i]);
                    }else if(!IsSuicide(x+i,y+i))v.push_back(moves[x+i][y+i]);
                }

                i++;

            }
        }

        return (!v.empty());
    }

    bool Queen_Moves(int x, int y, vector<string> &v){//logica regine
        Rook_Moves( x,  y, v);
        Bishop_Moves( x, y, v);
        return (!v.empty());
    }

    bool King_Moves(int x, int y, vector<string> &v){ //logica dei re
        //SOPRA DI LUI
        if(x>0){
            if(board[x-1][y]=='#'||board[x-1][y]==' '||IsEnemy(x-1,y)){
                if(NEXT){
                    v.push_back(moves[x-1][y]);
                }else if(!IsSuicide(x-1,y))v.push_back(moves[x-1][y]);
            }
            if(y>0){
                if(board[x-1][y-1]=='#'||board[x-1][y-1]==' '||IsEnemy(x-1,y-1)){
                    if(NEXT){
                        v.push_back(moves[x-1][y-1]);
                    }else if(!IsSuicide(x-1,y-1))v.push_back(moves[x-1][y-1]);
                }
            }
            if(y<SIZE-1){
                if(board[x-1][y+1]=='#'||board[x-1][y+1]==' '||IsEnemy(x-1,y+1)){
                    if(NEXT){
                        v.push_back(moves[x-1][y+1]);
                    }else if(!IsSuicide(x-1,y+1))v.push_back(moves[x-1][y+1]);
                }
            }
        }
        //SOTTO
        if(x<7){
            if(board[x+1][y]=='#'||board[x+1][y]==' '||IsEnemy(x+1,y)){
                if(NEXT){
                        v.push_back(moves[x+1][y]);
                }else if(!IsSuicide(x+1,y))v.push_back(moves[x+1][y]);
            }
            if(y>0){
                if(board[x+1][y-1]=='#'||board[x+1][y-1]==' '||IsEnemy(x+1,y-1)){
                    if(NEXT){
                        v.push_back(moves[x+1][y-1]);
                    }else if(!IsSuicide(x+1,y-1))v.push_back(moves[x+1][y-1]);
                }
            }
            if(y<SIZE-1){
                if(board[x+1][y+1]=='#'||board[x+1][y+1]==' '||IsEnemy(x+1,y+1)){
                    if(NEXT){
                        v.push_back(moves[x+1][y+1]);
                    }else if(!IsSuicide(x+1,y+1))v.push_back(moves[x+1][y+1]);
                }
            }
        }
        //AI LATI
        if(y>0){
            if(board[x][y-1]=='#'||board[x][y-1]==' '||IsEnemy(x,y-1)){
                if(NEXT){
                    v.push_back(moves[x][y-1]);
                }else if(!IsSuicide(x,y-1)){
                    v.push_back(moves[x][y-1]);
                }
            }
        }
        if(y<SIZE-1){
            if(board[x][y+1]=='#'||board[x][y+1]==' '||IsEnemy(x,y+1)){
                if(NEXT){
                    v.push_back(moves[x][y+1]);
                }else if(!IsSuicide(x,y+1))v.push_back(moves[x][y+1]);
            }
        }

        return (!v.empty());
    }

    void PawnPromotion(){//promozione pedone su ultima riga

        if((board[xA][yA]=='P' && (xB==0) )||(board[xA][yA]=='p' && (xB == 7))) {
            string risp = "0";
            int prova =0;
            cout << "Promozione pedone: " << endl;
            cout << "1.Torre" << endl;
            cout << "2.Cavallo" << endl;
            cout << "3.Alfiere" << endl;
            cout << "4.Regina" << endl;
            while (risp != "4" && risp != "3" && risp != "2" && risp != "1") {//"menu" principale

                cout << "Inserisci scelta [1-4]: ";

                cin >> risp;

                if (risp.length() == 1 && isdigit(risp[0])) {

                    int r = atoi(risp.c_str());

                    if (r > 0 && r < 5) {
                        if (board[xA][yA] == 'P') {
                            board[xA][yA] = whites[r];
                            prova++;
                        } else {
                            board[xA][yA] = blacks[r];
                            prova++;
                        }
                    }
                } else cout << "Risposta non valida" << endl;

            }
        }

    }
};

void PrintRegole(){
    cout<<endl<<"REGOLAMENTO SCACCHI PRESO DA: "<<endl;
    cout<<"https://www.chess.com/learn-how-to-play-chess"<<endl<<endl;
    cout<<"ARROCCO: "<<endl;
    cout<<"Se disponibile verrà proposto PRIMA "<<endl;
    cout<<"di scegliere una pedina da muovere."<<endl;
    cout<<"EN PASSANT: "<<endl;
    cout<<"Se disponibile verrà proposto DOPO"<<endl;
    cout<<"aver selezionato un pedone valido. "<<endl<<endl;
}


int main(){
    srand ( time(NULL) );

    cout<<"\n"
          "   _____  _____          _____________    _ _____\n"
          "  / ____|/ ____|   /\\   / ____/ ____| |  | |_   _|\n"
          " | (___ | |       /  \\ | |   | |    | |__| | | |  \n"
          "  \\___ \\| |      / /\\ \\| |   | |    |  __  | | |  \n"
          "  ____) | |____ / ____ \\ |___| |____| |  | |_| |_ \n"
          " |_____/ \\_____/_/    \\_\\_____\\_____|_|  |_|_____|\n"
          "                                                  \n"
          "                                                  "<<endl<<endl;

    string risp = "0";

    while(risp!="4"){//"menu" principale

        cout<<"1.NUOVA PARTITA (P1 VS P2)"<<endl;
        cout<<"2.NUOVA PARTITA (P1 VS CPU)"<<endl;
        cout<<"3.REGOLE"<<endl;
        cout<<"4.ESCI"<<endl;
        cout<<"Inserisci scelta [1-4]: ";

        cin>> risp;

        if(risp.length()==1 && isdigit(risp[0])){

            int r = atoi(risp.c_str());

            switch(r){
                case 1:
                {
                    Chess d(false);
                    d.Start();
                    break;
                }
                case 2:
                {
                    Chess d(true);
                    d.Start();
                    break;
                }
                case 3:
                    PrintRegole();
                    break;
                case 4:
                    cout<<endl<<"Arrivederci";
                    break;
                default:
                    cout<<"Risposta non valida"<<endl;
            }
        }else cout<<"Risposta non valida"<<endl;

    }

    cin.get();

}
