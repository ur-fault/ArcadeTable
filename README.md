# ArcadeTable
Arcade table source code for school projet

## 1. výstup

Během tohoto výstupu jsem se věnoval těmto věcem
1. Příprava IDE a vývojového prostředí na vývoj pro [Arduino](https://www.arduino.cc/)
    - Instalace VS Code, konfigurace a instalace extensionů
        - Arduino
        - Vim emulace
        - Zprovoznění debugování a komunikace s Arduinem
2. Seznámení se s [C++](https://en.wikipedia.org/wiki/C%2B%2B)
    - Základní jazykové útvary
        - Podmínky
        - Cykly
        - Proměnné a datové typy
        - Funkce a metody
    - H/CPP a jejich rozdíly
        - Headers soubory (.h) - soubory obsahující deklarace různých prvků kódu, jako např.
            - Funkce
            - Třídy
            - Struktury
        - CPP soubory (.cpp) - soubory obsahující skutečný kód, tedy definice těchto funkcí a metod
        - Definice a deklarace jsou rozdělené z historických důvodů, dříve měli kompilátory málo paměti a četli kód ze zhora dolů
    - Třídy a struktury
        - Syntaxe
        - Rozdíly
        - Používání operátoru `new`
        - Jejich rozdíly
    - Spouštění kódu
        - Po mnoha pokuses ho spouštím ve [WSL](https://learn.microsoft.com/en-us/windows/wsl/) (Windows Subsystem for Linux), což je funkce Windows 10 a novější, díky které  můžu využívat optimalizované distribude Linuxu přímo přes Windows a s velikou integrací do systému
            - VS Code také běží ve WSL, ale grafické rozhraní je pomocí tunelu ve Windows 
        - Kompilátor G++ od GNU Project
        - Spouštím přímo v terminálu a nevyužívám prozatím ladící nástroje VS Code
3. Počátek práce na hře Snake v terminálu implementovaná v C++
    - Hra je ve složce [/SnakeCpp](https://github.com/ur-fault/ArcadeTable/tree/vystup-1/SnakeCpp)
    - Prozatím umí tyto věci
        - Vykreslění stavu do konzolce
        - Pohyb hráčů - zatím nelze ovládat
        - Pojídání jablíček

---
Repository contents:
- [Base Snake for 2 players implemented in C++](https://github.com/ur-fault/ArcadeTable/tree/master/SnakeCpp). Just reference to look at when writing it for Arduino later

Not existing yet:
- Universal smoething of a framework for controlling LED RGB Matrix and handling user input on Arcade Table
- Snake for Arcade Table
- "Dáma" for Arcade Table
