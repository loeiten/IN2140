#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

main() {
  /* deklarasjon av litt datastruktur */
  struct sockaddr_in serveraddr;
  int sock;
  char buf[12];

  /* Opprett socket */
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  /* Null ut serveradresse-struct'en */
  bzero((void *)&serveraddr, sizeof(serveraddr));

  /* Sett domenet til Internett */
  serveraddr.sin_family = AF_INET;

  /* Sett inn internettadressen til kaksi.ifi.uio.no */
  serveraddr.sin_addr.s_addr = inet_addr("129.240.65.193");

  /* Sett portnummer */
  serveraddr.sin_port = htons(2009);

  /* Koble opp */
  connect(sock, (struct sockaddr *)&serveraddr, sizeof serveraddr);

  /* Send data */
  write(sock, "Hei verden!", 11);

  /* les data fra forbindelsen */
  read(sock, buf, 11);

  /* legg til et termineringstegn, og skriv ut til skjerm */
  buf[11] = '\0';
  printf("%s \n", buf);

  /* Steng socketen */
  close(sock);
}
