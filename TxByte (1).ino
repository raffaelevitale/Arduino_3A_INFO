/* TRASMISSIONE CON STRUTTURA MULTITASKING con temporizzazione */

// - la funzione TxByte fa UNA transizione e ritorna immediatamente 
// - il loop() la deve chiamare continuamente per mandare avanti l'automa
// - la temporizzazione è fatta con l'orologio di sistema millis()			  

// piedino su cui trasmettere
#define LED 13

// dato da trasmettere
char dato='A';

// struttura trama
int N=8; 
char P='E';
int S=2;  
unsigned long Tb=25;

// gestione stato
int statoPresente=0;
int statoFuturo=0;

// variabili di appoggio
bool parita=true; 
bool bit=true;
int i=0;

// gestione orologio
unsigned long tInit=0;

void setup() {
	pinMode(LED, OUTPUT);
	digitalWrite(LED,HIGH);
	
	statoPresente=0;
	statoFuturo=0;
}

void loop() {	
	TxByte(dato);  
	
}

void TxByte(char dato) {
	
	//non usare millis()-tInit>=Tb perchè essendo unsigned
	//quando la differenza fosse negativa si avrebbe
	//un numero enorme positivo (overflow)
	
	if (tInit==0 || millis()>=tInit+Tb) {
		
		switch(statoPresente) {
			case 0:
				digitalWrite(LED,HIGH); //idle
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
				bit=((dato &(1<<i))!=0);          
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
					statoFuturo=0;
				tInit+=Tb; //inizializza la sosta per il successivo tempo di bit
				break;
		} // switch
		
		statoPresente=statoFuturo;
		
	} // if
	
} // TxByte