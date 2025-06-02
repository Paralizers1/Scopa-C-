
#include <iostream>
#include <limits>
#include <vector>
using namespace std;
struct carta {
	int numero;
	string seme;
};

struct punti_primiera {
	int Denari = 0;
	int Coppe = 0;
	int Spade = 0;
	int Bastoni = 0;
};



int NumeroCartaPrimiera(int carta){
    //Trasformi il numero della carta in punteggio
    int punti = 10; //dall'8 al 10
    switch(carta){
        case 7:
            punti = 21;
            break;
        case 6:
            punti = 18;
            break;
        case 1:
            punti = 16;
            break;
        case 4:
            punti = 14;
            break;
        case 5:
            punti = 15;
            break;
        case 3:
            punti = 13;
            break;
        case 2:
            punti = 12;
            break;
    }
    return punti;
}

void mescolaMazzo(carta mazzo[]) {
    //Funzione che mescola il mazzo 100 volte con 2 posizione randomiche.
	int posizioneCarta1,posizioneCarta2;
	for(int i=0; i<100; i++) {
		posizioneCarta1=rand()%40;
		posizioneCarta2=rand()%40;
		carta temp = mazzo[posizioneCarta1];
		mazzo[posizioneCarta1] = mazzo[posizioneCarta2];
		mazzo[posizioneCarta2] = temp;
	}
}


void stampaMazzo(carta mazzo[]){
    //Stampo il mazzo
    for(int i = 0;i<40;i++){
        cout<<mazzo[i].numero<<"\t"<<mazzo[i].seme<<endl;
    }
}

void rimuoviCarta(vector<carta> &c,carta n) {
    //Rimuovo una specifica carta dal vector di carte
	for(int i = 0; i<c.size(); i++) {
		if(c[i].numero == n.numero && c[i].seme == n.seme) {
			c.erase(c.begin()+i);
			break;
		}
	}
}



void mostraCarteTavolo(vector<carta> carteTavolo){
    //Stampo le carte sul tavolo
    cout<<"Sul tavolo ci sono: ";
    for(int i = 0;i<carteTavolo.size();i++){
        cout<<carteTavolo[i].numero<<" di "<<carteTavolo[i].seme<<(i != (carteTavolo.size()-1) ? ", " : "");
    }
    cout<<endl<<endl;
}



void crea_combinazioni(int inizio,vector<carta>&v, vector<carta>& combinazione_attuale, int massimo, vector<vector<carta>>& tutte_combinazioni) {
    //Funzione per aggiungere tutte le possibili combinazioni
    if (combinazione_attuale.size() >= 2) {
        tutte_combinazioni.push_back(combinazione_attuale);
    }
    
    for (int i = inizio; i < massimo; ++i) {
        combinazione_attuale.push_back(v[i]);
        crea_combinazioni(i + 1, v,combinazione_attuale, massimo, tutte_combinazioni);
        combinazione_attuale.pop_back();
    }
}

vector<carta> trovaMiglioreCombinazione(vector<carta>& v, carta cercaCarta) {
    //Funzione che permette di trovare la migliore combinazione, restituisce il maggior
    //numero di carte che compone 
    vector<vector<carta>> combinazioni;
    vector<carta> comb;
    crea_combinazioni(0,v, comb, v.size(), combinazioni);

    vector<carta> migliore;

    for (int i = 0; i < combinazioni.size(); ++i) {
        int somma = 0;
        for (int k = 0; k < combinazioni[i].size(); ++k) {
            //Per ogni combinazione per ogni numero lo sommo per vedere a quanto arrivo
            somma += combinazioni[i][k].numero;
        }
    
        if (somma == cercaCarta.numero) {
            //Se la somma equivale al valore della carta
            if (combinazioni[i].size() > migliore.size()) {
                //Se il numero di carte sono maggiori allora risetto il migliore con la nuova combinazioni
                migliore = combinazioni[i];
            }
        }
    }

    return migliore;
}

