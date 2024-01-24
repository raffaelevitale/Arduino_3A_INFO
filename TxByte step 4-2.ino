/* TRASMISSIONE RIPETITIVA MESSAGGIO SENZA PAUSE */

// - si scorre sui caratteri fino al fondo e al termine si ricomincia 
// - ogni volta che TxByte ha finito si passa al carattere successivo			  

// piedino su cui trasmettere
#define LED 13

// dato da trasmettere
char Messaggio[]="CIAO MAMMA\n";

// struttura trama
int N=8; 
char P='E';
int S=2;  
unsigned long Tb=25;

// gestione attesa
unsigned long tInitAttesa=0;
unsigned long tAttesa=100;

// prototipi
bool TxByte(char dato);

void setup() {
	pinMode(LED, OUTPUT);
	digitalWrite(LED,HIGH);
}

void loop() {
	static int y=0;
	
	if(TxByte(Messaggio[y]))
		y++;
	
	if(Messaggio[y] == '\0')
		y=0;
	
}

bool TxByte(char dato) {
	
	// dato locale da trasmettere in cui verrà salvato il parametro
	static char cattura;
	
	// gestione stato
	static int statoPresente=0;
	static int statoFuturo=0;
	
	// variabili di appoggio
	static bool parita=true; 
	bool bit;
	static int i=0;
	
	// gestione orologio
	static unsigned long tInit=0;
	
	//non usare millis()-tInit>=Tb perchè essendo unsigned
	//quando la differenza fosse negativa si avrebbe
	//un numero enorme positivo (overflow)
	
	if (tInit==0 || millis()>=tInit+Tb) {
		
		switch(statoPresente) {
			case 0:
				digitalWrite(LED,HIGH); //idle
				cattura=dato; //cattura il parametro ricevuto nella vecchia variabile dato che diventa locale
				statoFuturo=1;
				parita=false; 
				tInit=0; //la prossima chiamata passo subito in 1 (partenza asincrona)
				break;
				
			case 1:
				digitalWrite(LED,LOW); //bit di start
				statoFuturo=2;
				i=0;
				tInit=millis(); //inizializza la sosta per il primo tempo di bit
				break;
				
			case 2:
				bit=((cattura &(1<<i))!=0);          
				digitalWrite(LED,bit);
				parita^=bit;
				i++;
				if(i==N)
				{
					if(P=='N')
						statoFuturo=4;
					else
						statoFuturo=3; 
					i=0;
				}
				tInit+=Tb; //inizializza la sosta per il successivo tempo di bit
				break;
				
			case 3:
				if(P=='O')
					parita=!parita;
				digitalWrite(LED,parita);
				statoFuturo=4;
				tInit+=Tb; //inizializza la sosta per il successivo tempo di bit
				break;
				
			case 4:
				digitalWrite(LED,HIGH);
				i++;
				if(i==S)
					statoFuturo=5; // attende ultimo Tb
				tInit+=Tb; //inizializza la sosta per il successivo tempo di bit
				break;
				
			case 5:	// stato di passaggio per attendere il completamento dell'ultimo Tb prima di ritornare true
				statoFuturo=0;
				break;
		} // switch
		
		statoPresente=statoFuturo;
		
	} // if
	
	return (statoPresente==0);
	
} // TxByte