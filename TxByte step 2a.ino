/* TRASMISSIONE CON STRUTTURA MULTITASKING senza temporizzazione */

// - la funzione TxByte fa UNA transizione e ritorna immediatamente 
// - il loop() la deve chiamare continuamente per mandare avanti l'automa
// - non essendoci temporizzazioni l'automa gira a manetta			  

// piedino su cui trasmettere
#define LED 13

// dato da trasmettere
char dato='A';

// struttura trama
int N=8; 
char P='E';
int S=2;  
int Tb=100;

// gestione stato
int statoPresente=0;
int statoFuturo=0;

// variabili di appoggio
bool parita=true; 
bool bit=true;
int i=0;

void setup() {
	pinMode(LED, OUTPUT);
	digitalWrite(LED,HIGH);
	
	statoPresente=0;
}

void loop() {	
	TxByte(dato);  
	//delay(Tb);
}

void TxByte(char dato) {
	
	switch(statoPresente) {
		case 0:
			statoFuturo=1;
			parita=false; 
			break;
			
		case 1:
			digitalWrite(LED,LOW); //bit di start
			statoFuturo=2;
			i=0;
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
			break;
			
		case 3:
			if(P=='O')
				parita=!parita;
			digitalWrite(LED,parita);
			statoFuturo=4;
			break;
			
		case 4:
			digitalWrite(LED,HIGH);
			i++;
			if(i==S)
				statoFuturo=0;
			break;
	} // switch
	
	statoPresente=statoFuturo;
} // TxByte