int controllaCarta(vector<carta> &CarteGiocatore,vector<carta> & carteTavolo,vector<carta> & cartePrese,carta c,int rimuovi = 0){
    //Funzione che controlla la carta e le confronta con le carte sul tavolo per prenderle
    vector<carta> ricontrollare;
    int prese = 0;
	for(int i = 0; i<carteTavolo.size(); i++) {
		if(carteTavolo[i].numero==c.numero) {
		    //Se è uguale rimuovo la carta stessa
			if(rimuovi){
			    rimuoviCarta(carteTavolo,carteTavolo[i]);
			    cartePrese.push_back(carteTavolo[i]);
			    prese = 1;
			}
			
		}
		else if(carteTavolo[i].numero<c.numero) {
		    //Se non è uguale l'aggiungo a ricontrollare
			ricontrollare.push_back(carteTavolo[i]);
		}
	}
	
	
    if(prese == 0 && ricontrollare.size() > 0){
        //Se ho almeno una carta da ricontrollare allora inizio a creare le combinazioni di carte
       vector<carta> combinazioni =  trovaMiglioreCombinazione(carteTavolo,c); //Ottengo la miglior combinazione di carte
       
       prese = combinazioni.size(); //Numero di carte prese
      
       if(prese>0){
           //Inizio a rimuovere le carte ed aggiungerle alle carte prese
            for(carta & c2 : combinazioni){
                if(rimuovi){
                    rimuoviCarta(carteTavolo, c2); //RImuovi carta dal tavolo
                    cartePrese.push_back(c2); //Aggiungi carta a chi le ha prese
                }
            }
        
        }
    }
    //Se Non sono stata presa nessuna carta
    if(prese==0 && rimuovi)carteTavolo.push_back(c); //Aggiungo la carta al tavolo
    else if(prese>0 && rimuovi)cartePrese.push_back(c); //Altrimenti l'aggiungo alle carte prese
    return prese;
}


void giocaCarta(vector<carta> & CarteGiocatore,vector<carta> & carteTavolo,vector<carta> & cartePrese, int ncarta, int & punti, int & prendeBot, int bot = 0) {
    //Funzione per giocare una carta
    carta cartaGiocata = CarteGiocatore[ncarta];
    int p = controllaCarta(CarteGiocatore,carteTavolo,cartePrese,cartaGiocata,1);
    if(p > 0) {
        //Funzione per vedere chi prende
        if(bot) prendeBot = 1;
        else prendeBot = 0;
    }
    if(carteTavolo.size() == 0){
        //Se qualcuno de due fa scopa
        cout<<(bot != 1 ? "Hai " : "Il Bot ha ")<<"fatto scopa!"<<endl<<endl ;
        punti++;
    }
    
    rimuoviCarta(CarteGiocatore,CarteGiocatore[ncarta]); //Funzione che rimuove la carte al giocatore
    mostraCarteTavolo(carteTavolo); //Sampo le carte del tavolo
   
}






int mostraCarteGiocatore(vector<carta> CarteGiocatore) {
    //Funzione che stampa le carte del giocatore e lo fa giocare
	int cartaGiocata = 0;
	cout<<"Tu attualmente hai le seguenti carte: ";
	for(int i = 0; i<CarteGiocatore.size(); i++) {
		cout<<CarteGiocatore[i].numero<<" di "<<CarteGiocatore[i].seme<<"("<<(i+1)<<")"<<(i != (CarteGiocatore.size()-1) ? ", " : "");
	}
	cout<<endl;
	
	//Se ci sono carte da giocare faccio decidere che carta giocare
	int cSize = CarteGiocatore.size(); 
	if(cSize > 0) {
		do {
			cout<<"Che carta vuoi giocare (1-"<<cSize<<"): ";
			cin>>cartaGiocata;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		} while(cartaGiocata == 0 || cartaGiocata < 1 || cartaGiocata > 3 || CarteGiocatore.size() < cartaGiocata);
		return cartaGiocata;
	}
	return 0;
}


int distribuisciCarte(carta mazzo[],int & carta_attuale,vector<carta> & carteTavolo,vector<carta> & CarteGiocatore,vector<carta> & carteBot,vector<carta> & carta_giocate) {
	//Funzione per distribuire le carte ad inizio partita
	int i = 0;
	for(i; i<10; i++) {
	   //Aggiungo alla variabile giocate
		carta_giocate.push_back(mazzo[carta_attuale]);

		if(i>6) {
		     //Distruibuico le carte al giocatore
			CarteGiocatore.push_back(mazzo[carta_attuale]);
		}
		else if(i>3) {
		     //Distruibuico le carte al bot
			carteBot.push_back(mazzo[carta_attuale]);
		}
		else {
		     //Distruibuico le carte a terra
			carteTavolo.push_back(mazzo[carta_attuale]);
		}
		carta_attuale++;

	}
	return i;
}

