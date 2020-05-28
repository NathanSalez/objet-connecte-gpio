DESTDIR=/var/www/html/quizz/ressources/
EXES=selectionner_reponse.exe montrer_reponse.exe envoyer_signal.exe

all: selectionner_reponse montrer_reponse envoyer_signal

install: all
	chown root $(EXES)
	chmod u+s $(EXES)
	mv $(EXES) $(DESTDIR)
	

selectionner_reponse: selectionner_reponse.c gpio_utils utils
	gcc selectionner_reponse.c gpio_utils.o utils.o -o selectionner_reponse.exe -lwiringPi

montrer_reponse: montrer_reponse.c gpio_utils
	gcc montrer_reponse.c gpio_utils.o -o montrer_reponse.exe -lwiringPi
	
envoyer_signal: envoyer_signal.c
	gcc envoyer_signal.c -o envoyer_signal.exe

gpio_utils: gpio_utils.c
	gcc -c gpio_utils.c
	
utils: utils.c
	gcc -c utils.c
	
clean:
	rm *.exe *.o
