# Proiect Sincretic 1 - Sistem de Reglare Automată a Temperaturii

## Cerințe Temă (Conform Campus)

   ### Se va realiza un sistem de conducere pe bază de microcontroller care va efectua următoarele funcţii:

   * Încălzirea sistemului până la o temperatura setată T_SET într-un timp t_incalzire
   * Menţinerea acestei temperaturi pentru o perioada de timp tmentinere
   * Răcirea sistemului treptat într-o perioadă de timp t_racire

   ### Interfaţa cu utilizatorul:

   * Sistemul va dispune de un LCD 16x2 pe care va fi afişat meniul, iar în timpul rulăriise vor afişa temperatura setată, temperatura curentă şi timpul rămas din etapa  actuală (t_incalzire, t_mentinere, t_racire).
   * Meniul sistemului va permite modificarea următorilor parametri: T_SET, t_incalzire, t_mentinere, t_racire, KP, KI, KD.
   * Parametrii vor fi salvaţi în memoria nevolatilă. Repornirea sistemului nu va afecta parametrii salvaţi.
   * Navigarea prin meniul echipamentului se va face prin patru butoane: “OK”, “Cancel”, “+”, “-”.
   * Opţional, se poate crea o interfaţă pe PC pentru afişarea unui grafic al temperaturii

   ### Reglarea temperaturii

   * Controlul temperaturii va fi asigurat de un regulator de tip PID
   * Senzorul de temperatură folosit va fi unul de tip LM35 sau echivalent.
   * Elementul de execuţie al sistemului ce va asigura încălzirea senzorului va fi un bec incandescent c.c. de putere >= 5W ce va putea ajunge la o temperatura de minim 50°C.
   * Se va folosi un releu sau un tranzistor pentru a comanda pornirea/oprirea alimentării acestui bec de către microcontroller.

   ### Perturbaţii

   * Se va implementa un generator de numere random (PRNG sau TRNG)
   * Sistemul va dispune de un generator de perturbații care va afecta circuitul de masurare a temperaturii cu valori aleatorii folosind modulul PRNG/TRNG
     implementat anterior.
   * Apăsarea butonului “OK” în timpul rulării unui program de reglare a temperaturii va porni modulul de perturbaţii, iar apăsarea din nou a aceluiaşi buton o va opri.

## Implementare Hardware
* **Microcontroler:** Arduino Uno (ATmega328P)
* **Senzor:** LM35 (scalare 10 mV/°C pe intrarea A0)
* **Afișaj:** LCD 16x2 în mod 4 biți (pini D12, D11, D5, D4, D3, D2)
* **Interfață:** 4 butoane tactile cu INPUT_PULLUP (D6-D9)
* **Actuator:** Modul MOSFET IRF520 (comandă pe D10) + sarcină bec 12V

### Conexiuni Pini (Pinout)
| Periferic | Pin Arduino | Funcție |
| :--- | :--- | :--- |
| LCD RS, E, D4-D7 | D12, D11, D5, D4, D3, D2 | Afișaj paralel 4 biți |
| Senzor LM35 | A0 | Citire analogică temperatură |
| Butoane Up / Down | D6 / D7 | Ajustare Setpoint |
| Butoane Select / Start | D8 / D9 | Meniu / Start-Stop sistem |
| Comandă MOSFET | D10 | Semnal comandă încălzire |

## Structură Repository
* `firmware/`
  * `Afisare_LCD/` - Inițializare și verificare magistrală afișaj.
  * `Proba_Senzor_Caldura/` - Achiziție tensiune LM35 și conversie în °C.
* `docs/` - Imagini cu etapele montajului pe breadboard.