void distribuisciTurno(carta mazzo[],int & carta_attuale,vector<carta> & carteTavolo,vector<carta> & CarteGiocatore,vector<carta> & carteBot,vector<carta> & carta_giocate) {
	//Funzione che distribuisce la carta ogni turno
	if(carta_attuale==40)return;
	
	//Distribuisco le carte al giocatore
	carta_giocate.push_back(mazzo[carta_attuale]);
	CarteGiocatore.push_back(mazzo[carta_attuale]);
	carta_attuale++; //Incremento la carta attuale
	
	//Distribuisco le carte al bot
	carta_giocate.push_back(mazzo[carta_attuale]);
	carteBot.push_back(mazzo[carta_attuale]);
	carta_attuale++; //Incremento la carta attuale
}


int conteggioCarteFinale(vector<carta> Carte, int & carteDenari, int &setteBello, bool impostaSettebello = false){
    //Funzione che mi fa il conteggio della primiera, aumenta il conteggio delle carte denari e vede di chi è il settebello
    int rpunti_prim = 0;
    punti_primiera punti_p;
    int size = Carte.size();
	for(int i = 0;i<size;i++){
	    int numeroPrimiera = NumeroCartaPrimiera(Carte[i].numero);
	        if(Carte[i].seme ==  "Denari")
	            {
	                carteDenari++; //Incremento il conteggio delle carte di denari
	                if(numeroPrimiera > punti_p.Denari)punti_p.Denari = numeroPrimiera;
	                if(impostaSettebello && Carte[i].numero==7)impostaSettebello=true; //Imposto il sette bello
	            }
	        else if(Carte[i].seme ==  "Spade")
	            {
	                if(numeroPrimiera > punti_p.Spade)punti_p.Spade = numeroPrimiera;
	            }
	        else if(Carte[i].seme ==  "Coppe")
	            {
	                if(numeroPrimiera > punti_p.Coppe)punti_p.Coppe = numeroPrimiera;
	            }
	        else if(Carte[i].seme ==  "Bastoni")
	            {
	                if(numeroPrimiera > punti_p.Bastoni)punti_p.Bastoni  = numeroPrimiera;
	            }
	            
	        //Stampo tutte le carte del giocatore
	        cout<<Carte[i].numero<<" "<<Carte[i].seme<<(i != (size-1) ? ", ":"");
	    }
	    
	    if(punti_p.Denari && punti_p.Spade && punti_p.Coppe && punti_p.Bastoni){
	        //Calcolo i reali punti di premiera
	        rpunti_prim = punti_p.Denari+punti_p.Spade+punti_p.Coppe+punti_p.Bastoni;
	    }
	   
	cout << endl;
	return rpunti_prim;
}


