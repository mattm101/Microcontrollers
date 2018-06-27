# Mikrokontrolery
## Opis projektu
Projekt komunikacji oraz obsługi urządzeń peryferyjnych czterech mikrokontrolerów AT89C52.
### Opis połączeń
Detale połączeń widoczne są na schemacie po kliknięciu przycisku Download.  
Wszystkie mikrokontrolery taktowane są zewnętrznym zegarem rezonatora kwarcowego z częstotliwością 11.0592 MHz, oraz połączone są portem szeregowym.   
  
**Mikrokontroler U1**  
  - klawiatura numeryczna 
  - dioda - port P0.2
  - połączenie równoległe z mikrokontrolerem U3 - port P1  
    
**Mikrokontroler U3**  
  - wyświetlacz 7SEGx2
  - przycisk - pin P3.2 (w trybie przerwania INT0)
  - połączenie równoległe z mikrokontrolerem U1 - port P1  
    
**Mikrokontroler U5**  
  - sterownik dwukanałowy silników DC  
    
**Mikrokontroler U8**  
  - wyświetlacz LCD  
    
  ### Scenariusz komunikacji
    
  **Mikrokontroler U3 - 7SEG**  
  Mikrokontroler uruchamia timer0 w trybie 1 - 16bit, bez przeładowania, wyświelacz 7SEG wyświetla stoper odliczający 20 sekund,
  oraz wyświetla przez 500ms odebrany na porcie P1 kod przycisku klawiatury obsługiwanej przez mikrokontroler U1.
  Jeśli U3 nie otrzyma kodu klawiatury w ciągu 20s, generuje przerwanie na pinie INT1, połączonym do U1.
  Przycisk na pinie INT0 generuje przerwanie, którego obsługa restartuje stoper oraz generuje przerwanie INT0 na mikrokontrolerze U1. 
    
  **Mikrokontroler U1 - KEYBOARD**  
  Mikrokontroler odczytuje kod wciśniętego klawisza klawiatury numerycznej, przerwania zewnętrzne INT0, INT1 wyzwalane zboczem opadającym odpowiednio startują, zatrzymują wysyłanie znaków odczytanych na klawiaturze.
  Mikrokontler wysyła znak odczytany na klawiaturze poprzez połączenie równoległe na porcie P1, oraz poprzez port szeregowy. 
    
  **Mikrokontroler U8 - LCD**  
  Mikrokontroler wyświetla na LCD grę snake, odczytując kody znaków na porcie szeregowym, pozwala na zmianę kierunku ruchu węrza. Po odebraniu znaku przekroczenia limitu czasu na ruch tj. 20s, wyłącza ekran. Po otrzymaniu znaku generowanego przez przycisk w mikrokontrolerze U3 restaruje grę oraz wyświetla ekran powitalny.  
    
  **Mikrokontroler U5 - MOTOR**  
  Odczytuje znaki wysyłane przez U1 na porcie szeregowym i odpowiednio wysterowuje pracę silników.
    
## Schemat układu
<p align="center" >
  <img src="circut_schema.png" >
</p>
