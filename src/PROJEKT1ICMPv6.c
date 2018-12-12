#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include "struktura.h"



int main(void) {
	struct rameczka rama;
	struct rameczka *wskaznik = &rama;
	unsigned char ramka[ETH_FRAME_LEN];
	printf("Uruchamiam odbieranie ramek Ethernet.\n");
	//Utworzenie bufora dla odbieranych ramek Ethernet
	char* buffer = (void*) malloc(ETH_FRAME_LEN);
	//Otwarcie gniazda pozwalającego na odbiór wszystkich ramek Ethernet
	int iEthSockHandl = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	//Kontrola czy gniazdo zostało otwarte poprawnie, w przypadku bledu wyświetlenie komunikatu.
	if (iEthSockHandl<0)
	printf("Problem z otwarciem gniazda : %s!\n", strerror(errno));
	//Zmienna do przechowywania rozmiaru odebranych danych
	int iDataLen = 0;
	//Pętla nieskończona do odbierania ramek Ethernet
	while (1) {
	//Odebranie ramki z utworzonego wcześniej gniazda i zapisanie jej do bufora
	iDataLen = recvfrom(iEthSockHandl, buffer, ETH_FRAME_LEN, 0, NULL, NULL);
	//Kontrola czy nie było bledu podczas odbierania ramki
	if (iDataLen == -1)
	printf("Nie moge odebrac ramki: %s! \n", strerror(errno));
	else { //jeśli ramka odebrana poprawnie wyświetlenie jej zawartości
	printf("\nOdebrano ramke Ethernet o rozmiarze: %d [B]\n", iDataLen);
	for (int j=0;j<iDataLen;j++)
	{
		ramka[j]=*(buffer+j);
		printf("%02x ",ramka[j]);
	}
	if (ramka[12]==134 && ramka[13]==221 && ramka[20]==58)
		{
		printf("\n\nRamka ethernet przenosi pakiet ipv6 z zapytaniem icmpv6\n");
		wskaznik->typ=ramka[54];
		wskaznik->kod=ramka[55];
		wskaznik->czeksum[0]=ramka[56];
		wskaznik->czeksum[1]=ramka[57];
		wskaznik->rezerwacja[0]=ramka[58];
		wskaznik->rezerwacja[1]=ramka[59];
		wskaznik->rezerwacja[2]=ramka[60];
		wskaznik->rezerwacja[3]=ramka[61];
		printf("TYP: %02x\n",wskaznik->typ);
		printf("KOD: %02x\n",wskaznik->kod);
		printf("SUMA KONTROLNA: %02x%02x\n",wskaznik->czeksum[0],wskaznik->czeksum[1]);
		printf("REZERWACJA: %02x%02x%02x%02x\n",wskaznik->czeksum[0],wskaznik->czeksum[1],wskaznik->czeksum[2],wskaznik->czeksum[3]);
		}
	else printf("\n \n Ramka ethernet nie przenosi ipv6\n");
	}
	}
	return EXIT_SUCCESS;
	}

/*int main(void) {
	unsigned char ramka[ETH_FRAME_LEN];
	printf("Uruchamiam odbieranie ramek Ethernet.\n");
	//Utworzenie bufora dla odbieranych ramek Ethernet
	char* buffer = (void*) malloc(ETH_FRAME_LEN);
	//Otwarcie gniazda pozwalającego na odbiór wszystkich ramek Ethernet
	int iEthSockHandl = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	//Kontrola czy gniazdo zostało otwarte poprawnie, w przypadku bledu wyświetlenie komunikatu.
	if (iEthSockHandl<0)
	printf("Problem z otwarciem gniazda : %s!\n", strerror(errno));
	//Zmienna do przechowywania rozmiaru odebranych danych
	int iDataLen = 0;
	//Pętla nieskończona do odbierania ramek Ethernet
	while (1) {
	//Odebranie ramki z utworzonego wcześniej gniazda i zapisanie jej do bufora
	iDataLen = recvfrom(iEthSockHandl, buffer, ETH_FRAME_LEN, 0, NULL, NULL);
	//Kontrola czy nie było bledu podczas odbierania ramki
	if (iDataLen == -1)
	printf("Nie moge odebrac ramki: %s! \n", strerror(errno));
	else { //jeśli ramka odebrana poprawnie wyświetlenie jej zawartości
	printf("\nOdebrano ramke Ethernet o rozmiarze: %d [B]\n", iDataLen);
	for (int j=0;j<iDataLen;j++)
	{
		ramka[j]=*(buffer+j);
		printf("%02x ",ramka[j]);
	}
	if (ramka[12]==134 && ramka[13]==221 && ramka[20]==58)
		{
		printf("\n\nRamka ethernet przenosi pakiet ipv6 z zapytaniem icmpv6\n");
		printf("Typ: %02x \n",ramka[54]);
		printf("Kod: %02x \n",ramka[55]);
		printf("Suma kontorlna: %02x%02x \n",ramka[56],ramka[57]);
		printf("Rezerwacja: %02x%02x%02x%02x \n",ramka[58],ramka[59],ramka[60],ramka[61]);
		}
	else printf("\n \n Ramka ethernet nie przenosi ipv6\n");
	}
	}
	return EXIT_SUCCESS;
	}
	*/