int main()
{
    int carta_attuale = 0;
    vector<carta> carte_giocate;
    vector<carta> CarteGiocatore;
    vector<carta> carteBot;
    
    vector<carta> CartePreseGiocatore;
    vector<carta> cartePreseBot;
    
    vector<carta> carteTavolo;
    int puntiBot = 0;
    int puntiGiocatore = 0;
    int prendeBot = 0;

	srand (time(NULL));
	carta mazzo[40];
	string semi[4] = {"Denari","Bastoni","Spade","Coppe"};
	int conteggio = 0;
	
	//Creo tutte le carte con seme appropriato
	for(int i = 0; i<40; i++) {
		if(i>0&&(i%10) == 0) {
			conteggio++;
		}
		mazzo[i].seme = semi[conteggio];
		mazzo[i].numero = (i%10)+1; //Resto di 10 carte per seme +1
	}
	
	mescolaMazzo(mazzo); //Mescolo il mazzo
	
	//Distribuisco le carte a tutti
	distribuisciCarte(mazzo,carta_attuale,carteTavolo,CarteGiocatore,carteBot,carte_giocate);
	
	//Mostro le carte sul tavolo
	mostraCarteTavolo(carteTavolo);
	
	//Inizio il ciclo per giocare tutte le carte
    do{
    	//Mossa giocatore
    	int cartaGiocata = mostraCarteGiocatore(CarteGiocatore);
    	cartaGiocata--;
    	//Gioco la carta del giocatore
    	giocaCarta(CarteGiocatore,carteTavolo,CartePreseGiocatore,cartaGiocata,puntiGiocatore,prendeBot);
    	
    	
    	//Mossa Bot
    	int migliore = -1;
        int max_prese = 0;
        
        for(int i = 0; i < carteBot.size(); i++) {
            int prese_possibili = controllaCarta(carteBot, carteTavolo, cartePreseBot, carteBot[i], 0); // Modalità "simulazione" (rimuovi=0)
           //Do priorità alle prese con il 7
            if(carteBot[i].numero == 7 && prese_possibili > 0){migliore = i;break;}
            else if(prese_possibili > max_prese) {
                 //Senò prendo quelle con piu prese
                max_prese = prese_possibili;
                migliore = i;
            }
        }
        
        //Se non ho prese con nessuna mano cerco di giocare la prima carta
        if(migliore == -1) {
            migliore = 0;
            //Evito di giocare il 7 con il bot se possibile quindi incremento
            if(carteBot[migliore].numero == 7 && carteBot.size()>1)migliore++;
            if(carteBot[migliore].numero == 7 && carteBot.size()>2)migliore++;
        }
        
        
        //Gioco la carta
        cout << "Il bot sta giocando: " << carteBot[migliore].numero << " " << carteBot[migliore].seme << endl;
        giocaCarta(carteBot, carteTavolo, cartePreseBot, migliore, puntiBot, prendeBot, 1);
        
        //Distribuisco le carte del turno
        distribuisciTurno(mazzo,carta_attuale,carteTavolo,CarteGiocatore,carteBot,carte_giocate);
	}while(CarteGiocatore.size() > 0); //Il while funziona finchè CarteGiocatore ha ancora una carta in mano
	
	
	
	cout<<"Il gioco è finito:"<<endl;
	//Chi ha preso per ultimo prende le ultime carte
	for(int i = 0;i<carteTavolo.size();i++){
    	if(prendeBot > 0){
    	    cartePreseBot.push_back(carteTavolo[i]);
    	}
    	else if(prendeBot == 0){
    	     CartePreseGiocatore.push_back(carteTavolo[i]);
    	}
	}
	carteTavolo.clear();
	
	//Mostro le carte del bot e le scope
	
	int sizeBot= cartePreseBot.size();
	cout<<"Il Bot ha  realizzato le seguenti scope: "<<puntiBot<<" e preso le seguenti carte ("<<sizeBot<<"): ";
	//Per il punteggio delle denari
	int carteDenariBot = 0,carteDenariGiocatore = 0; 
	int primieraBot = 0,primieraGiocatore = 0; 
	int setteBello = 0; //1 in caso di giocatore
	primieraBot = conteggioCarteFinale(cartePreseBot,carteDenariBot,setteBello); //Calcolo punteggio primiera, carte denari
	
	//Mostro le carte del bot e le scope
	int sizeGiocatore=CartePreseGiocatore.size();
	cout<<endl<<"Tu hai realizzato le seguenti scope: "<<puntiGiocatore<<" e preso le seguenti carte ("<<sizeGiocatore<<"): ";
	
	primieraGiocatore = conteggioCarteFinale(CartePreseGiocatore,carteDenariGiocatore,setteBello,true); //Calcolo punteggio primiera, carte denari e sette bello
	
	cout <<endl<<endl;
	
	
	
	//CONTEGGIO FINALE DEI PUNTI SCOPA 
	//Carte a lungo
	if(CartePreseGiocatore.size() > cartePreseBot.size()){
	    cout << "Hai preso piu' carte del bot, quindi guadagni un punto"<<endl;
	    puntiGiocatore++;
	}
	else {
	    cout << "Il Bot ha preso piu' carte di te, quindi guadagna un punto"<<endl;
	    puntiBot++;
	}
	
	
	//Sette bello
	if(setteBello == 1){
	    cout << "Hai preso il sette Bello ( di denari ), quindi guadagni un punto"<<endl;
	    puntiGiocatore++;
	}
	else {
	    cout << "Il Bot ha preso il sette Bello ( di denari ) , quindi guadagna un punto"<<endl;
	    puntiBot++;
	}
	
	
	//Primiera
	if(primieraGiocatore > primieraBot){
	    cout << "Hai realizato la primiera, quindi guadagni un punto"<<endl;
	    puntiGiocatore++;
	}
	else {
	    cout << "Il Bot ha  realizzato la primiera, quindi guadagna un punto"<<endl;
	    puntiBot++;
	}
	
	
	
	//Carte denaro
	if(carteDenariGiocatore > carteDenariBot){
	    cout << "Hai preso piu' carte denari del bot,  quindi guadagni un punto"<<endl;
	    puntiGiocatore++;
	}
	else {
	    cout << "Il Bot ha preso piu' carte denari di te, quindi guadagna un punto"<<endl;
	    puntiBot++;
	}
	
	
	//Punteggi Finali:
	cout<<endl<<"Punteggio Finale: Tu ("<<puntiGiocatore<<") , Bot("<<puntiBot<<")"<<(puntiBot > puntiGiocatore ? ", il Bot ha vinto!" : ", hai vinto!");
	cout << endl;
	
	return 0;
}
