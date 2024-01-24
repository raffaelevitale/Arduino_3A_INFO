/* TRASMISSIONE CON CODIFICA DIRETTA DELL'AUTOMA */

// - la funzione TxByte non ritorna finch� non ha finito
// - i ritardi sono realizzati con delay()	che si inlooppa

// piedino su cui trasmettere
#define LED 13

// dato da trasmettere
char dato = 'A'; //65 in tabella ascii

// struttura trama
int N = 8;
char P = 'E';
int S = 2;
int Tb = 100;

// gestione stato
int statoPresente = 0;
int statoFuturo = 0;

// variabili di appoggio
bool parita = true;
bool bit = true;
int i = 0;

void setup()
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
}

void loop()
{
	TxByte(dato);
	delay(1000);
}

void TxByte(char dato)
{

	statoPresente = 0;

	while (true)
	{
		switch (statoPresente)
		{
		case 0:
			statoFuturo = 1;
			parita = false;  //per invertire la parità, calcolo la parità dispari
			break;

		case 1:
			digitalWrite(LED, LOW); // bit di start
			delay(Tb);
			statoFuturo = 2;
			i = 0;
			break;

		case 2:
			bit = ((dato & (1 << i)) != 0); 
			/*
				Mi permette di andare a shiftare le posizioni di n posizionie, in base a quanto è scritto all'intero della i, 
				poi una volta shiftato faccio la AND con il dato e poi vedo se è == a 0 vuol dire che il numero era 0, se inceve esce != 0, non sarà 0
			*/
			digitalWrite(LED, bit);
			delay(Tb);
			parita ^= bit;
			i++;
			if (i == N)
			{
				if (P == 'N')
					statoFuturo = 4;
				else
					statoFuturo = 3;
				i = 0;
			}
			break;

		case 3:
			if (P == 'O')  //Dispari
				parita = !parita;
			digitalWrite(LED, parita);
			delay(Tb);
			statoFuturo = 4;
			break;

		case 4:
			digitalWrite(LED, HIGH);
			delay(Tb);
			i++;
			if (i == S)
				return;
			break;
		} // switch

		statoPresente = statoFuturo;

	} // while

} // TxByte